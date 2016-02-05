//
//  durationReceiver.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 10/05/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "durationReceiver.h"

// forward declarations are needed for event listening
ofEvent<durationBangEventArgs> durationReceiver::durationBangEvent;
ofEvent<durationFlagEventArgs> durationReceiver::durationFlagEvent;
ofEvent<durationFloatEventArgs> durationReceiver::durationFloatEvent;

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
durationReceiver::durationReceiver( ){
	
	//
	bEnabled = false;
}

durationReceiver::~durationReceiver(){
	stop();
}

// - - - - - - - -
// OSCNode PARENT FUNCTIONS
// - - - - - - - -

// tell router if a message can be handled by this OSCNode instance
bool durationReceiver::canHandle(const ofxOscMessage &_msg) const {
	return (_msg.getAddress().compare(0,DT_PREFIX_LEN,DURATION_TIMELINE_PREFIX) == 0);
}

// proceed the message
bool durationReceiver::handle(const ofxOscMessage &_msg) {
	if( !isEnabled() ) return false;
	
	// lock mutex
	ofScopedLock myLock(oscMutex);
	
	// get incoming track info
	if( _msg.getAddress().compare("/duration/info") == 0 ){
		
		// todo: register incomming variable types // tracks and let them be synchronised automatically with inter variables + getters
		
		return true; // we're done
	}
	
	// incoming track data
	if( canHandle(_msg) ){
		string track=_msg.getAddress().substr(DT_PREFIX_LEN,_msg.getAddress().npos);
		
		ofxOscArgType type = OFXOSC_TYPE_NONE;
		if( _msg.getNumArgs() > 0 ) type = _msg.getArgType(0);
		
		//	- - - - - - - - - -
		//	Notice:
		//	The following message handling are temporary and static.
		//	A system needs to be made for synchronising any track by automatically parsing it. #ofxOscVariable
		//	Hence you can dynamically create new variables and bind them to effect variables
		//	- - - - - - - - - -
		
		if( track.compare("bang") == 0 && type == OFXOSC_TYPE_NONE ){
			{ // send bangEvent
				durationBangEventArgs args;
				args.track=track;
				ofNotifyEvent(durationBangEvent, args);
			}
		}
		else if( track.compare("test") == 0 ){
			{ // send flagEvent
				string flag = "";
				if(_msg.getNumArgs()>0 && type == OFXOSC_TYPE_STRING){
					flag=_msg.getArgAsString(0);
				}
				durationFlagEventArgs args;
				args.track=track;
				args.flag="";
				
				ofNotifyEvent(durationFlagEvent, args);
			}
		}
		else if( track.compare("lineEffectIntensity") == 0 ){
			{ // curve
				float value = 0;
				if(_msg.getNumArgs()>0)
					value=_msg.getArgAsFloat(0);
				durationFloatEventArgs args;
				args.track=track;
				args.type="curve";
				args.value=value;
				ofNotifyEvent(durationFloatEvent, args);
			}
		}
		// tmp
		else if( track.compare("bgVendomeOpacity") == 0 ){
			{ // curve
				float value = 0;
				if(_msg.getNumArgs()>0)
					value=_msg.getArgAsFloat(0);
				durationFloatEventArgs args;
				args.track=track;
				args.type="curve";
				args.value=value;
				ofNotifyEvent(durationFloatEvent, args);
			}
		}
		
		else if( track.compare("imageBeatEffectIntensity") == 0 ){
			{ // curve
				float value = 0;
				if(_msg.getNumArgs()>0) value=_msg.getArgAsFloat(0);
				durationFloatEventArgs args;
				args.track=track;
				args.type="curve";
				args.value=value;
				ofNotifyEvent(durationFloatEvent, args);
			}
		}
		/*{
			// send bangEvent
			durationBangEventArgs args;
			args.what="aubioTempo";
			ofNotifyEvent(durationBangEvent, args);
		}
		{
			// send bangEvent
			durationTempoEventArgs args;
			args.bpm = mirCache.bpm;
			args.isTempoBis=false;
			ofNotifyEvent(durationTempoEventArgs, args);
		}
		 */
		// unknown track, could not get information
		else {
			// todo: try to add the signal here
			ofLogNotice();
		}
		return true;
	}/*
	else if( addr.compare("/aubioTempoBis") == 0 ){
		{
			// send bangEvent
			durationBangEventArgs args;
			args.what="aubioTempoBis";
			ofNotifyEvent(durationBangEvent, args);
		}
		{
			// send bangEvent
			durationTempoEventArgs args;
			args.bpm = mirCache.bpm;
			args.isTempoBis=true;
			ofNotifyEvent(durationTempoEventArgs, args);
		}
		return true;
	}
	else if( addr.compare("/aubioBpm") == 0 ){
		if(_msg.getNumArgs()>0) mirCache.bpm = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
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
	else if( addr.compare("/aubioIsPlaying") == 0 ){
		if(_msg.getNumArgs()>0) mirCache.isPlaying = (_msg.getArgAsInt32(0)==0)?true:false;
		return true;
	}
	else if( addr.compare("/aubioZcr") == 0 ){
		if(_msg.getNumArgs()>0) mirCache.zcr = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
		return true;
	}
	else if( addr.compare("/balance") == 0 ){
		if(_msg.getNumArgs()>0) mirCache.balance = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
		return true;
	}
	// Fiddle FFT Equaliser
	else if( addr.compare(0, 16, "/fiddle_fft/band") == 0 ){ // 16 first chars
		int band = ofToInt(addr.substr(16));
		
		if(addr.compare(17, 20, "rms")==0) // rms (volume)
			mirCache.fiddleFFT[band][0]=_msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
		else // pitch
			mirCache.fiddleFFT[band][1]=_msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
		
		return true;
	}
	else if( addr.compare("") == 0 ){
		
		return true;
	}*/
	else {
		//cout << "Not recognized OSC = " << _msg.getAddress() << endl;
	}
	
	// unrecognized messge
	return false;
}

// informs that the nose id gonna be unbound with the node server
void durationReceiver::detachNode() {
	stop();
	// detach events ?
}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
bool durationReceiver::start(){
	bEnabled = true;
	
	// bind events
	//ofAddListener(, durationReceiver:: *listener, )
	
	return isEnabled()==true;
}

bool durationReceiver::stop(){
	bEnabled = false;
	
	// unbind events
	
	return isEnabled()==false;
}

// - - - - - - - -
// EVENT LISTENERS
/*/ - - - - - - - -
void durationReceiver::oscIn(){
	// todo
}//*/

// GETTERS
bool durationReceiver::isEnabled() const {
	return bEnabled;
}
