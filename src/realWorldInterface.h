//
//  realWorldInterface.h
//  karmaMapper
//
//  Created by Daan de Lange on 27/3/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//
//	This class lets you configure & setup a virtual environment upon the physcal one.
//	Handles different shapes mapped to real world coordinates
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"

class realWorldInterface {

public:

	void setup();
	void update();
	void draw();
	void exit();
	
	basicShape* stringToShapeClass(string _s);
	void addShape();
	void removeShape(int shapeId);
	void removeActiveShape();
	void editShape(int shapeId);
	void editNextShape();
	void resetShapes();
	
	void selectNextShapeHandle();
	
	bool isInEditMode;
	int numShapes;
	
private:
	vector<basicShape*> shapes;
	int activeShape;
	//int activeShapeItem;
	bool isShapesIndex(int i);
	
	// GUI
	ofxPanel gui;
	ofxButton addNewShape;
	ofxButton saveButton;
	ofxButton loadButton;
	ofxButton selectNextShape;
	ofxButton removeButton;
	ofxButton nextHandleButton;
	//ofx
	
	void saveSettings();
	void loadSettings();
	
	// load & save stuff
	int nextSaveId;
	
};
