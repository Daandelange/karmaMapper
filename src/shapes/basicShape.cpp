//
//  basicShape.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 22/3/14.
//
//

#include "basicShape.h"

basicShape::basicShape() {
	shapeType = "basicShape";
	
	// set position
	position = ofPoint(0);
	boundingBox = ofRectangle(0,0,0,0);
	isInEditMode = false;
	initialized = false;
	hasError = false;
	activeHandle = -1;
	
	fgColor = ofColor(255);
	bgColor = ofColor(0);
}

basicShape::~basicShape(){
	basicDestroy();
}

// temp (?) (should not be handled by shape)
// only draw the shape, dont do shader nor other graphic stuff
void basicShape::render(){
	
	// prepare for drawing
	ofPushMatrix();
	ofPushStyle();
	ofBeginShape();
	
	ofTranslate(position);
	ofFill();
	
	hasError?ofSetHexColor(0xFF0000):ofSetHexColor(0xFFFFFF);
	
	// draw elements
	for(int i=0; i<points.size(); i++){
		// draw center point
		ofVertex( points[i] );
		
		// connect with prev point
		//if(i>0) ofLine( points[i-1], points[i] );
	}
	ofEndShape(OF_CLOSE);
	
	if(hasError){
		ofSetHexColor(0xFFFFFF);
		ofDrawBitmapString("error", -20, -5);
	}
	
	// reset
	ofPopStyle();
	ofPopMatrix();
}

// called when the new shape is instantiated
void basicShape::basicSpawn(){
	// set position
	basicSpawn( ofPoint( ofGetWidth()/2, ofGetHeight()/2 ), 0 );
}

void basicShape::basicSpawn(ofPoint _pos, unsigned int _shapeId){
	// set position
	position = _pos;
	boundingBox = ofRectangle(0,0,0,0);
	isInEditMode = false;
	initialized = false;
	activeHandle = -1;
	
	// remember ID
	shapeId = _shapeId;
	
	points.clear();
	pointHandlers.clear();
	// call this over-ruleable function so customShapes can do their thing
	spawn();
	
	// todo: use a setup system that needs to be completed before
	// and rm this line
	initialized = true;
	
	// bind event
	ofAddListener(ofEvents().keyPressed, this, &basicShape::editActiveHandle );
}


// 
void basicShape::basicDestroy(){
	// unbind event
	ofRemoveListener(ofEvents().keyPressed, this, &basicShape::editActiveHandle );
	
	// call this over-ruleable function so customShapes can do their thing
	destroy();
}

void basicShape::basicDrawWireframe(){
	ofPushStyle();
	ofPushMatrix();
	
	// show container box
	ofSetColor( fgColor, 30 );
	ofNoFill();
	ofRect( boundingBox );
	
	// show points
	ofSetColor( fgColor );
	ofFill();
	for(int i=0; i<pointHandlers.size(); i++){
		// draw center point
		pointHandlers[i].draw();
		
		// connect with prev point
		//if(i>0) ofLine( points[i-1], points[i] );
	}
	
	// go to center
	ofTranslate( position );
	
	// call this over-ruleable function so customShapes can do their thing
	drawWireframe();
	
	ofPopMatrix();
	ofPopStyle();
}

/*void basicShape::basicReset(){
	points.clear();
	initialized = false;
	
	reset();
}*/

void basicShape::enableEditMode(){
	// remember
	isInEditMode = true;
	activeHandle = -1;
	
	// create point handlers
	pointHandlers.clear();
	pointHandlers.resize( points.size()+1 ); // +1 is for center point, to move the whole shape
	for(int i=0; i<points.size(); i++){
		pointHandlers[i]= movablePoint();
		pointHandlers[i].setup();
		pointHandlers[i].setPosSize( position + points[i], 30 );
		pointHandlers[i].setEditable( true );
	}
	pointHandlers[points.size()] = movablePoint();
	pointHandlers[points.size()].setup();
	pointHandlers[points.size()].setPosSize( position, 30 );
	pointHandlers[points.size()].setEditable( true );
	pointHandlers[points.size()].makeParent( pointHandlers );
	
	// call virtual function for sub-classes
	editModeChanged(isInEditMode);
}

void basicShape::disableEditMode(){
	// remember
	isInEditMode = false;
	activeHandle = -1;
	
	// update position of whole shape
	if(pointHandlers[points.size()].getPos() != position){
		ofPoint difference = pointHandlers[points.size()].getPos() - position;
		//for(int i=0; i<points.size(); i++){
		//	points[i] += difference;
		position += difference;
		//}
	}
	
	// update relative point positions
	for(int i=0; i<points.size(); i++){
		points[i] = pointHandlers[i].getPos() - this->position;
		//cout << points[i] << endl;
	}
	
	// recalc some variables
	updateDimensions();
	
	// free memory
	pointHandlers.clear();
	
	// tell the real shape we're in edit mode
	editModeChanged(isInEditMode);
}

void basicShape::switchEditMode(){
	isInEditMode ? disableEditMode() : enableEditMode();
}

void basicShape::selectNextHandle(){
	// select next
	if( isSelectableHandle(activeHandle+1) ) selectHandle(activeHandle+1);
	else if(activeHandle==-1 ) selectHandle(0); // re-selects	
	else selectHandle(-1); // deselects
}

