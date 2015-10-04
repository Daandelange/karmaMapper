//
//  animationController.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/03/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "animationController.h"
#include "ofxVLCRemote.h"
#include "durationRC.h"

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
animationController::animationController( shapesDB& _scene ): scene(_scene){
	bEnabled = false;
	effects.clear();
	effects.resize(0);
	
	ofAddListener( ofEvents().draw , this, &animationController::draw );
	ofAddListener( ofEvents().update , this, &animationController::update );
	
	// build GUI
	gui = new ofxUISuperCanvas("ANIMATION CONTROLLER");
	gui->setColorBack(ofColor(41,123,117,180));
	gui->setColorFill(ofColor(255,160));
	gui->setColorFillHighlight(ofColor(255,220));
	//gui->setColorPadded(ofColor(255,150));
	
	//gui->setPosition(pos.x, pos.y);
	
	gui->addSpacer();
	string textString = "Control'em'all! ^_^";
	gui->addTextArea("textarea", textString, OFX_UI_FONT_SMALL);
	gui->addSpacer();
	textString = "Keyboard shortcuts:";
	gui->addTextArea("textarea", textString, OFX_UI_FONT_SMALL);
	textString = "  H Show/Hide Pointer";
	gui->addTextArea("textarea", textString, OFX_UI_FONT_SMALL);
	textString = "  F Toggle full screen";
	gui->addTextArea("textarea", textString, OFX_UI_FONT_SMALL);
	
	/*gui->addSpacer();
	gui->addToggle(GUIToggleFullScreen, false);
	gui->addToggle(GUIToggleMouseCursor, false);
	gui->addFPSSlider("FPS"); */
	
	gui->addSpacer();
	gui->addLabel("Scene", OFX_UI_FONT_LARGE);
	guiNumEffects = gui->addLabel(GUIEffectsNumEffects, OFX_UI_FONT_SMALL);
	guiNumShapes = gui->addLabel(GUIShapesNumShapes, OFX_UI_FONT_SMALL);
	
	gui->autoSizeToFitWidgets();
	gui->disable();
	ofAddListener(gui->newGUIEvent, this, &animationController::guiEvent);
}

animationController::~animationController(){
	ofRemoveListener( ofEvents().draw , this, &animationController::draw );
	ofRemoveListener( ofEvents().update , this, &animationController::update );
	
	// rm GUI stuff correctly
	ofRemoveListener( gui->newGUIEvent, this, &animationController::guiEvent );
	gui->disable();
	guiNumEffects = NULL;
	guiNumShapes = NULL;
	delete gui;
}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
bool animationController::start(){
	bEnabled = true;
	gui->enable();
	
	// all black! xD
	ofSetBackgroundAuto(false);
	ofClear(0,0,0,255);
	
	// play music
	//sound.loadSound("TEST MIX V0.1.wav");
	//music.load("music.wav");
	//music.setLoop(true);
	//music.play();
	
	// Setup video recording
	//recorder.startRecording("", 1500, 1200);
	
	{
		// play music with VLC
		ofxVLCRemote vlc;
		vlc.setup("/Users/daan/vlc.sock"); // client-specific... :(
		vlc.run("repeat on");
		vlc.run("normal"); // playback speed
		vlc.run("volume 256"); // 0-1024 (256=normal)
		
		//vlc.run("adev 7"); // tell VLC to use device 7 (soundflower 2ch)
		vlc.run("play");
		vlc.run("prev");
		//vlc.run("got0 0");
		
	}
	
	{	// SYNC DURATION
		durationRC rc;
		rc.setupOSC();
		rc.rewindAndPlay();
	}
	
	
	// enable mirOSCReceiver
	//mirOSCReceiver.start();
	//oscRouter.addNode( &mirOSCReceiver );
	
	// enable durationOSCReceiver
	//durationOSCReceiver.start();
	//oscRouter.addNode( &durationOSCReceiver );
	
	// get shape instance
	//string type="basicEffect";
	//e = stringToShapeClass( type );
	
	// re-enable when uncommenting 2 lines above
	/*if(e==NULL){
	 ofLogError("--> Could not create an effect of type "+type);
	 return;
	 }*/
	
	/*
	if(false){	// VIDEO EFFECT
		basicEffect* e;
		e = new videoEffect();
		e->initialise();
		e->bindWithShape( scene.getRandomShapeByGroup(5) ); // 3 = bg
		effects.push_back(e);
	}
	
	//
	if(true){	// IMAGE EFFECT
		basicEffect* e;
		e = new imageEffect();
		e->initialise();
		((imageEffect*) e)->loadImageFromFile("vendome_full_ok.jpg");
		((imageEffect*) e)->listenToOscIntensity("bgVendomeOpacity");
		e->bindWithShape( scene.getRandomShapeByGroup(5) ); // 3 = blue windows
		
		effects.push_back(e);
	}
	
	// no mutexes --> unstable
	if(true){	// IMAGE BEAT EFFECT
		basicEffect* e;
		e = new imageBeatEffect();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(3) ); // windows
		e->enable();
		effects.push_back(e);
	}
	
	if(false){	// BASIC EFFECT
		basicEffect* e;
		e = new basicEffect();
		e->initialise();
		e->bindWithShapes( scene.getAllShapes() );
		e->enable();
		effects.push_back(e);
	}
	
	if(false){	// LINE EFFECT
		basicEffect* e;
		e = new lineEffect();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(3) ); // windows
		e->bindWithShapes( scene.getShapesByGroup(0) ); // columns
		e->enable();
		effects.push_back(e);
	}
	
	/ *{
	basicEffect* e;
	e = new imageMeltingEffect();
	e->initialise();
	e->bindWithShapes( scene.getShapesByGroup(3) );
	e->bindWithShapes( scene.getShapesByGroup(1) );
	effects.push_back(e);
	}// */
	
	/*{
	basicEffect* e;
	e = new imageGrainEffect();
	e->initialise();
	e->bindWithShapes( scene.getShapesByGroup(1) );
	effects.push_back(e);
	} // */
	
	/*{ // MESH 3D
		basicEffect* e;
		e = new meshRenderer();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(1) );
		effects.push_back(e);
	} // */
	
	/*{ // GIF effect
		basicEffect* e;
		e = new gifEffect();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(1) );
		effects.push_back(e);
	}// */
	
	return isEnabled()==true;
}

