//
//  musicEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

#include "musicEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

musicEffect::musicEffect(){
	// effect type must match with class
	effectType = "musicEffect";
}

// - - - - - - -
// GLOBAL EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
// This function is called before it starts rendering.
bool musicEffect::initialise(){
	basicEffect::initialise();
}

bool musicEffect::render(){
	renderVariables();
	return true;
}

// like ofApp::update()
void musicEffect::update(){
	
}

// resets all values
void musicEffect::reset(){
	
}

void musicEffect::renderVariables(){
	ofClear( ofColor(0, 128) );
	
	int height = 20;
	int posY = 0;
	ofFill();
	
	// draw mono volume
	posY += height;
	ofSetColor(255);
	ofDrawRectangle(0, posY, karmaSoundAnalyser::getInstance().getVolumeMono(true)*ofGetWidth(), height);
	ofSetColor(255, 100);
	ofDrawRectangle(0, posY, karmaSoundAnalyser::getInstance().getVolumeMono(false)*ofGetWidth(), height);
	ofDrawBitmapStringHighlight("volumeMono", 0, posY+15);
	
	
	
	// draw left volume
	posY += height;
	ofSetColor(255);
	ofDrawRectangle(0, posY, karmaSoundAnalyser::getInstance().getVolumeLeft(true)*ofGetWidth(), height);
	ofSetColor(255, 100);
	ofDrawBitmapStringHighlight("volumeLeft", 0, posY+15);
	ofDrawRectangle(0, posY, karmaSoundAnalyser::getInstance().getVolumeLeft(false)*ofGetWidth(), height);
	
	
	
	// draw right
	posY += height;
	ofSetColor(255);
	ofDrawRectangle(0, posY, karmaSoundAnalyser::getInstance().getVolumeRight(true)*ofGetWidth(), height);
	ofSetColor(255, 100);
	ofDrawBitmapStringHighlight("volumeRight", 0, posY+15);
	ofDrawRectangle(0, posY, karmaSoundAnalyser::getInstance().getVolumeRight(false)*ofGetWidth(), height);
	
	
	// draw balance
	posY += height;
	ofSetColor(255);
	ofDrawRectangle( ofGetWidth()/2, posY, (karmaSoundAnalyser::getInstance().getBalance(false)*ofGetWidth()-1)/2, height);
	ofSetColor(255, 100);
	ofDrawBitmapStringHighlight("balance", 0, posY+15);
	ofDrawRectangle( ofGetWidth()/2, posY, (karmaSoundAnalyser::getInstance().getBalance(true)*ofGetWidth()-1)/2, height);

	
	
	
}


