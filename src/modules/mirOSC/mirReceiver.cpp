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
mirReceiver::mirReceiver() {
	
	// OSCNode setup
	nodeName = "mirReceiver";
	
	// karmaModule setup
	bInitialised = true;
	bEnabled = false;
	moduleName = "mirReceiver";
	moduleType = "mirReceiver";
}

mirReceiver::~mirReceiver(){
	disable();
}

// - - - - - - - -
// OSCNode PARENT FUNCTIONS
// - - - - - - - -

// tell router if a message can be handled by this OSCNode instance
bool mirReceiver::canHandle(const ofxOscMessage &_msg) const {
	return true; // todo
}

// proceed the message
bool mirReceiver::handle(const ofxOscMessage &_msg) {
	if( !isEnabled() ) return false;
	
	// lock mutex (threaded execution)
	ofScopedLock myLock(oscMutex);
	
	string addr = _msg.getAddress();
	
	// handle messages coming from karma Sound Analyser
	if( addr.substr(0,6)=="/kmsa/" ){
		string tmpLastMsg = "";
		string subAddr = addr.substr(5,addr.npos);
		
		if( subAddr.substr(0,7).compare("/aubio/") == 0 ){
			subAddr = subAddr.substr(6,addr.npos);
			if( subAddr.compare("/tempo") == 0 ){
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
			else if( subAddr.compare("/tempoBis") == 0 ){
				{
					// send bangEvent
					mirBangEventArgs args;
					args.what="aubioTempoBis";
					tmpLastMsg = args.what;
					ofNotifyEvent(mirBangEvent, args);
				}
				{
					// send tmpoEvent
					mirTempoEventArgs args;
					args.bpm = mirCache.bpm;
					args.isTempoBis=true;
					ofNotifyEvent(mirTempoEvent, args);
				}
				return true;
			}
			else if( subAddr.compare("/onSet") == 0 ){
				{
					// send OnSetEvent
					mirOnSetEventArgs args;
					args.source="aubioOnSet";
					tmpLastMsg = args.source;
					ofNotifyEvent(mirOnSetEvent, args);
				}
				return true;
			}
			else if( subAddr.compare("/bpm") == 0 ){
				if(_msg.getNumArgs()>0) mirCache.bpm = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
				
				tmpLastMsg = "bpm";
				
				return true;
			}
			else if( subAddr.compare("/pitch") == 0 ){
				if(_msg.getNumArgs()>0)	mirCache.pitch = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
				return true;
			}
			else if( subAddr.compare("/quiet") == 0 ){
				if(_msg.getNumArgs()>0) mirCache.silence = (_msg.getArgAsInt32(0)==0)?true:false;
				return true;
			}
			else if( subAddr.compare("/playing") == 0 ){
				if(_msg.getNumArgs()>0) mirCache.isPlaying = (_msg.getArgAsFloat(0)==0)?true:false;
				return true;
			}
			else if( subAddr.compare("/zcr") == 0 ){
				if(_msg.getNumArgs()>0) mirCache.zcr = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
				return true;
			}
			else if( subAddr.compare("/noisy") == 0 ){
				//if(_msg.getNumArgs()>0) mirCache.zcr = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
				return true;
			}
		}
		else if( subAddr.compare("/balance") == 0 ){
			if(_msg.getNumArgs()>0) mirCache.balance = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
			return true;
		}
		// Fiddle FFT Equaliser
		else if( subAddr.compare(0, 16, "/fiddle_fft/band") == 0 ){ // 16 first chars
			int band = ofToInt(addr.substr(16));
			
			if(addr.compare(17, 20, "rms")==0) // rms (volume)
				mirCache.fiddleFFT[band][0]=_msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
			else // pitch
				mirCache.fiddleFFT[band][1]=_msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
			
			return true;
		}
		
		// unrecognized /kmsa/ messge
		else {
			return true; // flags the message as 'intercepted'
		}
	}
	
	// unrecognized messge
	return false;
}

// informs that the node id gonna be unbound with the node server
void mirReceiver::detachNode() {
	disable();
	// detach events ?
}

void mirReceiver::notifyDetached(){
	ofScopedLock(oscMutex);
}

// - - - - - - - -
// VIRTUALS FROM karmaModule
// - - - - - - - -
bool mirReceiver::enable(){
	bool ret = karmaModule::enable();
	
	// try connect
	ret *= OSCRouter::getInstance().addNode(this);
	
	bHasError = ret;
	
	return ret;
}

bool mirReceiver::disable(){
	bool ret = karmaModule::disable();
	
	// disconnect
	ret *= OSCRouter::getInstance().removeNode(this);
	
	return ret;
}

void mirReceiver::update(const animationParams &params){
	karmaModule::update(params);
}

void mirReceiver::draw(const animationParams &params){
	karmaModule::draw(params);
}

bool mirReceiver::reset(){
	lastReceivedParamName="";
}

void mirReceiver::showGuiWindow(){
	if(!bShowGuiWindow) return;
	
	ImGui::SetNextWindowSize(ImVec2(400,ofGetHeight()*0.8), ImGuiSetCond_Once);
	ImGui::Begin( ((string)"Module: ").append(karmaModule::getName()).append("###module-").append( ofToString(this) ).c_str() , &bShowGuiWindow );
	ImGui::TextWrapped("This module receives OSC messages trough the OSCRouter module and forwards them to effects and other components.");
	
	ImGui::Separator();
	static bool oscRouterConnection = false;
	if( ImGui::Button("Test connection with OSC Router" )){
		oscRouterConnection = OSCRouter::getInstance().isEnabled();
		ImGui::OpenPopup("oscRouterConnectionResult");
	}
	if( ImGui::BeginPopup("oscRouterConnectionResult") ){
		ImGui::SameLine();
		
		if (oscRouterConnection){
			ImGui::Text("Up & Running! :)");
		}
		else {
			ImGui::Text("Error... Please check if the OSCRouter module is enabled.");
		}
		if(ImGui::Button("Ok")){
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::EndPopup();
	}
	
	ImGui::Separator();
	
	oscMutex.lock();
	ImGui::TextWrapped("Last Received Param: %s", lastReceivedParamName.c_str());
	oscMutex.unlock();
	
	ImGui::End();
}

void mirReceiver::drawMenuEntry() {
	karmaModule::drawMenuEntry();
}


mirData mirReceiver::mirCache;// = mirData();

const static ::module::factory::moduleDependencies  mirReceiverDependencies({"OSCRouter"});
MODULE_REGISTER( mirReceiver , "mirReceiver", mirReceiverDependencies );
