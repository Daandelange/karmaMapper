//
//  basicShape.h
//  karmaMapper
//
//  Created by Daan de Lange on 22/3/14.
//  Defines common methods for shape handling
//

#pragma once

#include "ofMain.h"
#include "movablePoint.h"
#include "ofxXmlSettings.h"

class basicShape {

public:
	basicShape();
	virtual ~basicShape();
	
	virtual void spawn()=0; // called when the new shape is instantiated
	virtual void destroy()=0; // called to destroy the shape
	virtual void drawWireframe()=0; // must be overruled with your own function
	virtual void editModeChanged(bool editMode)=0;
	virtual void pointsUpdated()=0;
	//virtual void reset()=0;

	void basicSpawn();
	void basicSpawn(ofPoint _pos, unsigned int _shapeId);
	void basicDestroy();
	void basicDrawWireframe();
	//void basicReset();
	
	// edit stuff
	void enableEditMode();
	void disableEditMode();
	void switchEditMode();
	
	// detailed edit stuff
	virtual void selectNextHandle();
	virtual bool isSelectableHandle(int _i);
	virtual void selectHandle(int _i);
	void editActiveHandle(ofKeyEventArgs& e);
	virtual void translateActiveHandle(ofPoint _offset);
	int activeHandle;
	
	void updateDimensions();
	
	// todo: implement basicAddXMLValues and make this one virtual
	void addXMLValues(ofxXmlSettings* xml, int _nb);
	bool loadXMLValues(ofxXmlSettings* xml, int _nb);
	
	//void setPoints(ofPoint _pts[4]);
	//void getAbsolutePoints();
	
	ofPoint position; // absolute (inside points will be relative)
	ofRectangle boundingBox;
	vector<ofPoint> points;
	vector<movablePoint> pointHandlers;
	bool initialized;
	
	bool isInEditMode;
	//bool isVisible;
	string shapeType;
	int shapeId; // utility for handling several basicShape() instances.
	ofColor fgColor;
	ofColor bgColor;
	
};
