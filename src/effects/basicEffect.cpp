//
//  basicEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

// todo:
// implement some shape-serving functions such as getShapesbyVertexNumber(), getShapesByType, getRandomShapes(int _nb), getShapesByGroup(int _group_id), etc.
// this will probably be problematic: http://stackoverflow.com/questions/3488571/does-insertion-of-elements-in-a-vector-damages-a-pointer-to-the-vector

#include "basicEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

basicEffect::basicEffect(){
	
	reset();
	
	// effect type mutch match with class
	effectType = "basicEffect";
}

basicEffect::~basicEffect(){
	detachFromAllShapes();
}

// - - - - - - -
// GLOBAL EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
// This function is called before it starts rendering.
// Rendering without having called this function should not be possible.
// isReady() should return true after this is done. (can take a long time)
bool basicEffect::initialise(){
	// init values
	reset();
	
	isLoading = true;
	
	// do specific stuff here
	// eg connect with database, setup a physical device, etc.
	// could this be multi threaded ?
	
	// set this when done
	isInitialised = true;
	isLoading = false;
}

// todo: update -(handled by)-> animation
// returns true if rendering succeeded
bool basicEffect::render(){
	if( !isReady() ) return false;
	
	// draw bounding box
	ofSetColor( 255,0,0 );
	ofNoFill();
	if(overallBoundingBox.width > 0) ofDrawRectangle( overallBoundingBox );
	
	// by default, basicEffect uses the shape's default rendering mode
	for(int i=0; i<shapes.size(); i++){
		shapes[i]->render();
	}
	
	return true;
}

// like ofApp::update()
void basicEffect::update(){
	aliveSince = ofGetSystemTime() - startTime;
}

// resets all values
void basicEffect::reset(){
	// todo: do this in _reset() which then calls reset();
	aliveSince=0;
	startTime=ofGetSystemTime();

	shapes.clear();
	shapes.resize(0);
	
	isInitialised = false;
	hasError = false;
	isEnabled = false;
	isLoading = false;
	
	overallBoundingBox = ofRectangle(0,0,0,0);
}

void basicEffect::enable(){
	isEnabled=true;
}

void basicEffect::disable(){
	isEnabled=false;
}

// - - - - - - -
// EFFECT PROPERTIES
// - - - - - - -

bool basicEffect::isReady() const{
	return isInitialised && !hasError;
}

bool basicEffect::isType(const string _type) const {
	return _type==effectType;
}

string basicEffect::getTypes() const{
	return effectType;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool basicEffect::randomizePresets(){
	return true;
}

void basicEffect::updateBoundingBox(){
	// no shapes ?
	if(shapes.size()==0){
		// return center if no shapes are selected
		overallBoundingBox = ofRectangle( ofGetWidth()/2, ofGetHeight()/2, 0, 0);
		return;
	}
	
	// cache variables
	ofVec2f LTCorner( ofGetWidth(), ofGetHeight() );
	ofVec2f RBCorner( 0, 0);
	
	// analyse all contained boundingBoxes
	for(int i=shapes.size()-1; i>=0; i--){
		
		// cache needed vars
		ofVec2f tmpLTCorner( shapes[i]->getBoundingBox().x, shapes[i]->getBoundingBox().y);
		ofVec2f tmpRBCorner( shapes[i]->getBoundingBox().x+shapes[i]->getBoundingBox().width, shapes[i]->getBoundingBox().y+shapes[i]->getBoundingBox().height);
		
		if(tmpLTCorner.x < LTCorner.x) LTCorner.x = tmpLTCorner.x;
		if(tmpLTCorner.y < LTCorner.y) LTCorner.y = tmpLTCorner.y;
		if(tmpRBCorner.x < RBCorner.x) RBCorner.x = tmpRBCorner.x;
		if(tmpRBCorner.y < RBCorner.y) RBCorner.y = tmpRBCorner.y;
	}
	
	overallBoundingBox = ofRectangle( LTCorner.x, LTCorner.y-LTCorner.x, RBCorner.x, RBCorner.y-LTCorner.y);
	
}

// - - - - - - -
// SHAPE BINDING FUNCTIONS
// - - - - - - -

bool basicEffect::bindWithShape(basicShape* _shape){
	if(_shape==NULL) return false;
	
	// prevent adding the same shape several times
	for(vector<basicShape*>::iterator it=shapes.begin(); it!=shapes.end(); ++it){
		if( _shape == *it ) return true;  // already exists
	}
	
	shapes.push_back( _shape );
	
	updateBoundingBox();
	
	return true;
}

bool basicEffect::detachFromAllShapes(){
	if(shapes.size()==0) return true;
	
	// loop trough shapes
	for(int i=shapes.size()-1; i>=0; i--){
		shapes.erase(shapes.begin()+i);
	}
	
	return (shapes.size()==0); // should always be true
}

// always returns true, just unbinds
bool basicEffect::detachFromShape(basicShape* _shape){
	if(shapes.size()==0) return true;
	
	// loop trough shapes
	for(int i=shapes.size()-1; i>=0; i++){
		if(_shape == shapes[i] ) shapes.erase(shapes.begin()+i);
	}
	
	// no more shapes available for animation ? disable effect!
	if(shapes.size()==0) disable();
	
	// todo: make it request new shapes ? (how?)
	
	return true;
}


