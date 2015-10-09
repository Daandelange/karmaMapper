//
//  distortEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Applies on vertexShape
//	Distorts it's vertexes

#include "distortEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

distortEffect::distortEffect(){
	
	distortEffect::reset();
	
	// effect type must match with class
	effectType = "distortEffect";
}

distortEffect::~distortEffect(){
	
}

// - - - - - - -
// GLOBAL EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
// This function is called before it starts rendering.
// Rendering without having called this function should not be possible.
// isReady() should return true after this is done. (can take a long time)
bool distortEffect::initialise(const animationParams& params){
	// init values
	basicEffect::initialise(params);
	
	isLoading = true;
	isInitialised = false;
	
	// do stuff
	ofEnableAlphaBlending();
	
	// set this when done
	isInitialised = true;
	isLoading = false;
	
	return isInitialised;
}

bool distortEffect::render(const animationParams &params){
	// everything happens in the update!
	//return;
	
	// tmp
	ofSetColor(255);
	ofFill();
	for(std::vector<basicShape*>::iterator it=shapes.begin(); it!=shapes.end(); ++it){
		// todo: basicEffect should have a nice downcast function like .asType<vertexShape>()
		if( (*it)->isType("vertexShape") ){
			if(params.seasons.summer > 0 ){
				//ofEnableAlphaBlending();
				ofSetColor( ofFloatColor(1.f, params.seasons.summer*.1f));
				ofFill();
				
			   (*it)->sendToGPU();
			}
			ofSetColor( ofFloatColor(1.f, 1));
			ofFill();
			
			ofPushMatrix();
			ofTranslate((*it)->getPositionPtr()->x, (*it)->getPositionPtr()->y);
			list<basicPoint>& pts = ((vertexShape*)(*it))->getPoints();
			for(auto it=pts.begin(); it!=pts.end(); ++it){
				ofDrawRectangle((*it).x, (*it).y, 2, 2);
			}
			ofPopMatrix();
		}
	}
	return true;
}

// updates shape data
void distortEffect::update(const animationParams& params){
	
	// do basic Effect function
	basicEffect::update( params );
	
	ofScopedLock lock(effectMutex);
	
	if(shapes.size()<1) return;
	
	for(auto it=shapes.begin(); it!=shapes.end(); it++){
		// todo: basicEffect should have a nice downcast getter function like .asType<vertexShape>()
		if( (*it)->isType("vertexShape") ){
			list<basicPoint>& pts = ((vertexShape*)(*it))->getPoints();
			int offset = 0;
			for(auto itb=pts.begin(); itb!=pts.end(); ++itb){
				(*itb) += basicPoint(
					sin(params.seasons.summer*TWO_PI+offset*params.seasons.summer)*50,
					cos(params.seasons.summer*TWO_PI+offset*params.seasons.summer)*50
				);
				offset++;
			}
			(*it)->onShapeModified();
		}
	}
}

// resets all values
void distortEffect::reset(){
	basicEffect::reset();
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool distortEffect::randomizePresets(){
	if(!basicEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}
