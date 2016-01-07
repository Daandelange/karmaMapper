//
//  vertexShape.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  vectorFigure
//  Lets you create a shape by placing points
//

#include "vertexShape.h"

#define VECT_SHAPE_DEFAULT_NUM_POINTS 4

list<basicPoint> vertexShape::zeroList = list<basicPoint>();

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -
vertexShape::vertexShape(){
	initialiseVertexVariables();
	
#ifdef KM_EDITOR_APP
	buildVertexMenu();
#endif
	
	initialized = true;
}

vertexShape::~vertexShape(){
	//basicShape::~basicShape();
	
}

// - - - - - - -
// OVER-RULED BASICSHAPE FUNCTIONS
// - - - - - - -
// todo: rename this to something less confusing. initialSetup() ( not to confuse with reset() )
void vertexShape::initialiseVertexVariables(){
	
	initialized = false;
	myShapeTypes.push_back("vertexShape");
	
	/*instructions.loadFont("fonts/UbuntuMono.ttf", 12);
	 instructions.setRectangle( ofRectangle(ofGetWidth()-220,20,200,300) );
	 instructions.showsFrame(false);*/
	
	// init variables
	points.resize( VECT_SHAPE_DEFAULT_NUM_POINTS );
	absolutePoints.clear();
	
	// spread 4 points trough space
	int i=0;
	for(auto p = points.begin(); p!=points.end(); p++, i++){
		(*p) = basicPoint( cos( PI-(TWO_PI/points.size())*i ), sin( PI-(TWO_PI/points.size())*i ))*100;
	}
	
	// recalculate bounding box & stuff
	onShapeModified();
	
#ifdef KM_EDITOR_APP
	showInstructions = true;
	//instructions.setText("VERTEX SHAPE\nh		Toggle help.\n[]	Shape vertex selection.\nlr	Shape selection.\nHold r while clicking on a point to remove it.\nRight click on an edge to insert a point.");
	
	menuNumVertexes.set(GUIinfo_numVertexes, "");
#endif
}

void vertexShape::sendToGPU(){
	
	// prepare for drawing
	ofPushMatrix();
	//ofPushStyle();
	// todo: this should be changingPosition
	ofTranslate( getPositionPtr()->x, getPositionPtr()->y);
	
	// if shape has error, draw it in red
#ifdef KM_EDITOR_APP
	ofSetColor(fgColor, 200);
#else
	//ofSetColor(255);
#endif
	
	if(hasError){
		ofSetHexColor(0xFFFFFF);
		ofFill();
	}
	
	//ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	//shared_ptr<ofBaseRenderer> tmp = ofGetCurrentRenderer();
	ofBeginShape();
	// draw elements
	for(auto it = changingPoints.begin(); it != changingPoints.end(); it++){
		// draw center point
		ofVertex( (*it).x, (*it).y );
	}
	ofEndShape(OF_CLOSE);
	
	// reset
	//ofPopStyle();
	ofPopMatrix();
}

// simply (re)computes shape data so that it fits in the boundingbox
void vertexShape::calculateBoundingBox(){
	// reset
	int minX=0, minY=0, maxX=0, maxY=0;
	for(auto it = points.begin(); it != points.end(); it++){
		if((*it).x > maxX) maxX = (*it).x;
		if((*it).x < minX) minX = (*it).x;
		
		if((*it).y > maxY) maxY = (*it).y;
		if((*it).y < minY) minY = (*it).y;
	}
	
	boundingBox.setPosition( position.x + minX, position.y + minY );
	boundingBox.setWidth( abs(maxX-minX) );
	boundingBox.setHeight( abs(maxY-minY) );
}

// updates other shape date after it's been (temporarily) altered (by effect for example)
void vertexShape::onShapeModified(){
	
	// let parent function do it's thing
	basicShape::onShapeModified();
	
	// update relative points to absolute points
	absolutePoints = changingPoints;
	for(auto it = absolutePoints.begin(); it!=absolutePoints.end(); it++ ){
		*it = *it + position;
	}
	
	// todo: update centerPos & more
	
}

