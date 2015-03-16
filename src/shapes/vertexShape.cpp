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

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -
vertexShape::vertexShape(){
	shapeType = "vertexShape";
	
	ofAddListener( ofEvents().mousePressed, this, &vertexShape::mousePressed );
	
	/*instructions.loadFont("fonts/UbuntuMono.ttf", 12);
	instructions.setRectangle( ofRectangle(ofGetWidth()-220,20,200,300) );
	instructions.showsFrame(false);*/
	
	// init variables
	points.resize( VECT_SHAPE_DEFAULT_NUM_POINTS );
	absolutePoints.resize( VECT_SHAPE_DEFAULT_NUM_POINTS );
	
	// spread 4 points trough space
	int i=0;
	list<ofVec2f>::iterator ap = absolutePoints.begin();
	for(list<ofVec2f>::iterator p = points.begin(); p!=points.end(); p++, i++){
		(*p) = ofPoint( cos( PI-(TWO_PI/points.size())*i ), sin( PI-(TWO_PI/points.size())*i ))*100;
		(*ap) = (*p)+position;
		//cout << "["<< points[i].x << ","<< points[i].y << "]" << endl;//
		
		ap++;
	}
	
	// recalculate bounding box & stuff
	onShapeChanged();
}

vertexShape::~vertexShape(){
	//basicShape::~basicShape();
	
	ofRemoveListener( ofEvents().mousePressed, this, &vertexShape::mousePressed );
}

// - - - - - - -
// OVER-RULED BASICSHAPE FUNCTIONS
// - - - - - - -
void vertexShape::reset(){
	// act like basic shape
	basicShape::reset();
	
	// do some other stuff
	points.clear();
	absolutePoints.clear();
	
	showInstructions = true;
	//instructions.setText("VERTEX SHAPE\nh		Toggle help.\n[]	Shape vertex selection.\nlr	Shape selection.\nHold r while clicking on a point to remove it.\nRight click on an edge to insert a point.");
	
}

void vertexShape::render(){
	// if in edit mode, update points from pointhandlers
	if(bEditMode) synchronisePointHandlers();
	
	// prepare for drawing
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(position);
	
	// if shape has error, draw it in red
	hasError?ofSetHexColor(0xFF0000):ofSetColor(fgColor);
	
	ofBeginShape();
	// draw elements
	for(list<ofVec2f>::iterator it = points.begin(); it != points.end(); it++){
		// draw center point
		ofVertex( (*it).x, (*it).y );
	}
	ofEndShape(OF_CLOSE);
	
	// draw it a 2nd time to smooth the edges
	//ofSetColor(bgColor);
	
	/*ofBeginShape();
	ofNoFill(); // this makes the difference
	// draw elements
	for(int i=0; i<points.size(); i++){
		// draw center point
		ofVertex( points[i].x, points[i].y );
	}
	ofEndShape(OF_CLOSE)//*/
	
	// reset
	ofPopStyle();
	ofPopMatrix();
	
	basicShape::render();
}

