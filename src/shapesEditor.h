//
//  shapesEditor.h
//  workshopMappingCyril
//
//  Created by Daan de Lange on 14/11/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "shapes.h"
#include "shapesServer.h"
#include "ofxUI.h"

#define shapesEditor_SAVE_FILE "saveFiles/appSettings.xml"

enum shapesEditMode {
	EDIT_MODE_OFF,
	EDIT_MODE_NORMAL,
	EDIT_MODE_BATCH_NONE,
	EDIT_MODE_BATCH_SCALE,
	EDIT_MODE_BATCH_MOVE,
	EDIT_MODE_BATCH_FLIPX,
	EDIT_MODE_BATCH_FLIPY
};

// this is a singleton class handling shape modification and serving shapes.

// todo: make the shape edit tools (un)instanciable for a performance gain. Make it dependent on a shapesServer//karmaScene database class which stays loaded for serving raw (+rw) shape data and maybe some utilities.
// effects can also be instanciatated with it's (db) instance for self changing of shapes which it's applied to.
// animator class holds and handles effects.
// animator has several modules which can be loaded and unloaded. They also associate effects (holding preferences) with shapes.
// an effect can be associated with one or more shapes.
// tag effects with #visual #modificator #
//
//
// Maybe this will allow it not to be a singleton anymore.

// forward declaration
class shapesServer;

class shapesEditor {

public:
	
	shapesEditor( shapesServer& _scene );
	~shapesEditor();
	
	// application functions
	void update();
	void draw();
	
	// gui tools
	void enableEditMode();
	void disableEditMode();
	void switchEditMode();
	bool isInEditMode() const;
	
	// custom functions
	bool addShape(string _shapeType);
	bool removeShape( basicShape* _s );
	void selectShape(basicShape* _i);
	void selectNextShape();
	void selectPrevShape();
	bool setEditMode(shapesEditMode _mode);

	// event listeners
	void guiEvent( ofxUIEventArgs &e);
	void batchGuiEvent( ofxUIEventArgs &e);
	void _mousePressed( ofMouseEventArgs &e);
	
private:
	//shapesEditor();
	shapesServer& scene;
	string loadedConfiguration;
	basicShape* activeShape; // only used for comparison, never access it. Not guaranteed to point to an instance.
	shapesEditMode editMode;
	
	// GUI (new)
	ofxUISuperCanvas* gui;
	void buildMenus();
	
	// multi shapes edit stuff
	ofxUISuperCanvas* batchGui;
	list<basicShape*> multiShapesSelection;
	void updateMultiShapesSelection();
	void syncMultiSelectionHandlers();
	ofRectangle multiShapesBoundingBox;
	vector<movablePoint> multiPointHandlers;

	
	
	
};

// GUI translations
#define GUIToggleFullScreen		("Full Screen")
#define GUIToggleMouseCursor	("Hide Mouse")
#define GUISelectNextShape		("Select Next Shape")
#define GUISelectPrevShape		("Select Prev Shape")
#define GUIDeleteShape			("Delete current shape")
#define GUImultiShapesSelection	("Batch Edit...")
#define GUIConfigFile			("Configuration File")
#define GUILoadConfig			("Load")
#define GUISaveConfig			("Save")
#define GUIAddShape				("Add Shape...")

#define batchGUISelectAll		("Select All")
#define batchGUISelectNone		("Select None")
#define batchGUINbSelected		("Selected shapes: ")
#define batchGUICancel			("Close")
#define batchGUIRescaleMode		("Rescale")
#define batchGUIMoveMode		("Move")
#define batchGUIFlipXMode		("Flip Horizontal")
#define batchGUIFlipYMode		("Flip Vertical")