void basicShape::selectHandle(int _i){
	if( !isSelectableHandle(_i) && _i!=-1 ) return;
	
	if(isSelectableHandle(activeHandle)) pointHandlers[activeHandle].notActive();
	activeHandle = _i;
	pointHandlers[activeHandle].makeActive();
}

void basicShape::editActiveHandle(ofKeyEventArgs& e){
	if(!isInEditMode || !isSelectableHandle(activeHandle)) return;
	
	ofPoint translation;
	
	int amplifier = 1 + ofGetKeyPressed(OF_KEY_SHIFT)*10;
	
	if( e.key == OF_KEY_DOWN ) translation += ofPoint(0,1);
	else if( e.key == OF_KEY_UP ) translation += ofPoint(0,-1);
	else if( e.key == OF_KEY_LEFT ) translation += ofPoint(-1,0);
	else if( e.key == OF_KEY_RIGHT ) translation += ofPoint(1,0);
	
	translateActiveHandle( translation*amplifier );
}

void basicShape::translateActiveHandle(ofPoint _offset){
	//pointHandlers[activeHandle].translate(_offset);
	_offset+=pointHandlers[activeHandle].getPos();
	pointHandlers[activeHandle].onDragOver( _offset.x, _offset.y, 1);
}

bool basicShape::isSelectableHandle(int _i){
	return (_i>=0 && _i<pointHandlers.size() );
}

void basicShape::updateDimensions(){
	
	int minX=0, minY=0, maxX=0, maxY=0;
	for(int i=0; i<points.size(); i++){
		if(points[i].x > maxX) maxX = points[i].x;
		else if(points[i].x < minX) minX = points[i].x;
		
		if(points[i].y > maxY) maxY = points[i].y;
		else if(points[i].y < minY) minY = points[i].y;
	}
	//this->width = abs(maxX-minX);
	//this->height = abs(maxY-minY);
	
	boundingBox.setPosition( position.x + minX, position.y + minY );
	boundingBox.setWidth( abs(maxX-minX) );
	boundingBox.setHeight( abs(maxY-minY) );
	
	
	
}

void basicShape::addXMLValues(ofxXmlSettings* xml, int _nb){
	string nb = ofToString(_nb);
	xml->setValue("RWI:SHAPE_"+nb+":SHAPE_ID", shapeId);
	xml->setValue("RWI:SHAPE_"+nb+":SHAPE_TYPE", shapeType);
	xml->setValue("RWI:SHAPE_"+nb+":SHAPE_CENTER_X", position.x);
	xml->setValue("RWI:SHAPE_"+nb+":SHAPE_CENTER_Y", position.y);
	xml->setValue("RWI:SHAPE_"+nb+":SHAPE_NUM_POINTS", (int) points.size());
	
	for(int i=0; i<points.size(); i++){
		string l = ofToString(i);
		xml->setValue("RWI:SHAPE_"+nb+":POINTS_"+l+":POS_X", points[i].x);
		xml->setValue("RWI:SHAPE_"+nb+":POINTS_"+l+":POS_Y", points[i].y);
	}
	
	// that's all folks !
}

// loads settings from file
bool basicShape::loadXMLValues(ofxXmlSettings *xml, int _nb){
	// todo -> reset something before loading ?
	
	string nb = ofToString(_nb);
	
	// get basic info
	shapeId = xml->getValue("RWI:SHAPE_"+nb+":SHAPE_ID", 0);
	string tmpShapeType = xml->getValue("RWI:SHAPE_"+nb+":SHAPE_TYPE", "basicShape");
	
	// check if everything went OK
	if(tmpShapeType != shapeType){
#ifdef KARMA_DEBUG
		cout << "basicShape::loadXMLValues() -> ["+shapeType+"] can not load settings of shapeType ["+tmpShapeType+"]." << endl;
#endif
		return false;
	}
	
	// get more info
	position.x = xml->getValue("RWI:SHAPE_"+nb+":SHAPE_CENTER_X", ofGetWidth()/2 );
	position.y = xml->getValue("RWI:SHAPE_"+nb+":SHAPE_CENTER_Y", ofGetHeight()/2);

	basicSpawn( position, shapeId);
	int tmpNumPoints = xml->getValue("RWI:SHAPE_"+nb+":SHAPE_NUM_POINTS", (int) points.size());
	
	// load points
	points.resize(tmpNumPoints);
	for(int i=0; i<tmpNumPoints; i++){
		string l = ofToString(i);
		points[i].x = xml->getValue("RWI:SHAPE_"+nb+":POINTS_"+l+":POS_X", 0);
		points[i].y = xml->getValue("RWI:SHAPE_"+nb+":POINTS_"+l+":POS_Y", 0);
	}
	
	// sets height and width
	updateDimensions();
	
	return true;
}

/*void basicShape::setPoints( ofPoint _pts[4] ){
	for(int i=0; i<4; i++) points[i] = _pts[i];
	
	// call thsi over-ruleable "event" function
	pointsUpdated();
} // */

ofPoint basicShape::getCenterOffsetFromBoundingBox(){
	return boundingBox.getPosition()-position;
}