bool animationController::stop(){
	
	// disable mirOSCReceiver
	//mirOSCReceiver.stop();
	//oscRouter.removeNode( &mirOSCReceiver );
	
	// disable durationOSCReceiver
	//durationOSCReceiver.stop();
	//oscRouter.removeNode( &durationOSCReceiver );
	
	gui->disable();
	
	{
		// play music with VLC
		ofxVLCRemote vlc;
		vlc.setup("/Users/daan/vlc.sock"); // client-specific... :(
		vlc.run("stop");
	}
	
	{	// SYNC DURATION
		durationRC rc;
		rc.setupOSC();
		rc.stop();
	}
	
	// delete effect pointers
	// todo: this can make it crash...
	for(int i=effects.size()-1;i>=0;i--){
		delete effects[i];
	}
	
	effects.clear();
	effects.resize(0);
	
	// save recorded file ?
	recorder.stopRecording();
	
	bEnabled = false;
	return isEnabled()==false;
}

bool animationController::isEnabled() const {
	return bEnabled;
}

// - - - - - - - -
// EVENT LISTENERS
// - - - - - - - -
void animationController::update(ofEventArgs &event){
	if(!isEnabled()) return;
	
	// update GUI data
	if(guiNumEffects!=NULL) guiNumEffects->setLabel(GUIEffectsNumEffects + ofToString(effects.size()) );
	if(guiNumShapes!=NULL) guiNumShapes->setLabel(GUIShapesNumShapes + ofToString( scene.getNumShapes() ) );
	
	// create animation state (bunch of variables)
	
	
	// update effects (run mode)
	for(int i=0; i<effects.size(); i++){
		effects[i]->update();
	}
}

void animationController::draw(ofEventArgs& event){
	if(!isEnabled()) return;
	
	//ofScale(0.5, 0.5); // tmp
	
	recorder.beginFrame();
	
	// tmp
	ofClear(0,1);
	
	// draw effects
	for(int i=0; i<effects.size(); i++){
		effects[i]->render();
	}
	
	recorder.endFrame(true);
}

void animationController::guiEvent(ofxUIEventArgs &e){
	string name = e.getName();
	
	// scan for open dropdowns
	// (dirty) bypass for issue https://github.com/rezaali/ofxUI/issues/68
	//ofxUIDropDownList* addShapeDDL = (ofxUIDropDownList*) gui->getWidget("Add Shape");
	//bool dropdownOpen = addShapeDDL->isOpen();
	//if( selectConfigFileOpen && name != "Configuration File") return;
	
	
	// interface options
	// - - - - - - - - -
	if(name==GUIToggleFullScreen){
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		ofSetFullscreen(toggle->getValue());
	}
	else if(name==GUIToggleMouseCursor){
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		toggle->getValue()?ofHideCursor():ofShowCursor();
	}
	
	#ifdef KARMAMAPPER__DEBUG
	else {
		ofLogNotice("animationController::guiEvent") << "Unknown GUI event: " << name << endl;
	}
	#endif
}
