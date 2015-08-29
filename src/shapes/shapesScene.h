//
//  shapesScene.h
//  karmaMapper
//
//  Created by Daan de Lange on 14/11/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "shapes.h"
#include "ofxXmlSettings.h"

#define shapesScene_SAVE_FILE "saveFiles/appSettings.xml"

// this class references and serves shapes

// todo:
// implement some shape-serving functions such as getShapesbyVertexNumber(), getShapesByType, getRandomShapes(int _nb), getShapesByGroup(int _group_id), etc.
// this will probably be problematic: http://stackoverflow.com/questions/3488571/does-insertion-of-elements-in-a-vector-damages-a-pointer-to-the-vector

class shapesScene { // holds the whole shapes "scene"
	
	friend class shapesEditor;

public:
	shapesScene();
	~shapesScene();
	
	// basic functions
	void setup();
	
	// custom functions
	basicShape* insertShape(basicShape* _shape);
	bool removeShape(basicShape* _shape);
	bool shapeExists(const basicShape* i) const;
	bool saveShapes( const string _fileName );
	bool loadShapes( string _fileName = "" );
	bool unloadShapes();
	
	// utilities
	const unsigned int getNumShapes() const;
	typedef list<basicShape*>::iterator shapesIterator;
	typedef list<basicShape*>::const_iterator shapesIteratorC;
	
protected:
	
	list<basicShape*> shapes; // vectors are uncompatible as they invalidate pointers
	
	// save file stuff
	ofxXmlSettings shapeSettings;

private:
	string loadedConfiguration;
};
