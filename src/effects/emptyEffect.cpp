//
//  emptyEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  blank template for starting myGeniusNewEffect
//

#include "emptyEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

emptyEffect::emptyEffect(){
	
	reset();
	
	// effect type must match with class
	effectType = "emptyEffect";
}

emptyEffect::~emptyEffect(){
	
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

bool emptyEffect::initialise(){
	basicEffect::initialise();
}


// return true if rendering succeeded
bool emptyEffect::render(){
	// No matrix transformations are set, evrything is up to you.
	// below is a default behaviour that you can use as you wish
	
	// prevent rendering is the effect has not been fully initialised
	if( !isReady() ) return false;
	
	// the drawingbox is the minimal zone contains all shapes the effect is bound with
	ofSetColor( 255,0,0 );
	ofNoFill();
	if(overallBoundingBox.width > 0) ofDrawRectangle( overallBoundingBox );
	
	// effects are bound to shapes which to render the effect on
	for(int i=0; i<shapes.size(); i++){
		shapes[i]->sendToGPU();
	}
	
	return true;
}

// like ofApp::update()
void emptyEffect::update(){
	basicEffect::update();
}

// resets all values
void emptyEffect::reset(){
	basicEffect::reset();
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool emptyEffect::randomizePresets(){
	return true;
}


