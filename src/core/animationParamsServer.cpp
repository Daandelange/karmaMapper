//
//  animationParamsServer.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/03/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "animationParamsServer.h"
#include <cmath>     // for fmod
//#include <functional> // for std::modulus

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -

animationParamsServer::animationParamsServer(){
	bShowParams = false;
	randomizeUIDs();
	paramsGui.setup("Animation Parameters");
	//paramsGui.add( new ofxGuiSpacer() );
	paramsGuiFbo.allocate(KM_AP_guiTextureWidth, KM_AP_guiTextureHeight, GL_RGBA);
	paramsGuiFbo.begin();
	ofClear(255);
	paramsGuiFbo.end();
	paramsGui.add( new ofxGuiBaseDraws( &paramsGuiFbo.getTexture() ) );
	paramsGui.unregisterMouseEvents();
	
	// bind
	ofAddListener( ofEvents().draw , this, &animationParamsServer::_draw, OF_EVENT_ORDER_AFTER_APP );
	ofAddListener( ofEvents().update , this, &animationParamsServer::_update, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().keyPressed , this, &::animationParamsServer::_keypressed, OF_EVENT_ORDER_APP );
}

animationParamsServer::~animationParamsServer(){
	
	// unbind
	ofRemoveListener( ofEvents().draw , this, &animationParamsServer::_draw );
	ofRemoveListener( ofEvents().update , this, &animationParamsServer::_update );
	ofRemoveListener( ofEvents().keyPressed , this, &::animationParamsServer::_keypressed );
}

// METHODS
// - - - - -

void animationParamsServer::randomizeUIDs(){
	// todo: make this better (HEX string?)
	string time = ofToString( ofGetUnixTime() );
	std::reverse(time.begin(), time.end());
	
	std::stringstream ss;
	int prevNum = 4;
	for(auto it=time.begin(); it!=time.end(); ++it){
		// check for only alphanum (+ some exceptions) chars
		// explanation: values from 2-53 are numbers and ponctuations with the exception of 14,15,28,29 and 43
		// 0x20-0x7e
		int otherNum = round( ((*it) * prevNum ) % (-0x20+0x7e));
		otherNum += 0x20; // skip to 0x20
		//if( otherNum > 0x39 ) otherNum+=(-0x39+0x41); // skip 14 & 15
		//if( otherNum > 0x5a ) otherNum+=(-0x39+0x41); // skip 28 & 42
		//if( otherNum >= 14 ) otherNum+=1; // skip 43
		ss << std::hex << (char) otherNum;
		prevNum = *it;
	}
	params.uniqueID = ss.str();
}

void animationParamsServer::setShowParams(const bool &_b){
	if( _b != bShowParams ){
		// enable
		if(_b){
			paramsGui.registerMouseEvents();
		}
		// disable
		else {
			paramsGui.unregisterMouseEvents();
		}
		bShowParams = _b;
	}
}

//
// LISTENERS
// - - - - - -

// todo: generate unique ID
void animationParamsServer::_update(ofEventArgs &e){
	params.elapsedTime = ofGetElapsedTimef();
	
	// set seasons
	float seasonValue = fmod( params.elapsedTime, KMAnimSeasonsYear) ;// ((int)KMAnimSeasonsYear);
	params.seasons.winter = (sin( seasonValue/KMAnimSeasonsYear*TWO_PI )+1)/2;
	seasonValue+=KMAnimSeasonsYear/4;
	params.seasons.spring = (sin( seasonValue/KMAnimSeasonsYear*TWO_PI + PI/2 )+1)/2;
	seasonValue+=KMAnimSeasonsYear/4;
	params.seasons.summer = (sin( seasonValue/KMAnimSeasonsYear*TWO_PI + PI )+1)/2;
	seasonValue+=KMAnimSeasonsYear/4;
	params.seasons.autumn = (sin( seasonValue/KMAnimSeasonsYear*TWO_PI + (3*PI)/2 )+1)/2;
	seasonValue+=KMAnimSeasonsYear/4;
}

