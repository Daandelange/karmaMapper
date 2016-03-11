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
	randomize();
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

void animationParamsServer::randomize(){
	
	// randomizeUIDs
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
	
	// randomize colors
	params.varyingColors.main = ofFloatColor::fromHsb(ofRandom(0.5f,.8f), ofRandom(0.5f,.7f), ofRandom(0.5f,.8f) );
	params.varyingColors.secondary = params.varyingColors.main;
	params.varyingColors.secondary.setHue( fmod(params.varyingColors.main.getHue()-0.3f, 1));
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

// update the params
void animationParamsServer::_update(ofEventArgs &e){
	
	params.fps = ofGetFrameRate();
	params.elapsedTime = ofGetElapsedTimef();
	
	// set seasons
	// formulaes used: (synchronized)
	// cos(x)+cos(2*x)
	// 1+cos(3*x)
	float seasonValue = fmod( params.elapsedTime, KMAnimSeasonsYear)/KMAnimSeasonsYear;// ((int)KMAnimSeasonsYear);
	
	if( cos(seasonValue*TWO_PI)+cos(2*seasonValue*TWO_PI) < 0 ) params.seasons.winter = 0;
	else params.seasons.winter = ofClamp( (cos( seasonValue*TWO_PI*3)+1)/2, 0,1);
	//float other = cos(seasonValue*TWO_PI)+cos(2*seasonValue*TWO_PI);
	//cout << ((cos( seasonValue*TWO_PI*3)+1)/2) << " \t \t " << other << ((other > 0)?" OK OK OK":"") << endl;
	
	seasonValue = fmod(seasonValue+0.25f,1);
	if( cos(seasonValue*TWO_PI)+cos(2*seasonValue*TWO_PI) < 0 ) params.seasons.autumn = 0;
	else params.seasons.autumn = ofClamp( (cos( seasonValue*TWO_PI*3)+1)/2, 0,1);
	
	seasonValue = fmod(seasonValue+0.25f,1);
	if( cos(seasonValue*TWO_PI)+cos(2*seasonValue*TWO_PI) < 0 ) params.seasons.summer = 0;
	else params.seasons.summer = ofClamp( (cos( seasonValue*TWO_PI*3)+1)/2, 0,1);
	
	seasonValue = fmod(seasonValue+0.25f,1);
	if( cos(seasonValue*TWO_PI)+cos(2*seasonValue*TWO_PI) < 0 ) params.seasons.spring = 0;
	else params.seasons.spring = ofClamp( (cos( seasonValue*TWO_PI*3)+1)/2, 0,1);
	
	// change colors
	// todo: make this more interesting
	params.varyingColors.main.setHue( fmod(params.varyingColors.main.getHue()+ (0.0005f+params.seasons.summer*0.003f)*(ofGetFrameRate()/60), 1) );
	params.varyingColors.secondary.setHue( fmod(params.varyingColors.main.getHue()+0.3f,1) );
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
		
		ofFill();
		
		// numbers
		ofSetColor(0,255);
		ofDrawBitmapString("FPS: "+ ofToString(params.fps), ITEM_spacing_x, height+KM_AP_guiElementHeight/1.2);
		height += (KM_AP_guiElementHeight + KM_AP_guiElementSpacing);
		
		ofSetColor(0,255);
		ofDrawBitmapString("elapsedUpdates: "+ ofToString(params.elapsedUpdates), ITEM_spacing_x, height+KM_AP_guiElementHeight/1.2);
		height += (KM_AP_guiElementHeight + KM_AP_guiElementSpacing);
		
		ofDrawBitmapString("elapsedFrames: "+ ofToString(params.elapsedFrames), ITEM_spacing_x, height+KM_AP_guiElementHeight/1.2);
		height += (KM_AP_guiElementHeight + KM_AP_guiElementSpacing);
		
		ofDrawBitmapString("elapsedTime: "+ ofToString(params.elapsedTime), ITEM_spacing_x, height+KM_AP_guiElementHeight/1.2);
		height += (KM_AP_guiElementHeight + KM_AP_guiElementSpacing);
		
		ofDrawBitmapString("idleTimeMillis: "+ ofToString(params.idleTimeMillis), ITEM_spacing_x, height+KM_AP_guiElementHeight/1.2);
		height += (KM_AP_guiElementHeight + KM_AP_guiElementSpacing);
		
		// strings
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
		
		// display colors
		ofSetColor(params.varyingColors.main);
		varyingColorsRect.set(ITEM_spacing_x, height, ITEM_width/2-ITEM_spacing_x/2, KM_AP_guiElementHeight);
		ofDrawRectangle( varyingColorsRect );
		ofSetColor(params.varyingColors.secondary);
		varyingColorsRectSec.set(ITEM_width/2+ITEM_spacing_x/2, height, ITEM_width/2-ITEM_spacing_x/2, KM_AP_guiElementHeight);
		ofDrawRectangle( varyingColorsRectSec );
		ofSetColor(0);
		ofDrawBitmapString("varyingColors", ITEM_spacing_x+3, height+KM_AP_guiElementHeight/1.2);
		height += KM_AP_guiElementHeight + KM_AP_guiElementSpacing;
		
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
