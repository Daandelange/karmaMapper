//
//  shapesScene.h
//  karmaMapper
//
//  Created by Daan de Lange on 14/11/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "KMSettings.h"

#include "ofMain.h"
#include "shapes.h"
#include "ofxXmlSettings.h"

#define KM_SCENE_SAVE_FILE "saveFiles/karmaMapperSceneSettings.xml"
#define KM_DEFAULT_SCENE "defaultScene.xml"

// this class references and serves shapes

// todo:
// implement some shape-serving functions such as getShapesbyVertexNumber(), getShapesByType, getRandomShapes(int _nb), getShapesByGroup(int _group_id), etc.
// this will probably be problematic: http://stackoverflow.com/questions/3488571/does-insertion-of-elements-in-a-vector-damages-a-pointer-to-the-vector
// todo: add static_cast<BasicShape*> everywhere

class shapesScene { // holds the whole shapes "scene"
	
	friend class shapesEditor;

public:
	shapesScene();
	~shapesScene();
	
	// basic functions
	virtual bool isEditorClass();
	void loadLastUsedScene();
	
	// custom functions
	basicShape* insertShape(basicShape* _shape);
	bool removeShape(basicShape* _shape);
	bool shapeExists(const basicShape* i) const;
	bool saveScene( const string _fileName = "" );
	bool loadScene( const string _fileName = "" );
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