// called when the shape has been edited so it can update some ot if't other variables
void vertexShape::onShapeEdited(){
	
	// let parent function do it's thing
	basicShape::onShapeEdited();

	changingPoints = points;
	
	vertexShape::onShapeModified();
	
#ifdef KM_EDITOR_APP
	// update GUI Toggle ?
	guiToggle.setPosition( boundingBox.getTopRight()+5 );
		
		// update displayed information with real-time data
		// todo
		//if( gui!=NULL ) ((ofxUITextArea*) gui->getWidget("info_Vertexes"))->setTextString("Number of Vertexes:  " + ofToString(points.size()));
	
	menuNumVertexes = ofToString( points.size() );
	
	// todo: update centerPos & more
#endif
}


void vertexShape::resetToScene() {
	basicShape::resetToScene();
	
	// syncs original shape data with modifyable data
	onShapeEdited();
}

// ### LOAD & SAVE
// writes the shape data to XML. xml's cursor is already pushed into the right <shape> tag.
// return success state
bool vertexShape::saveToXML(ofxXmlSettings &xml){
	
	
	if(!basicShape::saveToXML(xml)) return false;
	
	xml.addTag("vectors");
	xml.pushTag("vectors");
	//points is a vector<ofPoint> that we want to save to a file
	int p=0;
	for(auto it = points.begin(); it != points.end(); it++, p++){
		//each vector tag represents one point
		xml.addTag("vector");
		xml.pushTag("vector", p);
		//so set the three values in the file
		xml.addValue("X", (*it).x);
		xml.addValue("Y", (*it).y);
		xml.popTag();//pop vector
	}
	xml.popTag(); // pop vectors
	
	return true;
}

// load shape settings from xml
// xml's cursor is pushed in the shape to load
bool vertexShape::loadFromXML(ofxXmlSettings& xml){
	
	// position:X, position:Y and groupID are set by basicShape
	if( !basicShape::loadFromXML(xml) ) return false;
	
	points.clear();
	if( xml.pushTag("vectors") ){
		int numVectors = xml.getNumTags("vector");
		
		for(int i = 0; i < numVectors; i++){
			xml.pushTag("vector", i);
		
			points.push_back( basicPoint(
				xml.getValue("X", 0),
				xml.getValue("Y", 0)
			));
			xml.popTag();
		}
		xml.popTag(); // pop vectors
	
		onShapeEdited();
		
		return true;
	}
	
	return true;
}

// - - - - - - -
// VERTEX SHAPE FUNCTIONS
// - - - - - - -

// ### GETTERS
list<basicPoint>& vertexShape::getPoints( const basicShapePointType& _type ){
	switch( _type ){
		case POINT_POSITION_ABSOLUTE:
			return absolutePoints;
			break;
		case POINT_POSITION_RELATIVE:
			return changingPoints;
			break;
		
		case POINT_POSITION_RELATIVE_UNALTERED:
		default:
			return points;
			break;
	}
}

int vertexShape::getNumPoints(){
	return points.size();
}

// ### UTILITIES

/* / gets alterable vertex reference holding relative point coordinates
 ofVec2f& vertexShape::getRandomVertex(){
	// todo: what if no vertexes ?
	//if( points.size()==0 ) return NULL;
	
	list<ofVec2f>::iterator it = points.begin();
	std::advance(it, round(ofRandom(points.size()-1)) );
	return (*it);
 }*/

// gets alterable vertex pointer holding relative point coordinates
basicPoint* vertexShape::getRandomVertexPtr( const basicShapePointType& _type ){
	if( points.size()==0 ) return &basicPoint::nullPoint;
	
	if( _type == POINT_POSITION_RELATIVE ){
		auto it = changingPoints.begin();
		std::advance(it, round(ofRandom(changingPoints.size()-1)) );
		return &*it;
	}
	else if( _type == POINT_POSITION_ABSOLUTE ){
		auto it = absolutePoints.begin();
		std::advance(it, round(ofRandom(absolutePoints.size()-1)) );
		return &*it;
	}
	else if( _type == POINT_POSITION_RELATIVE_UNALTERED ){
		auto it = points.begin();
		std::advance(it, round(ofRandom(points.size()-1)) );
		return &*it;
	}
	else {
		return &basicPoint::nullPoint;
	}
}