void vertexShape::drawForEditor(){
	
	//basicShape::drawForEditor();
	
	// update points from pointhandlers
	if(bEditMode) synchronisePointHandlers();
	
	ofPushStyle();
	
	
	// show container box
	ofSetColor( 255, 150 );
	ofNoFill();
	ofDrawRectangle( boundingBox );
	
	// new colors!
	ofSetColor( fgColor, 200 );
	ofFill();
	
	// go to center
	ofPushMatrix();
	ofTranslate( position );
	
	// draw line of the shape
	ofBeginShape();
	for(list<ofVec2f>::iterator it = points.begin(); it != points.end(); it++){
		// draw center point
		ofVertex( (*it).x, (*it).y );
	}
	ofEndShape(OF_CLOSE);
	ofPopMatrix();
	
	// draw pointhandlers ?
	if(bEditMode){
		
		// draw pointhandlers
		int i=0;
		for(list<movablePoint>::iterator it = pointHandlers.begin(); it != pointHandlers.end(); it++){
			
			(*it).draw();
			
			// show handler ID ?
			if(it->isActive()) ofDrawBitmapStringHighlight(ofToString(i), (*it).getPos()+10);
			
			// incremment
			i++;
		}
		
		// draw center position
		positionPointHandler.draw();
		
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

// simply (re)computes shape data so that it fits in the boundingbox
void vertexShape::calculateBoundingBox(){
	// reset
	int minX=0, minY=0, maxX=0, maxY=0;
	for(list<ofVec2f>::iterator it = points.begin(); it != points.end(); it++){
		if((*it).x > maxX) maxX = (*it).x;
		if((*it).x < minX) minX = (*it).x;
		
		if((*it).y > maxY) maxY = (*it).y;
		if((*it).y < minY) minY = (*it).y;
	}
	
	boundingBox.setPosition( position.x + minX, position.y + minY );
	boundingBox.setWidth( abs(maxX-minX) );
	boundingBox.setHeight( abs(maxY-minY) );
}

void vertexShape::onShapeChanged(){
	// update relative points to absolute points
	absolutePoints = points;
	for(list<ofVec2f>::iterator it = absolutePoints.begin(); it!=absolutePoints.end(); it++ ){
		*it = *it + position;
	}
	
	// let parent function do the rest
	basicShape::onShapeChanged();
	
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
	
	xml.addValue("shapeType", shapeType );
	xml.addValue("groupID", getGroupID());
	
	xml.addTag("vectors");
	xml.pushTag("vectors");
	//points is a vector<ofPoint> that we want to save to a file
	int p=0;
	for(list<ofVec2f>::iterator it = points.begin(); it != points.end(); it++, p++){
		//each vector tag represents one point
		xml.addTag("vector");
		xml.pushTag("vector", p);
		//so set the three values in the file
		xml.addValue("X", (*it).x);
		xml.addValue("Y", (*it).y);
		xml.popTag();//pop vector
	}
	xml.popTag(); // pop vectors
	
	return false;
}

// load shape settings from xml
// xml's cursor is pushed in the shape to load
bool vertexShape::loadFromXML(ofxXmlSettings& xml){
	
	points.clear();
	
	xml.pushTag("position");
	ofVec2f pos( xml.getValue("X", 0), xml.getValue("Y", 0));
	xml.popTag(); // pop position
	
	// todo: must be  a basicShape;
	position = pos;
	setGroupID( xml.getValue("groupID", -1) );
	
	xml.pushTag("vectors");
	int numVectors = xml.getNumTags("vector");
	for(int i = 0; i < numVectors; i++){
		xml.pushTag("vector", i);
		
		ofVec2f p;
		p.x = xml.getValue("X", 0);
		p.y = xml.getValue("Y", 0);
		
		points.push_back(p);
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
list<ofVec2f> & vertexShape::getPoints(){
	// restrict this to edit mode
	if( isInEditMode() ){
		list<ofVec2f> ret;
		ret.resize(0);
		// todo: BAAAAD could cause null pointer to invalidated local reference
		return ret;
	}
	
	return points;
}

/*/ ### SETTERS
 bool basicShape::putPoints( list<ofVec2f>& _points ){ // DEPRECIATED
	// restrict this to edit mode
	if( isInEditMode() ) return false;
	
	bool tmpInEditMode = false;
	
	// it's easier to change points while not in edit mode #dirty
	if( isInEditMode() ){
 disableEditMode();
 tmpInEditMode = true;
	}
	
	// check if we have to re-allocate memory
	if( _points.size() != points.size() ) {
 points.resize( _points.size() );
 absolutePoints.resize( _points.size() );
	}
	
	// transfer points
	points = _points;
	
	// take changes in account
	onShapeChanged();
	
	// restore edit mode ?
	if(tmpInEditMode) enableEditMode();
	
	return true;
 }*/

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
ofVec2f* vertexShape::getRandomVertexPtr(){
	if( points.size()==0 ) return NULL;
	
	list<ofVec2f>::iterator it = absolutePoints.begin();
	std::advance(it, round(ofRandom(absolutePoints.size()-1)) );
	ofVec2f ret = *it;
	return &*it;
}

ofVec2f* vertexShape::getCenterPtr(){
	// todo
	return NULL;
}

// - - - - - - -
// EDIT MODE
// - - - - - - -

bool vertexShape::enableEditMode(){
	// first of all, init vertex shape
	basicShape::enableEditMode();
	
	// create point handlers
	pointHandlers.clear();
	pointHandlers.resize( 0 );
	for(list<ofVec2f>::iterator it = points.begin(); it != points.end(); it++){
		pointHandlers.push_back( movablePoint() );
		pointHandlers.back().setup();
		pointHandlers.back().setPos( position + (*it) );
		pointHandlers.back().setEditable( true );
	}
	positionPointHandler.makeParent( pointHandlers );
	
	gui->addSpacer();
	gui->addLabel("vertexShape", OFX_UI_FONT_MEDIUM);
	gui->addTextArea("info_Vertexes", "Loading..." + ofToString(points.size()), OFX_UI_FONT_SMALL);
	
	//gui->addSpacer();
	gui->addLabel("Keyboard Shortcuts", OFX_UI_FONT_MEDIUM);
	gui->addTextArea("addPoint", "Right click on an edge to insert an additional point", OFX_UI_FONT_SMALL );
	gui->addTextArea("addPoint", "R + click on point to remove", OFX_UI_FONT_SMALL );
	
	// calculate dimensions
	gui->autoSizeToFitWidgets();
	
	return (bEditMode==true);
}

bool vertexShape::disableEditMode(){
	// prevents executing this twice
	if( bEditMode ){
		
		basicShape::disableEditMode();
		
		// remember
		bEditMode = false;
		//guiPos = ofVec2f(gui->getRect()->x, gui->getRect()->y);
		
		// free memory
		pointHandlers.clear();
		
		// recalc some variables
		onShapeChanged();
	}
	
	return (bEditMode==false);
}

// ### HANDLES
// updates shape data from it's respective pointhandlers
bool vertexShape::synchronisePointHandlers(){
	
	// points size not right ?
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
	position = positionPointHandler.getPos();
	
	// update relative point positions to absolute
	list<movablePoint>::iterator ph = pointHandlers.begin();
	for(list<ofVec2f>::iterator pts = points.begin(); pts != points.end(); pts++){
		(*pts) = (*ph).getPos() - position;
		ph++;
	}
	
	// sync shape variables
	onShapeChanged();
	
	// update GUI Toggle ?
	if( isInEditMode() ){
		guiToggle.setPosition( boundingBox.getTopRight()+5 );
		
		// update displayed information with real-time data
		if( gui!=NULL ) ((ofxUITextArea*) gui->getWidget("info_Vertexes"))->setTextString("Number of Vertexes:  " + ofToString(points.size()));
	}
	
	return true;
}
void vertexShape::selectPrevHandle(){
	basicShape::selectPrevHandle();
}

void vertexShape::selectNextHandle(){
	basicShape::selectNextHandle();
}

void vertexShape::selectHandle( movablePoint* _i){
	basicShape::selectHandle(_i);
}

void vertexShape::translateActiveHandle(ofVec2f _offset){
	basicShape::translateActiveHandle(_offset);
}

/*bool vertexShape::isSelectableItem(int _i){
	return (_i>=0 && _i<points.size() );
}*/

void vertexShape::removeHandle(movablePoint *_i){
	
	// create iterators
	list<ofVec2f>::iterator p=points.end();
	list<ofVec2f>::iterator ap=absolutePoints.end();
	for(list<movablePoint>::iterator h=pointHandlers.end(); h!=pointHandlers.begin(); h--){
		
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
	
}

void vertexShape::applyScale(ofVec2f scale){
	// update points
	for(list<ofVec2f>::iterator it=points.begin(); it!=points.end(); it++){
		(*it) *= scale;
	}
	
	// update
	onShapeChanged();
}

void vertexShape::keyPressed(ofKeyEventArgs &e){
	basicShape::keyPressed(e);
	
	// delete active point ?
	if( (e.key == 'r' || e.key == 'R' ) && isSelectableHandle( activeHandle ) ){
		removeHandle(activeHandle);
	}
}

// custom functions
void vertexShape::mousePressed( ofMouseEventArgs& args ){
	// dont treat useless clicks
	if(!isInEditMode() || !boundingBox.inside(args.x, args.y) ) return;
	
	ofVec2f mousePos = ofVec2f( args.x, args.y ) - position;
	
	// remove point
	if( (ofGetKeyPressed('r') || ofGetKeyPressed('R')) ){
		
		// check if a point is hovered
		for(list<movablePoint>::iterator h=pointHandlers.end(); h!=pointHandlers.begin(); h--){
			
			// remove it
			if( h->isMouseOver() ) removeHandle( &*h );
		}
	}
	
	// Add new vertex if clicked on a vertex line
	if(args.button == 2){ // right click
		//ofVec2f* addNew = NULL;
		
		// Search for the right place to insert the point in the array
		// Based on code from https://github.com/patriciogonzalezvivo/ofxComposer
		//
		ofVec2f* prevPoint = &points.back();
		int i=0;
		for(list<ofVec2f>::iterator p = points.begin(); p!=points.end(); p++, i++){
			
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
			
			ofVec2f pointOnLine = ( mX <= 0 ) ? ofVec2f(*p) : (( mX >= d )? ofVec2f(*prevPoint) : (*p)+ofVec2f(mX*ca, mX*sa) );
			
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
				points.insert( p , mousePos );
				
				list<ofVec2f>::iterator ap = absolutePoints.begin();
				std::advance(ap, i); // calls h++ i times.
				absolutePoints.insert(ap , mousePos + position);
				
				// grab iterator for pointHandlers
				list<movablePoint>::iterator ph = pointHandlers.begin();
				std::advance(ph, i); // calls h++ i times.
				
				// add pointhandler
				// todo : (?) make a reloadPointHandlers function.
				movablePoint* newPoint = &*pointHandlers.insert( ph, *new movablePoint() );
				
				newPoint->setup();
				newPoint->setPos( position + mousePos );
				newPoint->setEditable( true );
				
				//selectHandle(newPoint);
				
				// call callback
				onShapeChanged();
				
				// nothing to do anymore
				break;
			}
			prevPoint = &*p;
		}
	}
}

/*void vertexShape::mouseDragged( ofMouseEventArgs& args ){
	
}

void vertexShape::mouseReleased( ofMouseEventArgs& args ){
	
}*/
