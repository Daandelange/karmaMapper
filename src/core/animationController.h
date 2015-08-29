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
#include "basicShape.h"
#include "effects.h"
#include "shapesDB.h"
#include "ofxUI.h"
#include "oscRouter.h"
#include "mirReceiver.h"
#include "durationReceiver.h"
#include "fboRecorder.h"

// todo: an overall mask that hides any unwanted projection zones

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
	void update(ofEventArgs& event);
	void draw(ofEventArgs& event);
	void guiEvent( ofxUIEventArgs &e);
	
protected:
	// gui
	ofxUISuperCanvas* gui;
	ofxUILabel* guiNumEffects;
	ofxUILabel* guiNumShapes;
	
	// effects stuff
	bool isEffectsIndex(int i);
	vector<basicEffect*> effects;
	
	shapesDB& scene;
	
	// music
	ofSoundPlayer music;
	
	
	// OSC
	//OSCRouter& oscRouter;
	mirReceiver mirOSCReceiver;
	durationReceiver durationOSCReceiver;
	
	// Video recording
	fboRecorder recorder;
	
private:
	bool bEnabled;
	
};


// GUI translations
#define GUIToggleFullScreen		("Full Screen")
#define GUIToggleMouseCursor	("Hide Mouse")

#define GUIEffectsNumEffects	("# Effects:\t")
#define GUIShapesNumShapes		("# Shapes:\t")
