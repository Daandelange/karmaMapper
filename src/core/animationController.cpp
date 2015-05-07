//
//  animationController.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/03/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "animationController.h"


// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
animationController::animationController( shapesServer& _scene, OSCRouter& _oscRouter ): scene(_scene), oscRouter(_oscRouter){
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
	
	// enable mirOSCReceiver
	mirOSCReceiver.start();
	oscRouter.addNode( &mirOSCReceiver );
	
	// get shape instance
	//string type="basicEffect";
	//e = stringToShapeClass( type );
	
	// re-enable when uncommenting 2 lines above
	/*if(e==NULL){
	 ofLogError("--> Could not create an effect of type "+type);
	 return;
	 }*/
	
	/*{
	 basicEffect* e;
	 e = new imageBeatEffect();
	 e->bindWithShape( server.getRandomShape() );
	 effects.push_back(e);
	}*/
	
	{
	basicShape* sh = scene.getRandomShape();
	basicEffect* e;
	e = new lineEffect();
	e->initialise();
	e->bindWithShape( scene.getRandomShape() );
	e->bindWithShape( scene.getRandomShape() );
	e->bindWithShape( scene.getRandomShape() );
	e->bindWithShape( scene.getRandomShape() );
	e->enable();
	effects.push_back(e);
	} // */
	
	/*{
	basicEffect* e;
	e = new imageMeltingEffect();
		e->initialise();
	e->bindWithShapes( scene.getShapesByGroup(3) );
	e->bindWithShapes( scene.getShapesByGroup(1) );
	e->enable(); // should in fact be called once basicShape::isReady();
	effects.push_back(e);
	}// */
	
	/*{
	basicEffect* e;
	e = new imageGrainEffect();
	e->initialise();
	e->bindWithShapes( scene.getShapesByGroup(1) );
	e->enable(); // should in fact be called once basicShape::isReady();
	effects.push_back(e);
	} // */
	
	/*e = new MeshRenderer3D();
	 e->bindWithShape( server.getRandomShape() );
	 effects.push_back(e);*/
	
	return isEnabled()==true;
}

bool animationController::stop(){
	
	// disable mirOSCReceiver
	mirOSCReceiver.stop();
	oscRouter.removeNode( &mirOSCReceiver );
	
	gui->disable();
	
	// delete effect pointers
	for(int i=0; i<effects.size();++i) delete effects[i];
	
	effects.clear();
	effects.resize(0);
	
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
	
	// draw effects
	for(int i=0; i<effects.size(); i++){
		effects[i]->render();
	}
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
