//
//  animationParamsServer.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/03/2015.
//
//	Provides and updates a bunch of parameters and variables like elapsed frames/time, seasons, cosinus, sinus, colors, etc

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxGuiExtended.h"

#define KMAnimSeasonsYear 10 // (sec) year cycle duration

#define KM_AP_guiTextureWidth 200
#define KM_AP_guiTextureHeight 250
#define KM_AP_guiElementSpacing 3
#define KM_AP_guiElementHeight 12

struct seasonsYear{
	seasonsYear(): winter(0), spring(0), summer(0), autumn(0){};
	
	float winter, spring, summer, autumn;
};

struct colorSet {
		colorSet() :
		mainColor(ofColor(0,0,0,255)),
		secondaryColor(ofColor(0,0,0,255)),
		altColor(ofColor(0,0,0,255))
	{};
	
	ofColor mainColor, secondaryColor, altColor;
};

struct animationParams {
	
	animationParams():
		elapsedUpdates(0),
		elapsedFrames(0),
		elapsedTime(0.f),
		uniqueID(""),
		uniqueIDAlt(""),
		userChain("")
	{};
	
	unsigned int elapsedUpdates;
	unsigned int elapsedFrames;
	float elapsedTime;
	
	// used for customising effects and synchronising their
	string uniqueID; // hex string
	string uniqueIDAlt;
		
	// chain typed by user // keylogger
	string userChain;
	
	seasonsYear seasons;
	colorSet colors;
	
};

// todo: let effects ask for needed "param" components and only then load them ?
// todo: add component system?

class animationParamsServer {

public:
	animationParamsServer();
	~animationParamsServer();
	
	// methods
	void randomizeUIDs();
	void setShowParams(const bool& _b );
	
	// listeners
	void _update( ofEventArgs& e );
	void _draw( ofEventArgs& e );
	void _keypressed( ofKeyEventArgs& e );
	
	animationParams params;
	
	
private:
	bool bShowParams;
	ofFbo paramsGuiFbo;
	ofxPanelExtended paramsGui;
};