basicPoint* vertexShape::getNextVertexPtr(basicPoint &_p, const basicShapePointType& _type, bool _getPrev){
	if( _type == POINT_POSITION_RELATIVE ){
		auto it= std::find( changingPoints.begin(), changingPoints.end(), _p );
		if( it != changingPoints.end() ){
			_getPrev ? --it : ++it;
			return &*it;
		}
		else return &basicPoint::nullPoint;
	}
	else if( _type == POINT_POSITION_ABSOLUTE ){
		auto it= std::find( absolutePoints.begin(), absolutePoints.end(), _p );
		if( it != absolutePoints.end() ){
			if(_getPrev && it == absolutePoints.begin()) return &*absolutePoints.end();
			// incremment
			_getPrev ? --it : ++it;
			
			if(!_getPrev && it==absolutePoints.end()) return &*absolutePoints.begin();

			return &(*it);
		}
		else return &basicPoint::nullPoint;
	}
	else if( _type == POINT_POSITION_RELATIVE_UNALTERED ){
		auto it= std::find( points.begin(), points.end(), _p );
		if( it != points.end() ){
			_getPrev ? --it : ++it;
			return &*it;
		}
		else return &basicPoint::nullPoint;
	}
	else {
		return &basicPoint::nullPoint;
	}
}

basicPoint* vertexShape::getCenterPtr(){
	// todo
	return &position;
}


#ifdef KM_EDITOR_APP
// - - - - - - -
// EDITING ESSENTIALS
// - - - - - - -
void vertexShape::render(){
	
	ofPushStyle();
	
	// show container box
	ofSetColor( 255, 150 );
	ofNoFill();
	ofDrawRectangle( boundingBox );
	
	// new colors!
	hasError?ofSetHexColor(0xFF0000):ofSetColor( fgColor, 200 );
	ofFill();
	
	// go to center
	ofPushMatrix();
	ofTranslate( position.x, position.y );
	
	// draw line of the shape
	ofBeginShape();
	for(auto it = points.begin(); it != points.end(); it++){
		// draw center point
		ofVertex( (*it).x, (*it).y );
	}
	ofEndShape(OF_CLOSE);
	ofPopMatrix();
	
	// draw pointhandlers ?
	if(bEditMode){
		
		// draw pointhandlers
		int i=0;
		for(auto it = points.begin(); it != points.end(); it++){
			
			(*it).draw();
			
			// show handler ID ?
			if(it->isFocused()) ofDrawBitmapStringHighlight(ofToString(i), (*it).x+position.x+10, (*it).y+position.y+10);
			
			// incremment
			i++;
		}
		
		// draw center position
		position.draw();
		
		// draw gui toggle
		ofSetColor(fgColor, 200);
		ofFill();
		ofDrawRectangle(guiToggle);
		
		// draw additional shape gui.
		if( drawShapeGui ) shapeGui.draw();
		
		// draw instructions
//		ofSetColor(bgColor);
//		ofFill();
//		ofDrawRectangle( instructions.getRect() );
//		ofNoFill();
//		ofSetColor(fgColor);
		//instructions.draw();
	}
	
	// reset
	ofPopStyle();
}

void vertexShape::buildVertexMenu(){
	
	vertexMenu.clear();
	vertexMenu.setup( getShapeType() );
	basicShapeGui.setShowHeader(true);
	
	vertexMenu.add( new ofxLabel(menuNumVertexes) );
	 
	 /*/gui->addSpacer();
	 gui->addLabel("Keyboard Shortcuts", OFX_UI_FONT_MEDIUM);
	 gui->addTextArea("addPoint", "Right click on an edge to insert an additional point", OFX_UI_FONT_SMALL );
	 gui->addTextArea("addPoint", "R + click on point to remove");*/
	
	shapeGui.add( &vertexMenu );
}

bool vertexShape::enableEditMode(){
	// first of all, init vertex shape
	basicShape::enableEditMode();
	
	// enable point handlers
	for(auto it = points.begin(); it != points.end(); it++){
		it->setEditable( true );
		it->setShape(*this, true);
	}
	//position.makeParent( points );
	
	return (bEditMode==true);
}

bool vertexShape::disableEditMode(){
	// prevents executing this twice
	if( bEditMode ){
		
		basicShape::disableEditMode();
		
		// enable point handlers
		for(auto it = points.begin(); it != points.end(); it++){
			it->setEditable( false );
			it->unbindShape();
		}
		//position.removeChildren();
		
		// remember
		bEditMode = false;
		//guiPos = ofVec2f(gui->getRect()->x, gui->getRect()->y);
		
		// recalc some variables
		onShapeEdited();
	}
	
	return (bEditMode==false);
}

// - - - - - - -
// EDITING UI CONTROL
// - - - - - - -

void vertexShape::selectPrevHandle(){
	basicShape::selectPrevHandle();
}

void vertexShape::selectNextHandle(){
	basicShape::selectNextHandle();
}

