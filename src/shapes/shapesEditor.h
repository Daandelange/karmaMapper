//
//  shapesEditor.h
//  karmaMapper
//
//  Created by Daan de Lange on 14/11/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "shapes.h"
#include "shapesScene.h"
#include "ofxUI.h"
#include "ofxGui.h"

enum shapesEditMode {
	EDIT_MODE_OFF,			// Everything disabled
	EDIT_MODE_RENDER,		// Render the scene
	EDIT_MODE_SHAPE,		// Edit (single) shapes
	EDIT_MODE_BATCH_SELECT,	// Batch - (multi)select shapes
	EDIT_MODE_BATCH_SCALE,	// Batch - scale shapes
	EDIT_MODE_BATCH_MOVE,	// Batch - move shapes
	EDIT_MODE_BATCH_FLIPX,	// Batch - Flip horizontal
	EDIT_MODE_BATCH_FLIPY	// Batch - Flip vertical
};

// The editor lets you load, edit and save scenes
// It adds the UI and handles UI events for shapes

// todo: make the shape edit tools (un)instanciable for a performance gain. Make it dependent on a shapesServer//karmaScene database class which stays loaded for serving raw (+rw) shape data and maybe some utilities.
// todo: add a layer re-ordering panel

class shapesEditor : public shapesScene {

public:
	
	shapesEditor( );
	~shapesEditor();
	
	// application functions
	bool isEditorClass();
	void update();
	void draw();
	
	// edit mode toggling
	void enableEditing();
	void disableEditing();
	void switchEditing();
	bool isInEditMode() const;
	
	// custom functions
	bool addShape(string _shapeType);
	bool removeShape( basicShape* _s );
	void selectShape(basicShape* _i);
	void selectNextShape();
	void selectPrevShape();
	bool setEditMode(shapesEditMode _mode);

	// event listeners
	void _draw( ofEventArgs &e );
	void _update( ofEventArgs &e );
	void guiEvent( ofxUIEventArgs &e);
	void batchGuiEvent( ofxUIEventArgs &e);
	void _mousePressed( ofMouseEventArgs &e);
	void _keyPressed( ofKeyEventArgs &e );
	
	// menu listeners
	void showLoadMenu( );
	void showSaveDialog( );
	void setFullScreen( bool & _fullScreen );
	void enableShapeEditing( bool & _on );

private:
	
private:
	basicShape* activeShape; // only used for comparison, never access it. Not guaranteed to point to an instance.
	shapesEditMode editMode;
	
	// GUI (new)
	ofxPanelExtended editorGui;
	ofxGuiGroupExtended editorMenu, shapesMenu;
	ofxMinimalButton loadButton, saveButton;
	ofxToggle fullScreenToggle, enableEditingToggle;
	ofxGuiMatrix batchModeSelect, simpleMode;
	void buildMenus();
	
	// multi shapes edit stuff
	//ofxUISuperCanvas* batchGui;
	list<basicShape*> multiShapesSelection;
	void updateMultiShapesSelection();
	void syncMultiSelectionHandlers();
	ofRectangle multiShapesBoundingBox;
	vector<basicPoint> multiPointHandlers;

	ofImage background;
	
	bool mouseHidden = false;
	
};

// GUI translations
#define GUIToggleFullScreen		(" Full Screen")
//#define GUIToggleMouseCursor	("Hide Mouse")
#define GUIEnableEditing		("Enable Editing")
#define GUINbSelectedShapes		("Selected shapes")
#define GUISelectNextShape		("Select Next Shape")
#define GUISelectPrevShape		("Select Prev Shape")
#define GUIDeleteShape			("Delete current shape")
#define GUImultiShapesSelection	("Batch Edit...")
#define GUIConfigFile			("Configuration File")
#define GUILoadConfig			("Load...")
#define GUISaveConfig			("Save...")
#define GUIAddShape				("Add Shape...")

#define batchGUISelectAll		("Select All")
#define batchGUISelectNone		("Select None")
#define batchGUICancel			("Close")
#define batchGUIRescaleMode		("Rescale")
#define batchGUIMoveMode		("Move")
#define batchGUIFlipXMode		("Flip Horizontal")
#define batchGUIFlipYMode		("Flip Vertical")
