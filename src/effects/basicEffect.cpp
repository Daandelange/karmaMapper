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

#include "basicEffect.h"

basicEffect::basicEffect(){
	basicEffect::reset();
}

basicEffect::~basicEffect(){
	
}

// spawns the effect @ the scene so it starts rendering (setup)
// overrule this function with your own.
void basicEffect::spawn(){
	reset();
}

// update --> animation
// overrule this function with your own.
void basicEffect::render(){
	pShape->render();
}

void basicEffect::update(){
	lifeSpan ++;
}

// resets all values
// overrule this function with your own.
void basicEffect::reset(){
	// todo: do this in _reset() which calls reset();
	lifeSpan=0;
	startTime=ofGetSystemTime();
	pShape = NULL;
}

// called just before removal
void basicEffect::destroy(){
	
}

void basicEffect::bindWithShape(basicShape* _shape){
	pShape = _shape;
	hasShape = true;
}


