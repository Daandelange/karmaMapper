//
//  shapesScene.h
//  karmaMapper
//
//  Created by Daan de Lange on 14/11/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "KMSettings.h"
#include "shapes.h"
#include "ofxXmlSettings.h"


// this class references and serves shapes

// note: this class is focused on not changing the pointer addresses of some *basicShape instances, thus the use of std::list

// todo:
// implement some shape-serving functions such as getShapesbyVertexNumber(), getShapesByType, getRandomShapes(int _nb), getShapesByGroup(int _group_id), etc.

// todo: add static_cast<BasicShape*> everywhere
// todo: add event notifiers onSceneLoaded(arg configurationName) for animation controller 

class shapesScene { // holds the whole shapes "scene"
	
	friend class shapesEditor;

public:
	shapesScene();
	~shapesScene();
	
	// basic functions
	virtual bool isEditorClass() const;
	void loadLastUsedScene();
	string getLoadedScene() const;
	
	// custom functions
	basicShape* insertShape(basicShape* _shape);
	bool removeShape(basicShape* _shape);
	bool shapeExists(const basicShape* i) const;
	bool saveScene( const string& _fileName = "" );
	bool loadScene( const string& _fileName = "" );
	bool unloadShapes();
	
	// utilities
	const unsigned int getNumShapes() const;
	typedef list<basicShape*>::iterator shapesIterator;
	typedef list<basicShape*>::const_iterator shapesIteratorC;
	
protected:
	// Note: vectors are uncompatible as they invalidate pointers
	list<basicShape*> shapes;

private:
	
	string loadedConfiguration;
};
