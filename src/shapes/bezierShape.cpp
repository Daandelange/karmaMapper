//
//  bezierShape.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  vectorFigure
//  Lets you create a shape by placing points then
//

#include "bezierShape.h"

#define BEZIER_SHAPE_DEFAULT_NUM_POINTS 4

bezierShape::bezierShape(){
	shapeType = "bezierShape";
}

bezierShape::~bezierShape(){
	
}

// spawns the effect @ the scene so it starts rendering (setup)
void bezierShape::spawn(){
	// start with 4 points
	numPoints = BEZIER_SHAPE_DEFAULT_NUM_POINTS;
	
	// init variables
	points.clear();
	points.resize( numPoints );
	
	// set points
	for(int i=0; i<numPoints; i++){
		points[i] = ofPoint( cos( (TWO_PI/numPoints)*i ), sin( (TWO_PI/numPoints)*i ))*100;
	}
	
	// set point vector handlers
	calculateVectorPoints(20);
	
	// recalculate bounding box
	updateDimensions();
}

void bezierShape::destroy(){
	
}

void bezierShape::drawWireframe(){
	
	// draw same stuff as parent object
	//basicShape::drawWireframe();
	
	ofPushStyle();
	ofPushMatrix();
	
	ofTranslate( ofGetWidth()/2, ofGetHeight()/2 );
	
	// draw curve thingy underneath gui items
	ofSetColor(fgColor);ofNoFill();
	ofSetColor(0,255,0); // rm me
	//cout << curveShape.size() << endl;
	
	// colors are already set by basicShape
	for(int i=0; i<numPoints; i++){
		
		
		//leftPointHandlers[i].draw();
		// todo: add point position to this (todo before: sync points with pointHandlers)
		ofCircle( points[i]+leftVectors[i], 5); // uncomment me
		//ofCircle( pointHandlers[i].getPos() + leftVectors[i], 10 ); // rm this line
	}
	
	ofSetColor(255,0,0);
	curveShape.draw();
	
	ofPopMatrix();
	ofPopStyle();
}

void bezierShape::editModeChanged(bool editMode){
	
}

// called after points were moved.
void bezierShape::pointsUpdated(){
	
}

void bezierShape::selectNextHandle(){
	
}

void bezierShape::selectHandle( int _i){
	
}

void bezierShape::translateActiveHandle(ofPoint _offset){
	
}

bool bezierShape::isSelectableItem(int _i){
	return (_i>=0 && _i<points.size() ); // same as parent 4tm
}

// custom functions

// automatically assigns vectors
void bezierShape::calculateVectorPoints(int length){
	// ensure polyline has been created & filled
	//if(curveShape.size() != numPoints) return; // todo: notify this "error" ?
	if(numPoints==0){
		ofLogNotice("bezierShape::calculateVectorPoints -> curveShape is empty! Abording.");
	}
	
	// prepare
	leftVectors.resize( numPoints );
	rightVectors.resize( numPoints );
	
	// loop trough points
	for(int i=0; i<numPoints; i++){
		
		if(i==0) curveShape.addVertex(points[i]);
		//else curveShape.curveTo(points[i]);
		else {
			int prevPoint = i-1;
			if(prevPoint<0) prevPoint=numPoints+(prevPoint%numPoints);
			int nextPoint = i+1%numPoints;
			ofPoint curveControls = points[prevPoint] - points[nextPoint];
			curveShape.bezierTo(points[i]-curveControls*.9f, points[i]+curveControls*.9f, points[i]);
			
			// todo = something is wrong here URGENT
		}
		
		/*if(i%2==0){
			curveShape.curveTo(points[i]);
			curveShape.curveTo(points[i]);
		}
		else {
			curveShape.curveTo(points[i]);
			curveShape.curveTo(points[i]);
		} */
		
		/* In this part I tried to calculate them manually, but now I rely on the functionalities of ofPolyLine
		// see http://en.wikipedia.org/wiki/B%C3%A9zier_spline#Approximating_circular_arcs
		float k = 4/3;
		
		int pointBefore = (i-1)%numPoints;
		if(pointBefore<0) pointBefore=numPoints-1;
		int pointAfter = (i+1)%numPoints;
		
		ofPoint orientationL = points[pointBefore] - points[i];
		ofPoint orientationR = points[pointAfter] - points[i];
		ofPoint orientation = (orientationL + orientationR)/2;
		
		leftVectors[i] = points[i] + ofPoint(); // */
		//curveShape.
		cout << "curveShape.size() -> " << curveShape.size() << endl;
	}
	curveShape.close();
	
	// get coordinates from the generated virtual shape
	vector<ofVec3f> centerPoints = curveShape.getVertices();
	for(int ii=0; ii<numPoints; ii++){
		// store them
		points[ii]=ofPoint( centerPoints[ii] );
		leftVectors[ii]=ofPoint( curveShape.getTangentAtIndex(ii) );
		rightVectors[ii]=ofPoint( curveShape.getNormalAtIndex(ii)*2 ); // wrong but this may do what we want
		
		std::cout << "leftVectors[" << ii << "] = " << leftVectors[ii] << std::endl;
	}
	
}


