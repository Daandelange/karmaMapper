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

// addons
#include "ofxImGui.h"
#include "karmaImGuiTheme.h"

#include "ofMain.h"
#include "ofEvents.h"
#include "KMSettings.h"
#include "shapes.h"
#include "effects.h"
#include "shapesDB.h"
#include "animationParamsServer.h"
#include "karmaModule.h"
#include "karmaConsole.h"
#include "animationControllerEvents.h"
#include "karmaFboLayer.h"
#include "karmaUtilities.h"
#include "ofxMSATimer.h"

#include "OSCRouter.h"
#include "mirReceiver.h"
#include "durationReceiver.h"

// TODO
// Ensure shape names are always UNIQUE

class animationController {
	// let ImGui control app variables
	//friend class ofxImGui;
	
public:
	animationController( shapesDB& _scene );
	~animationController();
	// fix for no default constructor
	animationController& operator=(const animationController& crap) { return *this; }
	
	// basic functions
	bool start();
	bool stop();
	bool isEnabled() const;
	bool removeEffect( basicEffect* _e);
	bool addEffect( basicEffect* _e, karmaFboLayer& _layer );
	bool removeLayer( karmaFboLayer& _layer );

	// load & save
	bool loadConfiguration(const string& _file = "");
	bool loadLastConfiguration();
	bool saveConfiguration(const string& _filePath = "");
	string loadedConfiguration;
	bool unloadAllEffects();
	bool unloadAllLayers();
	void newConfiguration();
	bool unbindAllShapes();
	bool unloadAllModules();
	karmaModule* tryLoadModule(const string& _moduleType);
	karmaModule* getModule(const string& _moduleType);
	
	// getters
	unsigned int getNumEffects() const;
	vector<basicEffect*> getEffectsOfType(string _type);
	map<string, vector<basicEffect*> > getAllEffectsByType() const;
	unsigned int getNumLayers() const;
	
	// event handlers
	void update( ofEventArgs& event );
	void draw( ofEventArgs& event );
	void _keyPressed( ofKeyEventArgs& e );
	void _mousePressed( ofMouseEventArgs& e );
	
	static ofEvent<karmaControllerDrawEventArgs> karmaControllerBeforeDraw;
	static ofEvent<karmaControllerDrawEventArgs> karmaControllerAfterDraw;
	
	// menu listeners
	void showShapeLoadMenu( );
	void showSaveDialog( );
	void setFullScreen( bool _fullScreen );
	void setShowMouse( bool _showMouse );
	void showAnimationsGui( bool& _b );
	//void enableAnimations( bool & _on );
	
protected:
	// app variables
	bool bIsFullScreen;
	bool bShowMouse;
	bool bShowGui;
	bool bGuiShowMainWindow;
	bool bGuiShowAnimParams;
	bool bGuiShowPlugins;
	bool bGuiShowModules;
	bool bGuiShowConsole;
	bool bDisplayFullSize;
	int displayFullSizeOffset[2];
	
	// gui
	ofxImGui::Gui gui;
	
	// effects stuff
	bool isEffectsIndex(int i);
	//list<basicEffect*> effects;
	//list<ofxSwapBuffer> layers;
	
	list< karmaFboLayer::fboWithEffects > layers; // ping-pong
	//map<karmaFboLayer*, list<basicEffect*>, karmaFboLayer::orderByIndex > layers; // ping pong
	
	animationParamsServer animationParams;
	list< karmaModule* > modules;
	
	shapesDB& scene;
	
	ofxMSATimer idleTimeTimer;
	uint32_t currentIdleTime;
	//ofxMSATimer appLoadTimer;
	
	//durationReceiver durationOSCReceiver;
	
private:
	bool bEnabled;
	
};


// GUI translations

// Keyboard Shordcuts
#define GUIMouseHide	("Hide Cursor")
#define GUIGuiToggle	("Toggle GUI")
#define GUIselected		("Toggle GUI")


// Shapes
#define GUIShapesInfo		("Scene Information")
#define GUINewConfiguration	("New configuration")
#define GUILoadScene		("Load Shapes...")
#define GUILoadedScene		("") // empty for having more gui space
#define GUIShapesNumShapes	("# Shapes:\t")


// Effects
#define GUIEffectsTitle			("Effects Information")
#define GUIToggleFullScreen		("Full Screen")
#define GUIEffectsViewParams	("Show Animation Parameters")
#define GUIShowPlugins	("Installed plugins...")
#define GUIShapeTypesInfo "Installed shape types"
#define GUIEffectTypesInfo "Installed effect types"
#define GUIShowModules "Show Modules"
#define GUIModulesPanel "Modules"
#define GUIToggleConsole "Show Console Window"
#define GUIConsolePanel "Console"
