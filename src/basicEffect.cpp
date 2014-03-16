//
//  basicEffect.cpp
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 12/3/14.
//
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
void basicEffect::animate(){
	lifeSpan ++;
}

// resets all values
// overrule this function with your own.
void basicEffect::reset(){
	// todo: do this in _reset() which calls reset();
}

// called after points moved.
// overrule this function with your own.
void basicEffect::pointsUpdated(){
	
}

void basicEffect::setPoints( ofPoint _pts[4] ){
	for(int i=0; i<4; i++) points[i] = _pts[i];
	
	pointsUpdated();
}

// for extension: register points rather then always using 3 points

