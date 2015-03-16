//
//  shapesServer.h
//  workshopMappingCyril
//
//  Created by Daan de Lange on 14/11/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "shapes.h"
#include "ofxXmlSettings.h"

#define shapesServer_SAVE_FILE "saveFiles/appSettings.xml"

// this class references and serves shapes

// forward declaration
class shapesEditor;

class shapesServer {

public:
	shapesServer();
	~shapesServer();
	
	// basic functions
	void setup();
	
	// custom functions
	basicShape* insertShape(basicShape* _shape);
	bool removeShape(basicShape* _shape);
	bool shapeExists(const basicShape* i) const;
	bool saveShapes( const string _fileName );
	bool loadShapes( string _fileName = "" );
	bool unloadShapes();
	
	// shape serving functions
	list<basicShape*>& getShapesRef(){ return shapes; }
	basicShape* getRandomShape();
	vector<basicShape*> getShapesByType(string _type);
	basicShape* getRandomShapeByType(string _type);
	//vector<basicShape*> getShapesByGroup(int _group);
	//basicShape* getShapeByGroup(int _group);
	
	// utilities
	const unsigned int getNumShapes() const;
	typedef list<basicShape*>::iterator shapesIterator;
	typedef list<basicShape*>::const_iterator shapesIteratorC;
	
private:
	
	list<basicShape*> shapes; // vectors are uncompatible as they invalidate pointers
	
	// save file stuff
	ofxXmlSettings shapeSettings;
	string loadedConfiguration;
	
	
};
