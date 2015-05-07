//
//  mirReceiver.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 06/05/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "mirReceiver.h"

// forward declarations are needed for event listening
ofEvent<mirTempoEventArgs> mirReceiver::mirTempoEvent;
ofEvent<mirOnSetEventArgs> mirReceiver::mirOnSetEvent;
ofEvent<mirBangEventArgs> mirReceiver::mirBangEvent;

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
mirReceiver::mirReceiver( ){
	
	//
	bEnabled = false;
}

mirReceiver::~mirReceiver(){
	stop();
}

// - - - - - - - -
// OSCNode PARENT FUNCTIONS
// - - - - - - - -

// tell router if a message can be handled by this OSCNode instance
bool mirReceiver::canHandle(const ofxOscMessage &_msg) const {
	return true;
}

// proceed the message
bool mirReceiver::handle(const ofxOscMessage &_msg) {
	if( !isEnabled() ) return false;
	
	// lock mutex
	//ofScopedLock myLock(oscMutex);
	
	string addr = _msg.getAddress();
	if( addr.compare("/aubioTempo") == 0 ){
		{
			// send bangEvent
			mirBangEventArgs args;
			args.what="aubioTempo";
			ofNotifyEvent(mirBangEvent, args);
		}
		{
			// send bangEvent
			mirTempoEventArgs args;
			args.bpm = mirCache.bpm;
			args.isTempoBis=false;
			ofNotifyEvent(mirTempoEvent, args);
		}
		return true;
	}
	else if( addr.compare("/aubioTempoBis") == 0 ){
		{
			// send bangEvent
			mirBangEventArgs args;
			args.what="aubioTempoBis";
			ofNotifyEvent(mirBangEvent, args);
		}
		{
			// send bangEvent
			mirTempoEventArgs args;
			args.bpm = mirCache.bpm;
			args.isTempoBis=true;
			ofNotifyEvent(mirTempoEvent, args);
		}
		return true;
	}
	else if( addr.compare("/aubioOnSet") == 0 ){
		{
			// send OnSetEvent
			mirOnSetEventArgs args;
			args.source="aubioOnSet";
			ofNotifyEvent(mirOnSetEvent, args);
		}
		return true;
	}
	else if( addr.compare("/aubioBpm") == 0 ){
		if(_msg.getNumArgs()>0) mirCache.bpm = _msg.getArgAsFloat(0);
		return true;
	}
	else if( addr.compare("/aubioPitch") == 0 ){
		if(_msg.getNumArgs()>0)	mirCache.pitch = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
		return true;
	}
	else if( addr.compare("/aubioQuiet") == 0 ){
		if(_msg.getNumArgs()>0) mirCache.silence = (_msg.getArgAsInt32(0)==0)?true:false;
		return true;
	}
	else if( addr.compare("/aubioZcr") == 0 ){
		if(_msg.getNumArgs()>0) mirCache.zcr = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
		return true;
	}
	else if( addr.compare("") == 0 ){
		
		return true;
	}
	else {
		//cout << "Not recognized OSC = " << addr << endl;
	}
	
	// unrecognized messge
	return false;
}

// informs that
void mirReceiver::youHaveToDie() {
	
	// detach events ?
}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
bool mirReceiver::start(){
	bEnabled = true;
	
	// bind events
	//ofAddListener(, mirReceiver:: *listener, )
	
	return isEnabled()==true;
}

bool mirReceiver::stop(){
	bEnabled = false;
	
	// unbind events
	
	return isEnabled()==false;
}

// - - - - - - - -
// EVENT LISTENERS
/*/ - - - - - - - -
void mirReceiver::oscIn(){
	// todo
}//*/

// GETTERS
bool mirReceiver::isEnabled() const {
	return bEnabled;
}

mirData mirReceiver::mirCache;// = mirData();

