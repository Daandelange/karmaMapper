//
//  vertexShape.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  vectorFigure
//  Lets you create a shape by placing points
//

#include "vertexShape.h"

#define VECT_SHAPE_DEFAULT_NUM_POINTS 4

vertexShape::vertexShape(){
	shapeType = "vertexShape";
	
	ofAddListener( ofEvents().mousePressed, this, &vertexShape::_mousePressed );
}

vertexShape::~vertexShape(){
	ofRemoveListener( ofEvents().mousePressed, this, &vertexShape::_mousePressed );
}

// spawns the effect @ the scene so it starts rendering (setup)
void vertexShape::spawn(){
	// start with 4 points
	//numPoints = VECT_SHAPE_DEFAULT_NUM_POINTS;
	
	// init variables
	points.clear();
	points.resize( VECT_SHAPE_DEFAULT_NUM_POINTS );
	
	// set points
	for(int i=0; i<points.size(); i++){
		points[i] = ofPoint( cos( (TWO_PI/points.size())*i ), sin( (TWO_PI/points.size())*i ))*100;
	}
	
	// recalculate bounding box
	updateDimensions();
}

void vertexShape::destroy(){
	
}

void vertexShape::drawWireframe(){
	// style and matrix are already pre-set.
	// translation is set to shape origin
	
	// colors are already set by basicShape
	for(int i=0; i<points.size(); i++){
		//ofCircle( points[i], 5);
		ofLine(points[i], points[(i+1)%points.size()]);
	}
}

void vertexShape::editModeChanged(bool editMode){
	
}

// called after points have been moved.
void vertexShape::pointsUpdated(){
	
}

void vertexShape::selectNextHandle(){
	basicShape::selectNextHandle();
}

void vertexShape::selectHandle( int _i){
	basicShape::selectHandle(_i);
}

void vertexShape::translateActiveHandle(ofPoint _offset){
	basicShape::translateActiveHandle(_offset);
}

bool vertexShape::isSelectableItem(int _i){
	return (_i>=0 && _i<points.size() );
}

// custom functions
void vertexShape::_mousePressed( ofMouseEventArgs& args ){
	// dont treat useless clicks
	if(!isInEditMode || !boundingBox.inside(args.x, args.y) ) return;
	
	ofPoint mousePos = ofPoint( args.x, args.y ) - position;
	
	// remove point
	if( (ofGetKeyPressed('r') || ofGetKeyPressed('R')) ){
		
		// check if a point is hovered
		for(int i=0; i< pointHandlers.size(); i++){
			if( pointHandlers[i].isMouseOver() ){
				// remove point
				points.erase(points.begin()+i);
				//delete pointHandlers[i];
				pointHandlers.erase(pointHandlers.begin()+i);
			}
		}
	}
	
	// Add new vertex if clicked on a vertex line
	if(args.button == 2){ // right click
		int addNew = -1;
		
		// Search for the right placer to incert the point in the array
		// Borrowed from https://github.com/patriciogonzalezvivo/ofxComposer
		//
		for (int i = 0; i < points.size(); i++){
			int next = (i+1)%points.size();
			
			ofVec2f AtoM = mousePos - points[i];
			ofVec2f AtoB = points[next] - points[i];
			
			float a = atan2f(AtoM.x, AtoM.y);
			float b = atan2f(AtoB.x, AtoB.y);
			
			if ( abs(a - b) < 0.08){ // define tolerance here
				addNew = next;
			}
		}
		
		// if clicked on a line; add a vertex
		if (addNew >= 0 ){
			points.insert( points.begin()+addNew, mousePos );
			
			if( activeHandle > -1 ) activeHandle = addNew;
			
			// add pointhandler
			// todo : (?) make a reloadPointHandlers function.
			pointHandlers.insert( pointHandlers.begin()+addNew, movablePoint() );
			pointHandlers[addNew].setup();
			pointHandlers[addNew].setPosSize( position + points[addNew], 30 );
			pointHandlers[addNew].setEditable( true );
			
			// call callback
			pointsUpdated();
		}
	}
}

void vertexShape::_mouseDragged( ofMouseEventArgs& args ){
	
}

void vertexShape::_mouseReleased( ofMouseEventArgs& args ){
	
}
