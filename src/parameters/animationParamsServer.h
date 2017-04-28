//
//  animationParamsServer.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/03/2015.
//
//	Provides and updates a bunch of parameters and variables like elapsed frames/time, seasons, cosinus, sinus, colors, etc

#pragma once

#include "ofMain.h"
//#include "ofxGui.h"
#include "ofxGuiExtended.h"
#include "animationParams.h"
//#include "animationControllerEvents.h"


#define KMAnimSeasonsYear 20 // (sec) year cycle duration

#define KM_AP_guiTextureWidth 200
#define KM_AP_guiTextureHeight 250
#define KM_AP_guiElementSpacing 3
#define KM_AP_guiElementHeight 12

// todo: let effects ask for needed "param" components and only then load them ?
// todo: add component system?

class animationParamsServer {

public:
	animationParamsServer();
	~animationParamsServer();
	
	// methods
	void randomize();
	void setShowParams(const bool& _b );
	
	// listeners
	void _update( ofEventArgs& e );
	void _draw( ofEventArgs& e );
	void _keypressed( ofKeyEventArgs& e );
	
	animationParams params;
	
	
private:
	bool bShowParams;
	ofFbo paramsGuiFbo;
	ofxGuiPanel paramsGui;
	
	ofRectangle varyingColorsRect;
	ofRectangle varyingColorsRectSec;
};