void vertexShape::selectHandle( basicPoint* _i){
	basicShape::selectHandle(_i);
}

void vertexShape::translateActiveHandle(basicPoint _offset){
	basicShape::translateActiveHandle(_offset);
}

bool vertexShape::handleExists(basicPoint* _i){
	return basicShape::handleExists(_i);// todo
}

// - - - - - - - -
// EDITING UTILITIES
// - - - - - - - -

void vertexShape::applyScale(basicPoint scale){
	// update points
	for(auto it=points.begin(); it!=points.end(); it++){
		(*it) *= scale;
	}
	
	// update
	onShapeEdited();
}

// - - - - - - - -
// GUI LISTENERS
// - - - - - - - -

void vertexShape::keyPressed(ofKeyEventArgs &e){
	basicShape::keyPressed(e);
	
	// delete active point ?
	/*if( (e.key == 'r' || e.key == 'R' ) && handleExists( activeHandle ) ){
		removeHandle(activeHandle);
	}*/
}

// custom functions
bool vertexShape::interceptMouseClick( ofMouseEventArgs& args ){
	if(basicShape::interceptMouseClick(args)) return true;
	
	// dont treat useless clicks
	if(!isInEditMode()) return false;
	
	// restrict to shape area
	// (upsizes rect so point handles are inside)
	ofRectangle biggerRect(boundingBox);
	biggerRect.growToInclude( boundingBox.getTopLeft()-basicPoint::pointSize, boundingBox.getBottomRight()+basicPoint::pointSize );
	if( !biggerRect.inside(args.x, args.y) ) return false;
	
	// check if a point is hovered
	for(auto p=points.begin(); p!=points.end(); p++){
		
		// interact?
		if( p->interceptMouseClick( args ) ){
			
			// remove point ?
			if( ofGetKeyPressed('r') || ofGetKeyPressed('R') ){
				points.erase(p);
				onShapeEdited();
			}
			else {
				// moving is done by basicPoint
				//p->setPos( args.x, args.y );
			}
			
			return true;
		}
	}
	
	// todo : restore this feature
	// Add new vertex if clicked on a vertex line
	if(args.button == 2){ // right click
		
		// Search for the right place to insert the point in the array
		// Based on code from https://github.com/patriciogonzalezvivo/ofxComposer
		//
		basicPoint* prevPoint = &points.back();
		int i=0;
		for(auto p = points.begin(); p!=points.end(); p++, i++){
			
			// obscure math
			// borrowed from https://processing.org/discourse/beta/num_1276644884.html
			// x, y mouse coords
			// x1, x2, y1, y2 = line  coords
			basicPoint mousePos(args.x-position.x, args.y-position.y);
			float dx =  (*prevPoint).x - p->x;
			float dy =  (*prevPoint).y - p->y;
			float d = sqrt( dx*dx + dy*dy );
			float ca = dx/d; // cosine
			float sa = dy/d; // sine
			float mX = (-p->x+mousePos.x)*ca + (-p->y+mousePos.y)*sa;
			
			basicPoint pointOnLine = ( mX <= 0 ) ? *p : (( mX >= d )? *prevPoint : (*p)+basicPoint(mX*ca, mX*sa) );
			
			dx = mousePos.x - pointOnLine.x;
			dy = mousePos.y - pointOnLine.y;
			d = sqrt( dx*dx + dy*dy );// */
			
			/*/ version with atan2
			 // from https://github.com/patriciogonzalezvivo/ofxComposer/blob/master/src/ofxPatch.cpp
			 ofVec2f AtoM = mousePos - *p;
			 ofVec2f AtoB = *prevPoint - *p;
			 
			 float a = atan2f(AtoM.x, AtoM.y);
			 float b = atan2f(AtoB.x, AtoB.y);
			 float d = abs(a-b); // */
			
			// hit ?
			if ( d < 4){ // define tolerance here (distance in px)
				
				// add a vertex
				basicPoint* newPoint =  &*points.insert( p , basicPoint(mousePos.x, mousePos.y) );
				newPoint->setShape(*this, true);
				newPoint->setEditable(true);
				
				// call callback
				onShapeEdited();
				
				// nothing to do anymore
				return true;
			}
			prevPoint = &*p;
		}
	}
	return false;
}

// endif KM_EDITOR_APP
#endif

// register shape type
SHAPE_REGISTER( vertexShape , "vertexShape" );