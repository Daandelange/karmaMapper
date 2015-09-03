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

// todo: crashes when you keep switching edit mode very quickly (ex: hold 'e')

list<basicPoint> vertexShape::zeroList = list<basicPoint>();

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -
vertexShape::vertexShape(){
	initialized = false;
	shapeTypes.push_back("vertexShape");
	
	/*instructions.loadFont("fonts/UbuntuMono.ttf", 12);
	instructions.setRectangle( ofRectangle(ofGetWidth()-220,20,200,300) );
	instructions.showsFrame(false);*/
	
	// init variables
	points.resize( VECT_SHAPE_DEFAULT_NUM_POINTS );
	absolutePoints.resize( VECT_SHAPE_DEFAULT_NUM_POINTS );
	
	// spread 4 points trough space
	int i=0;
	for(auto p = points.begin(); p!=points.end(); p++, i++){
		(*p) = basicPoint( cos( PI-(TWO_PI/points.size())*i ), sin( PI-(TWO_PI/points.size())*i ))*100;
		//cout << "["<< points[i].x << ","<< points[i].y << "]" << endl;//
	}
	
	// recalculate bounding box & stuff
	onShapeChanged();
	
	initialized = true;
}

vertexShape::~vertexShape(){
	//basicShape::~basicShape();
	
}

// - - - - - - -
// OVER-RULED BASICSHAPE FUNCTIONS
// - - - - - - -
void vertexShape::initialiseVariables(){
	// act like basic shape
	basicShape::initialiseVariables();
	
	// do some other stuff
	points.clear();
	absolutePoints.clear();
	
	showInstructions = true;
	//instructions.setText("VERTEX SHAPE\nh		Toggle help.\n[]	Shape vertex selection.\nlr	Shape selection.\nHold r while clicking on a point to remove it.\nRight click on an edge to insert a point.");
	
}

void vertexShape::sendToGPU(){
	
	// prepare for drawing
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(position.x, position.y);
	
	// if shape has error, draw it in red
	ofSetColor(255,255,255,255);
	
	ofBeginShape();
	// draw elements
	for(auto it = points.begin(); it != points.end(); it++){
		// draw center point
		ofVertex( (*it).x, (*it).y );
	}
	ofEndShape(OF_CLOSE);
	
	// reset
	ofPopStyle();
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

// called when shape data changed.
void vertexShape::onShapeChanged(){
	
	// let parent function do it's thing
	basicShape::onShapeChanged();
	
	// update relative points to absolute points
	absolutePoints = points;
	for(auto it = absolutePoints.begin(); it!=absolutePoints.end(); it++ ){
		*it = *it + position;
	}
	
	// update GUI Toggle ?
	if( isInEditMode() ){
		guiToggle.setPosition( boundingBox.getTopRight()+5 );
		
		// update displayed information with real-time data
		// todo
		//if( gui!=NULL ) ((ofxUITextArea*) gui->getWidget("info_Vertexes"))->setTextString("Number of Vertexes:  " + ofToString(points.size()));
	}
	
	// todo: update centerPos & more
	
}

void vertexShape::resetToScene() {
	
}

// ### LOAD & SAVE
// writes the shape data to XML. xml's cursor is already pushed into the right <shape> tag.
// return success state
bool vertexShape::saveToXML(ofxXmlSettings &xml){
	
	// create position tag
	xml.addTag("position");
	xml.pushTag("position");
	xml.addValue("X", getPositionUnaltered().x);
	xml.addValue("Y", getPositionUnaltered().y);
	xml.popTag();
	
	/*/ create GUI position tag
	shapeSettings.addTag("GUIposition");
	shapeSettings.pushTag("GUIposition");
	shapeSettings.addValue("X", (*it)->getGuiPosition().x);
	shapeSettings.addValue("Y", (*it)->getGuiPosition().y);
	shapeSettings.popTag();
	 */
	
	xml.addValue("shapeType", getShapeType() );
	xml.addValue("groupID", getGroupID());
	
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
	
	points.clear();
	
	xml.pushTag("position");
	basicPoint position( xml.getValue("X", 0), xml.getValue("Y", 0) );
	xml.popTag(); // pop position
	
	// todo: must be  a basicShape;
	setGroupID( xml.getValue("groupID", -1) );
	
	xml.pushTag("vectors");
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
	
	onShapeChanged();    
	
	return true; // todo
}

// - - - - - - -
// VERTEX SHAPE FUNCTIONS
// - - - - - - -

// ### GETTERS
list<basicPoint>& vertexShape::getPoints(){
	// restrict this to edit mode
	if( isInEditMode() ){
		// todo -> what is this function for?!?! Rm this ?
		return zeroList;
	}
	
	return points;
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
basicPoint* vertexShape::getRandomVertexPtr(){
	if( points.size()==0 ) return &zeroPoint;
	
	auto it = absolutePoints.begin();
	std::advance(it, round(ofRandom(absolutePoints.size()-1)) );
	return &*it;
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
	//hasError?ofSetHexColor(0xFF0000):ofSetColor(fgColor);
	ofSetColor( fgColor, 200 );
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
			if(it->isFocused()) ofDrawBitmapStringHighlight(ofToString(i), (*it).getPos()+10);
			
			// incremment
			i++;
		}
		
		// draw center position
		position.draw();
		
		// draw gui toggle
		ofSetColor(fgColor, 200);
		ofFill();
		ofDrawRectangle(guiToggle);
		
		// draw instructions
		ofSetColor(bgColor);
		ofFill();
		ofDrawRectangle( instructions.getRect() );
		ofNoFill();
		ofSetColor(fgColor);
		instructions.draw();
	}
	
	// reset
	ofPopStyle();
}

