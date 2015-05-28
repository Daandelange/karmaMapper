//
//  durationRC.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 10/05/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "durationRC.h"


// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
durationRC::durationRC( ){
	
}

durationRC::~durationRC(){
	
}

bool durationRC::setupOSC(){
	setup("localhost", 12346);
	bConnected = true;
	// (shuts down automatically)
}

// - - - - - - - -
// RC COMMANDS - Remote Control Duration
// - - - - - - - -
bool durationRC::rewindAndPlay(){
	if(!isConnected()) return false;
	
	bool ret = true;
	ret *= stop();
	
	ofxOscMessage m;
	m.setAddress("/duration/seektoposition");
	m.addFloatArg(0);
	sendMessage(m);
	
	ret *= play();
	
	return ret;
}

bool durationRC::play(){
	ofxOscMessage m;
	m.setAddress("/duration/play");
	sendMessage(m);
}

bool durationRC::stop(){
	ofxOscMessage m;
	m.setAddress("/duration/stop");
	sendMessage(m);
}

// GETTERS
bool durationRC::isConnected() const {
	return bConnected;
}

//mirData durationRC::mirCache;// = mirData();

