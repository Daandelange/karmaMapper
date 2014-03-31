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
	
}

basicEffect::~basicEffect(){
	
}

// spawns the effect @ the scene so it starts rendering (setup)
// overrule this function with your own.
void basicEffect::spawn(){
	
}

// update --> animation
// overrule this function with your own.
void basicEffect::render(){
	lifeSpan ++;
}

void basicEffect::update(){
	
}

// resets all values
// overrule this function with your own.
void basicEffect::reset(){
	// todo: do this in _reset() which calls reset();
}

// called just before removal
void basicEffect::destroy(){
	
}