void vertexShape::buildMenu(){
	basicShape::buildMenu();
	
	vertexMenu.clear();
	vertexMenu.add( (new ofxLabelExtended())
				   ->setup("vertexShape", "extended Label")
				   ->setShowLabelName(false)
				   );
	/*
	 gui->addTextArea("info_Vertexes", "Loading..." + ofToString(points.size()), OFX_UI_FONT_SMALL);
	 
	 //gui->addSpacer();
	 gui->addLabel("Keyboard Shortcuts", OFX_UI_FONT_MEDIUM);
	 gui->addTextArea("addPoint", "Right click on an edge to insert an additional point", OFX_UI_FONT_SMALL );
	 gui->addTextArea("addPoint", "R + click on point to remove", OFX_UI_FONT_SMALL );*/
	
	shapeGui->add( &vertexMenu );
}

bool vertexShape::enableEditMode(){
	// first of all, init vertex shape
	basicShape::enableEditMode();
	
	// enable point handlers
	for(auto it = points.begin(); it != points.end(); it++){
		it->setEditable( true );
	}
	position.makeParent( points );
	
	return (bEditMode==true);
}

bool vertexShape::disableEditMode(){
	// prevents executing this twice
	if( bEditMode ){
		
		basicShape::disableEditMode();
		
		// enable point handlers
		for(auto it = points.begin(); it != points.end(); it++){
			it->setEditable( false );
		}
		position.removeChildren();
		
		// remember
		bEditMode = false;
		//guiPos = ofVec2f(gui->getRect()->x, gui->getRect()->y);
		
		// recalc some variables
		onShapeChanged();
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

/*void vertexShape::removeHandle(basicPoint *_i){
	
	// create iterators
	list<ofVec2f>::iterator p=points.end();
	list<ofVec2f>::iterator ap=absolutePoints.end();
	for(list<basicPoint>::iterator h=pointHandlers.end(); h!=pointHandlers.begin(); h--){
		
		if( &*h == _i ){
			
			// deselect
			if(activeHandle==&*h) selectHandle(NULL);
			
			// remove point
			points.erase(p);
			absolutePoints.erase(ap);
			pointHandlers.erase(h);
			
			onShapeChanged();
			
			break;
		}
		p--;
		ap--;
	}
	
}*/

// - - - - - - - -
// EDITING UTILITIES
// - - - - - - - -

// updates shape data from it's respective pointhandlers
bool vertexShape::synchronisePointHandlers(){
	
	/*/ points size not right ?
	if( pointHandlers.size() !=  points.size()){
		// resize one of them
		if(!bEditMode){
			pointHandlers.resize(points.size());
		}
		else{
			absolutePoints.resize(pointHandlers.size());
			points.resize(pointHandlers.size());
		}
	}
	
	// update shape position
	position = positionHandler.getPos();
	
	// update relative point positions to absolute
	list<basicPoint>::iterator ph = pointHandlers.begin();
	for(list<ofVec2f>::iterator pts = points.begin(); pts != points.end(); pts++){
		(*pts) = (*ph).getPos() - position;
		ph++;
	}
	
	// sync shape variables
	onShapeChanged();
	
	return true;
	 */
}

void vertexShape::applyScale(basicPoint scale){
	// update points
	for(auto it=points.begin(); it!=points.end(); it++){
		(*it) *= scale;
	}
	
	// update
	onShapeChanged();
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
	if(!isInEditMode() || !boundingBox.inside(args.x, args.y) ) return false;
	
	basicPoint mousePos = basicPoint( args.x, args.y ) - position;
	
	// check if a point is hovered
	for(auto h=points.rbegin(); h!=points.rend(); h++){
		
		// interact?
		if( h->isMouseOver() ){
			
			// remove point ?
			if( (ofGetKeyPressed('r') || ofGetKeyPressed('R')) ){
				//removeHandle( &*h );
			}
			else {
				h->setPos( args.x, args.y );
			}
			
			return true;
		}
	}
	
	// Add new vertex if clicked on a vertex line
	if(args.button == 2){ // right click
		//ofVec2f* addNew = NULL;
		
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
			float dx =  (*prevPoint).x - p->x;
			float dy =  (*prevPoint).y - p->y;
			float d = sqrt( dx*dx + dy*dy );
			float ca = dx/d; // cosine
			float sa = dy/d; // sine
			float mX = (-p->x+mousePos.x)*ca + (-p->y+mousePos.y)*sa;
			
			basicPoint pointOnLine = ( mX <= 0 ) ? basicPoint(*p) : (( mX >= d )? basicPoint(*prevPoint) : (*p)+basicPoint(mX*ca, mX*sa) );
			
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
				basicPoint* newPoint =  &*points.insert( p , mousePos );
				newPoint->setEditable(true);
				
				//selectHandle(newPoint);
				
				list<basicPoint>::iterator ap = absolutePoints.begin();
				std::advance(ap, i); // calls h++ i times.
				absolutePoints.insert(ap , mousePos + position);
				
				// call callback
				onShapeChanged();
				
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