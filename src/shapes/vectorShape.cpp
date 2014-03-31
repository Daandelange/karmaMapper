//
//  vectorShape.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  vectorFigure
//  Lets you create a shape by placing points then
//

#include "vectorShape.h"

#define VECT_SHAPE_DEFAULT_NUM_POINTS 4

vectorShape::vectorShape(){
	shapeType = "vectorShape";
}

vectorShape::~vectorShape(){
	
}

// spawns the effect @ the scene so it starts rendering (setup)
// overrule this function with your own.
void vectorShape::spawn(){
	// start with 4 points
	numPoints = VECT_SHAPE_DEFAULT_NUM_POINTS;
	
	// spawn points
	points.clear();
	points.resize( numPoints );
	for(int i=0; i<numPoints; i++){
		points[i] = ofPoint( cos( (TWO_PI/numPoints)*i ), sin( (TWO_PI/numPoints)*i ))*100;
	}
	updateDimensions();
}

// resets all values
// overrule this function with your own.
/*void vectorShape::reset(){
	
	//numPoints = VECT_SHAPE_DEFAULT_NUM_POINTS;
	this->spawn();
}*/

void vectorShape::destroy(){
	
}

void vectorShape::drawWireframe(){
	
}

void vectorShape::editModeChanged(bool editMode){
	
}

// called after points were moved.
// overrule this function with your own.
void vectorShape::pointsUpdated(){
	 // same number as before ?
}

bool vectorShape::isSelectableItem(int _i){
	return (_i>=0 && _i<points.size() ); // same as parent 4tm
}