void animationParamsServer::_draw(ofEventArgs &e){
	params.elapsedFrames++;
	
	if( bShowParams ){
		int height = 5;
#define MULTIPLY(val1, val2)  ((val1) * (val2))
#define ITEM_width MULTIPLY(KM_AP_guiTextureWidth, 0.9f)
#define ITEM_spacing_x MULTIPLY(KM_AP_guiTextureWidth, 0.05f)
		// (KM_AP_guiTextureWidth - item_width)/2
		
		paramsGuiFbo.begin();
		ofClear(255,255);
		ofSetColor(0,255);
		ofFill();
		
		ofDrawBitmapString("elapsedUpdates: "+ ofToString(params.elapsedUpdates), ITEM_spacing_x, height+KM_AP_guiElementHeight/1.2);
		height += (KM_AP_guiElementHeight + KM_AP_guiElementSpacing);
		
		ofDrawBitmapString("elapsedFrames: "+ ofToString(params.elapsedFrames), ITEM_spacing_x, height+KM_AP_guiElementHeight/1.2);
		height += (KM_AP_guiElementHeight + KM_AP_guiElementSpacing);
		
		ofDrawBitmapString("elapsedTime: "+ ofToString(params.elapsedTime), ITEM_spacing_x, height+KM_AP_guiElementHeight/1.2);
		height += (KM_AP_guiElementHeight + KM_AP_guiElementSpacing);
		
		ofDrawBitmapString("uniqueID: "+ ofToString(params.uniqueID), ITEM_spacing_x, height+KM_AP_guiElementHeight/1.2);
		height += (KM_AP_guiElementHeight + KM_AP_guiElementSpacing);
		
		ofDrawBitmapString("uniqueIDAlt: "+ ofToString(params.uniqueIDAlt), ITEM_spacing_x, height+KM_AP_guiElementHeight/1.2);
		height += (KM_AP_guiElementHeight + KM_AP_guiElementSpacing);
		
		ofDrawBitmapString("userChain: "+ ofToString(params.userChain), ITEM_spacing_x, height+KM_AP_guiElementHeight/1.2);
		height += (KM_AP_guiElementHeight + KM_AP_guiElementSpacing);
		
		// seasons
		ofDrawRectangle(ITEM_spacing_x, height, ITEM_width*params.seasons.winter, KM_AP_guiElementHeight);
		ofSetColor(200);
		ofDrawBitmapString("Winter: ", ITEM_spacing_x+3, height+KM_AP_guiElementHeight/1.2);
		ofSetColor(0);
		height += KM_AP_guiElementHeight + KM_AP_guiElementSpacing;
		
		ofDrawRectangle(ITEM_spacing_x, height, ITEM_width*params.seasons.spring, KM_AP_guiElementHeight);
		ofSetColor(200);
		ofDrawBitmapString("Spring: ", ITEM_spacing_x+3, height+KM_AP_guiElementHeight/1.2);
		ofSetColor(0);
		height += KM_AP_guiElementHeight + KM_AP_guiElementSpacing;
		
		ofDrawRectangle(ITEM_spacing_x, height, ITEM_width*params.seasons.summer, KM_AP_guiElementHeight);
		ofSetColor(200);
		ofDrawBitmapString("Summer: ", ITEM_spacing_x+3, height+KM_AP_guiElementHeight/1.2);
		ofSetColor(0);
		height += KM_AP_guiElementHeight + KM_AP_guiElementSpacing;
		
		ofDrawRectangle(ITEM_spacing_x, height, ITEM_width*params.seasons.autumn, KM_AP_guiElementHeight);
		ofSetColor(200);
		ofDrawBitmapString("Autumn: ", ITEM_spacing_x+3, height+KM_AP_guiElementHeight/1.2);
		ofSetColor(0);
		height += KM_AP_guiElementHeight + KM_AP_guiElementSpacing;
		
		//ofDrawRectangle(ITEM_spacing_x, height, ITEM_width, KM_AP_guiElementHeight);
		//height += KM_AP_guiElementHeight + KM_AP_guiElementSpacing;
		
		ofNoFill();
		paramsGuiFbo.end();
		
		paramsGui.draw();
	}
}

void animationParamsServer::_keypressed(ofKeyEventArgs &e){
	
	// grow user string
	if( isalnum( (char) e.key ) ){
		params.userChain += (char) e.key;
		// limit to 10 lst chars
		if(params.userChain.length()>10){
			params.userChain = std::string(params.userChain.end()-10 , params.userChain.end() );
		}
	}
}
