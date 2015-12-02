//
//  shapesEditor.h
//  karmaMapper
//
//  Created by Daan de Lange on 14/11/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#ifdef KM_EDITOR_APP

#pragma once

#include "ofMain.h"
#include "KMSettings.h"

#include "shapes.h"
#include "shapesScene.h"
#include "shapesTransformator.h"
#include "ofxGui.h"

enum shapesEditMode {
	EDIT_MODE_OFF,			// Everything disabled
	EDIT_MODE_RENDER,		// Render the scene
	EDIT_MODE_SHAPE,		// Edit (single) shapes
	EDIT_MODE_BATCH
};

// The editor lets you load, edit and save scenes
// It adds the UI and handles UI events for shapes

// todo: add a layer re-ordering panel
// todo: add contrast mode for configuring shapes easier "on the field"
// todo: edit modes are a mess, clarify & tidy up

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
	bool isInEditMode(shapesEditMode _mode = EDIT_MODE_OFF ) const;
	bool isInEditModeBatch() const;
	bool isInEditModeSingle() const;
	
	// custom functions
	bool addShape(string _shapeType);
	bool removeShape( basicShape* _s );
	void selectShape(basicShape* _i, const bool& preventToggle=false, const bool allowMultiple=false);
	bool hasSelectedShape() const;
	void selectNextShape();
	void selectPrevShape();
	bool setEditMode(shapesEditMode _mode);

	// event listeners
	void _draw( ofEventArgs &e );
	void _update( ofEventArgs &e );
	//void guiEvent( ofxUIEventArgs &e);
	//void batchGuiEvent( ofxUIEventArgs &e);
	void _mousePressed( ofMouseEventArgs &e);
	void _keyPressed( ofKeyEventArgs &e );
	
	// menu listeners
	void showLoadMenu( );
	void showSaveDialog( );
	void setFullScreen( bool & _fullScreen );
	void enableShapeEditing( bool & _on );

private:
	
private:
	// multi shapes edit stuff
	//ofxUISuperCanvas* batchGui;
	list<basicShape*> selectedShapes;
	shapesTransformator multiShapesHandler;
	shapesEditMode editMode;
	
	// GUI (new)
	ofxPanelExtended editorGui;
	ofxGuiGroupExtended editorMenu, shapesMenu;
	ofxMinimalButton loadButton, saveButton;
	ofxToggle fullScreenToggle, enableEditingToggle;
	ofxGuiMatrix batchModeSelect, simpleMode;
	ofParameter<string> menuNumSelectedShapes;
	ofxPanelExtended shapeCreationGui;
	bool shapeCreationGuiVisible;
	list<ofxMinimalButton*> shapeCreationGuiElements;
	basicPoint lastRightClickPosition;
	void buildMenus();
	
	

	ofImage background;
	
	bool mouseHidden;
	
};



// GUI translations
#define GUIToggleFullScreen		(" Full Screen")
//#define GUIToggleMouseCursor	("Hide Mouse")
#define GUIEnableEditing		("Enable Editing")
#define GUINbSelectedShapes		("Selected shapes")
#define GUISelectNextShape		("Select Next Shape")
#define GUISelectPrevShape		("Select Prev Shape")
#define GUIDeleteShape			("Delete current shape")
#define GUIselectedShapes		("Batch Edit...")
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

#endif // KM_EDITOR_APP
