//
//  animationController.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/03/2015.
//
//	This class handles the animation.
//	It links the shapes with effects, controls animation and basically links everything together.
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "effects.h"
#include "shapesDB.h"
#include "ofxGui.h"
#include "animationParamsServer.h"

#include "oscRouter.h"
#include "mirReceiver.h"
#include "durationReceiver.h"
//#include "fboRecorder.h"

#ifdef TARGET_OS_MAC
//	#include "ofxSyphon.h"
#endif

// todo: an overall mask that hides any unwanted projection zones (could be done by an effect too)

class animationController {
	
public:
	animationController( shapesDB& _scene );
	~animationController();
	// fix for no default constructor
	animationController& operator=(const animationController& crap) { return *this; }
	
	// basic functions
	bool start();
	bool stop();
	bool isEnabled() const;

	// listeners
	void update( ofEventArgs& event );
	void draw( ofEventArgs& event );
	//void guiEvent( ofxUIEventArgs& e );
	void _keyPressed( ofKeyEventArgs& e );
	
	// menu listeners
	void showShapeLoadMenu( );
	void showSaveDialog( );
	void setFullScreen( bool& _fullScreen );
	void showAnimationsGui( bool& _b );
	//void enableAnimations( bool & _on );
	
protected:
	// app variables
	bool bIsFullScreen;
	bool bMouseHidden;
	
	// gui
	ofxPanelExtended animationGui;
	ofParameter<string> guiNumEffects, guiNumShapes, guiLoadedShapesScene;
	ofxGuiGroupExtended shortCutsMenu, shapesInfoMenu, effectsMenu;
	ofParameter<bool> bGuiShowAnimParams;
	//ofxMinimalToggle guiShowAnimParamsToggle;
	ofxMinimalButton guiLoadShapesSceneBtn;
// tmp
//	ofxMinimalButton loadButton, saveButton;
//	ofxToggle fullScreenToggle, enableEditingToggle;
//	ofxGuiMatrix batchModeSelect, simpleMode;
//	ofParameter<string> menuNumSelectedShapes;
//	ofxPanelExtended shapeCreationGui;
	
	// effects stuff
	bool isEffectsIndex(int i);
	vector<basicEffect*> effects;
	
	animationParamsServer animationParams;
	
	shapesDB& scene;
	
	// music
	ofSoundPlayer music;
	
	// OSC
	OSCRouter oscRouter;
	mirReceiver mirOSCReceiver;
	//durationReceiver durationOSCReceiver;
	
	// Video recording
	//fboRecorder recorder;
	
private:
	bool bEnabled;
	bool bShowGui;
	
#ifdef TARGET_OS_MAC
        //ofxSyphonServer syphonTexture;
#endif
};


// GUI translations

// Keyboard Shordcuts
#define GUIksMouseHide	("Hide Cursor")
#define GUIksGuiToggle	("Toggle GUI")
#define GUIselected		("Toggle GUI")


// Shapes
#define GUIShapesInfo		("Scene Information")
#define GUILoadScene		("Load Shapes...")
#define GUILoadedScene		("") // empty for having more gui space
#define GUIShapesNumShapes	("# Shapes:\t")


// Effects
#define GUIEffectsTitle			("Animation Information")
#define GUIToggleFullScreen		("Full Screen")
#define GUIEffectsNumEffects	("# Effects:\t")
#define GUIEffectsViewParams	("Show Animation Vars")


