//
//  liveGrabberOSC.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 24/03/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "liveGrabberOSC.h"

// forward declarations are needed for event listening
ofEvent<liveGrabberBangEventArgs> liveGrabberOSC::liveGrabberBangEvent;
ofEvent<liveGrabberTempoEventArgs> liveGrabberOSC::liveGrabberTempoEvent;
ofEvent<liveGrabberNoteEventArgs> liveGrabberOSC::liveGrabberNoteEvent;

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
liveGrabberOSC::liveGrabberOSC() {
	
	// OSCNode setup
	nodeName = "liveGrabberOSC";
	
	// karmaModule setup
	bInitialised = true;
	bEnabled = false;
	moduleName = "liveGrabberOSC";
	moduleType = "liveGrabberOSC";
}

liveGrabberOSC::~liveGrabberOSC(){
	disable();
}

// - - - - - - - -
// OSCNode PARENT FUNCTIONS
// - - - - - - - -

// tell router if a message can be handled by this OSCNode instance
bool liveGrabberOSC::canHandle(const ofxOscMessage &_msg) const {
	return true; // todo
}

// proceed the message
bool liveGrabberOSC::handle(const ofxOscMessage &_msg) {
	if( !isEnabled() ) return false;
	
	// lock mutex (threaded execution)
	ofScopedLock myLock(oscMutex);
	
	string addr = _msg.getAddress();
	
	// handle messages coming from Ableton Live - Live Grabber
	if( addr.substr(0,6).compare("/allg/")==0 ){
		string tmpLastMsg = "";
		string subAddr = addr.substr(5,addr.npos);
		bool ret = false;
		
		if( subAddr.substr(0,9).compare("/Follower") == 0 ){
			
			if(_msg.getNumArgs()>0){
				subAddr = subAddr.substr(9,addr.npos);
				float value = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
				
				if( subAddr.compare("Low") == 0 ){
					liveGrabberAnalysis.lowFollower = value;
					tmpLastMsg = "LowFollower";
				}
				else if( subAddr.compare("Mid") == 0 ){
					liveGrabberAnalysis.midFollower = value;
					tmpLastMsg = "MidFollower";
				}
				else if( subAddr.compare("High") == 0 ){
					liveGrabberAnalysis.highFollower = value;
					tmpLastMsg = "HighFollower";
				}
			}
			ret = true;
		}
		
		if( subAddr.substr(0,8).compare("/Trigger") == 0 ){
			
			if(_msg.getNumArgs()>0){
				
				subAddr = subAddr.substr(8,addr.npos);
				float value = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);
				liveGrabberBangEventArgs args;
				
				if( subAddr.compare("Low") == 0 ){
					args.what = "ButtonLow";
				}
				else if( subAddr.compare("Mid") == 0 ){
					args.what = "ButtonMid";
				}
				else if( subAddr.compare("High") == 0 ){
					args.what = "ButtonHigh";
				}
				ofNotifyEvent(liveGrabberBangEvent, args);
				
				ret = true;
			}
		}
		
		else if( subAddr.substr(0,6).compare("/note-") == 0 ){
			subAddr = subAddr.substr(6,addr.npos);
			
			if( subAddr.compare("A0") == 0 ){
				{
					// send bangEvent
					liveGrabberBangEventArgs args;
					args.what="Note-A0";
					ofNotifyEvent(liveGrabberBangEvent, args);
				}
				{
					// send bangEvent
					liveGrabberNoteEventArgs args;
					args.key = "A0";
					//args.isTempoBis=false;
					ofNotifyEvent(liveGrabberNoteEvent, args);
				}
				tmpLastMsg = "Note A0";
				ret = true;
			}
			else if( subAddr.compare("A0m") == 0 ){
				{
					// send bangEvent
					liveGrabberBangEventArgs args;
					args.what="Note-A#0";
					ofNotifyEvent(liveGrabberBangEvent, args);
				}
				{
					// send bangEvent
					liveGrabberNoteEventArgs args;
					args.key = "A#0";
					//args.isTempoBis=false;
					ofNotifyEvent(liveGrabberNoteEvent, args);
				}
				tmpLastMsg = "Note A#0";
				ret = true;
			}
		}
	
		//...else if(){}
		
		// unrecognized /kmsa/ messge
		else {
			tmpLastMsg = subAddr.append(" (unrecognised)");
			//cout << subAddr << " - " << addr << endl;
			ret = true; // flags the message as 'intercepted'
		}
		
		lastReceivedParamName = tmpLastMsg;
		return ret;
	}
	
	// unrecognized messge
	return false;
}

// informs that the node id gonna be unbound with the node server
void liveGrabberOSC::detachNode() {
	disable();
	// detach events ?
}

void liveGrabberOSC::notifyDetached(){
	ofScopedLock(oscMutex);
}

// - - - - - - - -
// VIRTUALS FROM karmaModule
// - - - - - - - -
bool liveGrabberOSC::enable(){
	bool ret = karmaModule::enable();
	
	// try connect
	ret *= OSCRouter::getInstance().addNode(this);
	
	bHasError = ret;
	
	return ret;
}

bool liveGrabberOSC::disable(){
	bool ret = karmaModule::disable();
	
	// disconnect
	ret *= OSCRouter::getInstance().removeNode(this);
	
	return ret;
}

void liveGrabberOSC::update(const animationParams &params){
	karmaModule::update(params);
}

void liveGrabberOSC::draw(const animationParams &params){
	karmaModule::draw(params);
}

bool liveGrabberOSC::reset(){
	lastReceivedParamName="";
}

void liveGrabberOSC::showGuiWindow(){
	if(!bShowGuiWindow) return;
	
	ImGui::SetNextWindowSize(ImVec2(400,ofGetHeight()*0.8), ImGuiSetCond_Once);
	ImGui::Begin( ((string)"Module: ").append(karmaModule::getName()).append("###module-").append( ofToString(this) ).c_str() , &bShowGuiWindow );
	ImGui::TextWrapped("This module receives OSC messages trough the OSCRouter module and forwards them to effects and other components.");
	ImGui::TextWrapped("To use ableton live with LiveGrabber Max for Live plugin.");
	
	ImGui::Separator();
	OSCRouter::ImGuiShowOSCRouterConnectionTester();
	ImGui::Separator();
	
	oscMutex.lock();
	ImGui::TextWrapped("Last Received Param: %s", lastReceivedParamName.c_str());
	oscMutex.unlock();
	
	ImGui::End();
}

void liveGrabberOSC::drawMenuEntry() {
	karmaModule::drawMenuEntry();
}


liveGrabberAnalysisData liveGrabberOSC::liveGrabberAnalysis;// = mirData();

const static ::module::factory::moduleDependencies  liveGrabberOSCDependencies({"OSCRouter"});
MODULE_REGISTER( liveGrabberOSC , "liveGrabberOSC", liveGrabberOSCDependencies );
