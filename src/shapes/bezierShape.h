//
//  bezierShape.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"

class bezierShape : public basicShape {
	
public:
	bezierShape();
	~bezierShape();
	
	// inherited by basicShape
	void spawn();
	//void reset();
	void destroy();
	void drawWireframe();
	void editModeChanged(bool editMode);
	void pointsUpdated();
	
	void selectNextHandle();
	void selectHandle(int _i);
	void translateActiveHandle(ofPoint _offset);
	bool isSelectableItem(int _i);
	
	// custom functions
	void calculateVectorPoints(int smooth);
	
private:
	//bool hovered, selected;
	unsigned int numPoints;
	vector<ofPoint> leftVectors;
	vector<ofPoint> rightVectors;
	ofPolyline curveShape;
	
};
