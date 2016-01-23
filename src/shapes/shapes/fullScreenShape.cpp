//
//  fullScreenShape.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  vectorFigure
//  Lets you create a shape by placing points
//

#include "fullScreenShape.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -
fullScreenShape::fullScreenShape(const basicPoint _pos) : vertexShape(_pos) {
	// vertexShape(); has already been called here.
	
	initialiseFullScreenVariables();
	
	ofAddListener( ofEvents().windowResized , this, &fullScreenShape::onResChanged);
}

fullScreenShape::~fullScreenShape(){
	ofRemoveListener( ofEvents().windowResized , this, &fullScreenShape::onResChanged);
}



// - - - - - - -
// OVER-RULED BASICSHAPE FUNCTIONS
// - - - - - - -
void fullScreenShape::initialiseFullScreenVariables(){
	
	myShapeTypes.push_back("fullScreenShape");
	
	// init variables
	points.clear();
	absolutePoints.clear();
	changingPoints.clear();
	
	points.resize(FULLSCREEN_SHAPE_NUM_POINTS);
	absolutePoints.resize(FULLSCREEN_SHAPE_NUM_POINTS);
	changingPoints.resize(FULLSCREEN_SHAPE_NUM_POINTS);
	
	setPointsToScreenRes();
}

// ### LOAD & SAVE
// writes the shape data to XML. xml's cursor is already pushed into the right <shape> tag.
// return success state
bool fullScreenShape::saveToXML(ofxXmlSettings &xml){
	return vertexShape::saveToXML(xml);
}

// load shape settings from xml
// xml's cursor is pushed in the shape to load
bool fullScreenShape::loadFromXML(ofxXmlSettings& xml){
	return vertexShape::loadFromXML(xml);
}

// - - - - - - -
// VERTEX SHAPE FUNCTIONS
// - - - - - - -


// - - - - - - -
// FULLSCREEN SHAPE FUNCTIONS
// - - - - - - -
void fullScreenShape::setPointsToScreenRes(){
	if(points.size()!=FULLSCREEN_SHAPE_NUM_POINTS){
		points.clear();
		points.resize(FULLSCREEN_SHAPE_NUM_POINTS);
	}
	
	auto p=points.begin();
	*p = basicPoint( ofGetWidth()/2.0f*-1.0f, ofGetHeight()/2.0f*-1.0f );
	p++;
	*p = basicPoint( ofGetWidth()/2.0f*-1.0f, ofGetHeight()/2.0f );
	p++;
	*p = basicPoint( ofGetWidth()/2.0f, ofGetHeight()/2.0f );
	p++;
	*p = basicPoint( ofGetWidth()/2.0f, ofGetHeight()/2.0f*-1.0f );
	
	//onShapeEdited();
#ifdef KM_EDITOR_APP
	position.setPos( ofGetWidth()/2.0, ofGetHeight()/2.0);
#else
	position.x = ofGetWidth()/2.0f;
	position.y = ofGetHeight()/2.0f;
#endif
	//basicShape::setPosition( basicPoint( ofGetWidth()/2, ofGetHeight()/2 ) );
	//position = basicPoint( ofGetWidth()/2, ofGetHeight()/2 );
	
	onShapeEdited();
}

void fullScreenShape::onResChanged( ofResizeEventArgs& _args){
	setPointsToScreenRes();
}

// register shape type
SHAPE_REGISTER( fullScreenShape , "fullScreenShape" );