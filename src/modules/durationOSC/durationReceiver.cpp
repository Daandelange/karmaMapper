//
//  durationReceiver.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 10/05/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "durationReceiver.h"
#include "basicEffect.h"

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
	
	moduleName = "durationReceiver";
	moduleType = "durationReceiver";
}

durationReceiver::~durationReceiver(){
	stop();
}

// - - - - - - - -
// OSCNode PARENT FUNCTIONS
// - - - - - - - -

// tell router if a message can be handled by this OSCNode instance
bool durationReceiver::canHandle(const ofxOscMessage &_msg) const {
	//string tmp = _msg.getAddress().substr(0,DT_PREFIX_LEN)+" • " + ofToString(DT_PREFIX_LEN);
	return (_msg.getAddress().compare(0,DT_PREFIX_LEN, DURATION_TIMELINE_PREFIX) == 0);
}

// proceed the message
bool durationReceiver::handle(const ofxOscMessage &_msg) {
	if( !isEnabled() ) return false;
	
	// lock mutex
	ofScopedLock myLock(oscMutex);
	
	// get incoming track info
	if( _msg.getAddress().compare("/dt/info") == 0 ){
		
		// todo: register incomming variable types // tracks and let them be synchronised automatically with inter variables + getters
		
		return true; // we're done
	}
	
	// incoming track data
	if( canHandle(_msg) ){
		string track=_msg.getAddress().substr(DT_PREFIX_LEN,_msg.getAddress().npos);
		
		ofxOscArgType type = OFXOSC_TYPE_NONE;
		if( _msg.getNumArgs() > 0 ) type = _msg.getArgType(0);
		
		if(_msg.getNumArgs()>0 && (type == OFXOSC_TYPE_INT32 || type == OFXOSC_TYPE_FLOAT) ){
			// /dt/cmd/effect-*
			#define cmpEffectCmd "cmd/effect-"
			if( track.compare(0, sizeof(cmpEffectCmd)-1, cmpEffectCmd) == 0  ) {
				
				track=track.substr(sizeof(cmpEffectCmd)-1,track.npos);
				
				// enable / disable an effect
				// /dt/cmd/effect-enable-effectName
				#define cmpEffectCmdEnable "enable-"
				if( track.compare(0, sizeof(cmpEffectCmdEnable)-1, cmpEffectCmdEnable) == 0  ) {
					
					effectCmdEventArgs args;
					args.command = "enable";
					args.targetEffectName = track.substr(sizeof(cmpEffectCmdEnable)-1, track.npos);
					args.boolValue = _msg.getArgAsInt32(0);
					
					ofNotifyEvent(basicEffect::effectCommandEvent, args);
				}
				#define cmpEffectCmdAlpha "alpha-"
				else if( track.compare(0, sizeof(cmpEffectCmdAlpha)-1, cmpEffectCmdAlpha) == 0  ) {
					
					effectCmdEventArgs args;
					args.command = "alpha";
					args.targetEffectName = track.substr(sizeof(cmpEffectCmdAlpha)-1, track.npos);
					args.floatValue = _msg.getArgAsFloat(0);
					
					ofNotifyEvent(basicEffect::effectCommandEvent, args);
				}
			}
		}
        
        else if( type == OFXOSC_TYPE_FLOAT){
            float value = 0;
            if(_msg.getNumArgs()>0) value=_msg.getArgAsFloat(0);
            durationFloatEventArgs args;
            args.track=track;
            //args.type="curve";
            args.value=value;
            ofNotifyEvent(durationFloatEvent, args);
        }
        else if(type == OFXOSC_TYPE_INT32){
            
        }
        else if(type == OFXOSC_TYPE_STRING){
            durationFlagEventArgs args;
            args.track=track;
            args.flag = _msg.getArgAsString(0);
            ofNotifyEvent(durationFlagEvent, args);
        }
        // handle bangs
        else if(type == OFXOSC_TYPE_NONE){
            durationBangEventArgs args;
            args.track=track;
            ofNotifyEvent(durationBangEvent, args);
        }
        
		
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
			//ofLogNotice();
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
// VIRTUALS FROM karmaModule
// - - - - - - - -
bool durationReceiver::enable(){
	bool ret = karmaModule::enable();
	
	// try connect
	ret *= OSCRouter::getInstance().addNode(this);
	connectOSCSender();
	
	bHasError = ret;
	bEnabled = true;
	
	return ret;
}

bool durationReceiver::disable(){
	bool ret = karmaModule::disable();
	
	// disconnect
	ret *= OSCRouter::getInstance().removeNode(this);
	bEnabled = false;
	
	return ret;
}

void durationReceiver::update(const animationParams &params){
	
	karmaModule::update(params);
	
}

void durationReceiver::draw(const animationParams &params){
	karmaModule::draw(params);
}

bool durationReceiver::reset(){
	
	
	connectOSCSender();

    return true; // tmp
}

void durationReceiver::showGuiWindow(){
	if(!bShowGuiWindow) return;
	
	ImGui::SetNextWindowSize(ImVec2(400,ofGetHeight()*0.8), ImGuiSetCond_Once);
	ImGui::Begin( ((string)"Module: ").append(karmaModule::getName()).append("###module-").append( ofToString(this) ).c_str() , &bShowGuiWindow );
	ImGui::TextWrapped("This module receives OSC messages trough the OSCRouter module and forwards them to effects and other components.");
	ImGui::TextWrapped("Duration");
	
	ImGui::Separator();
	OSCRouter::ImGuiShowOSCRouterConnectionTester();
	ImGui::Separator();
	
	if( ImGui::CollapsingHeader( "liveGrabberOSC",  ImGuiTreeNodeFlags_NoTreePushOnOpen ) ){
        if(ImGui::InputInt("Duration sending port", &oscSendParams.port)){
             connectOSCSender();
        }
		static char addrBuffer[64];
		for(int i=0; i<64; ++i){
			if(i < oscSendParams.host.size()){
				addrBuffer[i]=oscSendParams.host[i];
			}
			else {
				addrBuffer[i]=0;
			}
		}
		if(ImGui::InputText("Duration remote host", &addrBuffer[0], 64, ImGuiInputTextFlags_EnterReturnsTrue)){
			oscSendParams.host = ofToString(addrBuffer);
            connectOSCSender();
		}
	}
	
	ImGui::End();
}

void durationReceiver::drawMenuEntry() {
	karmaModule::drawMenuEntry();
}

// writes the module data to XML. xml's cursor is already pushed into the right <module> tag.
bool durationReceiver::saveToXML(ofxXmlSettings& xml) const{
	bool ret = karmaModule::saveToXML(xml);
	
	xml.addValue("OSCListeningPort", (int) oscSendParams.port);
	xml.addValue("OSCListeningHost", oscSendParams.host);
	
	return ret;
}

// load module settings from xml
// xml's cursor is pushed to the root of the <module> tag to load
bool durationReceiver::loadFromXML(ofxXmlSettings& xml){
	
	bool ret=karmaModule::loadFromXML(xml);
	
	oscSendParams.port = xml.getValue("OSCListeningPort", (int) oscSendParams.port );
	oscSendParams.host = xml.getValue("OSCListeningHost", oscSendParams.host);
    connectOSCSender();
	
	//initialise(animationParams.params);
	
	return ret; // todo
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

bool durationReceiver::connectOSCSender(){
	//sender.setup(KM_LG_OSC_ADDR, KM_LG_OSC_PORT_OUT);
	//sender.setup("192.168.0.15", 2345); // antoine
	sender.setup(oscSendParams.host, oscSendParams.port);
	bSenderIsConnected = true;
	return bSenderIsConnected;
}

bool durationReceiver::sendOscMessage(ofxOscMessage& _msg){
	if( bSenderIsConnected ){
		sender.sendMessage(_msg);
		return true;
	}
	
	return false;
}

// (addr starts with a slash)
bool durationReceiver::sendOscMessage(const string& _addr, const string& _value){
	if( bSenderIsConnected && !_addr.empty() ){
		ofxOscMessage m;
		m.setAddress(_addr);
		
		if(_value.empty()){
			m.addTriggerArg();
		}
		else {
			m.addSymbolArg(_addr);
		}
		
		return sendOscMessage(m);
	}
	
	return false;
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


const static ::module::factory::moduleDependencies  durationReceiverDependencies({"OSCRouter"});
MODULE_REGISTER( durationReceiver , "durationReceiver", durationReceiverDependencies );
