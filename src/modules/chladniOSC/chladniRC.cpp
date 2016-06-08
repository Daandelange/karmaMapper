//
//  chladniRC.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 05/06/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "chladniRC.h"


// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
chladniRC::chladniRC( ){
	setupOSC();
}

chladniRC::~chladniRC(){
	setup("",-1);
	disableBroadcast();
}

bool chladniRC::setupOSC(){
	setup("localhost", 12002);
	bConnected = true;
	// (shuts down automatically)
	return bConnected;
}

// - - - - - - - -
// RC COMMANDS - Remote Control chladni / cymatics generator (pd)
// - - - - - - - -
bool chladniRC::sendBang(const string& _name) {
	if(!bConnected){
		return false;
	}
	
	ofxOscMessage m;
	m.setAddress("/bang");
	m.addStringArg(_name);
	sendMessage(m);
	return true;
}

bool chladniRC::sendFloat(const string& _name, const float& _float){
	if(!bConnected){
		return false;
	}
	
	ofxOscMessage m;
	m.setAddress(_name);
	m.addFloatArg(_float);
	sendMessage(m);
	return true;
}

bool chladniRC::sendPing(){
	if(!bConnected){
		return false;
	}
	
	ofxOscMessage m;
	m.setAddress("/ping");
	m.addImpulseArg();
	sendMessage(m);
	return true;
}


// GETTERS
bool chladniRC::isConnected() const {
	return bConnected;
}

//mirData chladniRC::mirCache;// = mirData();

