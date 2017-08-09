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
ofEvent<liveGrabberFloatEventArgs> liveGrabberOSC::liveGrabberFloatEvent;

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
liveGrabberOSC::liveGrabberOSC() {
	
	// karmaModule setup
	bInitialised = true;
	bEnabled = false;
	bSenderIsConnected = false;
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
	return (_msg.getAddress().compare(0,ALLG_PREFIX_LEN, KM_ALLG_PREFIX) == 0);
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
        
        else if( subAddr.substr(0,13).compare("/clipNameHang") == 0 ){
            
            if(_msg.getNumArgs()>0){
                subAddr = subAddr.substr(13,addr.npos);
                string value = _msg.getArgTypeName(0); // tmp
                
                liveGrabberBangEventArgs args;
                args.what = "hangDrum";//_msg.getArgAsString(0);
                ofNotifyEvent(liveGrabberBangEvent, args);
            }
            ret = true;
        }
        
        
        else if( subAddr.substr(0,8).compare("/criquet") == 0 ){
            
            if(_msg.getNumArgs()>0){
                
                subAddr = subAddr.substr(8,addr.npos);
                
                liveGrabberFloatEventArgs args;
                args.what = "criquet";
                args.value = _msg.getArgTypeName(0)=="int32"?_msg.getArgAsInt32(0):_msg.getArgAsFloat(0);;
                ofNotifyEvent(liveGrabberFloatEvent, args);
                
                tmpLastMsg = "criquet";
                ret = true;
            }
            
        }
        
		
		// /allg/TriggeredClipName"
		
		else if( subAddr.length()==8 && subAddr.substr(0,8).compare("/Trigger") == 0 ){
			
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
		
		else if( subAddr.substr(0,7).compare("/piano/") == 0 ){
			subAddr = subAddr.substr(7,addr.npos);
			
			if(subAddr.length()==2 || subAddr.length()==3){
				{
					// send bangEvent
					liveGrabberBangEventArgs args;
					args.what="Note-"+subAddr;
					ofNotifyEvent(liveGrabberBangEvent, args);
				}
				{
					// send bangEvent
					liveGrabberNoteEventArgs args;
					args.key = subAddr;
					args.isMajor = (subAddr.length()==3);
					//args.isTempoBis=false;
					ofNotifyEvent(liveGrabberNoteEvent, args);
				}
				tmpLastMsg = "Piano "+subAddr;
				ret = true;
			}
		}
		
		else if( subAddr.substr(0,5).compare("/kick") == 0 ){
			
			if(_msg.getNumArgs()>0){
				
				subAddr = subAddr.substr(5,addr.npos);
				
				liveGrabberBangEventArgs args;
				args.what = "kick";
				ofNotifyEvent(liveGrabberBangEvent, args);
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
	//ofScopedLock(oscMutex);
}

// - - - - - - - -
// VIRTUALS FROM karmaModule
// - - - - - - - -
bool liveGrabberOSC::enable(){
	bool ret = karmaModule::enable();
	
	// try connect
	ret *= OSCRouter::getInstance().addNode(this);
	connectOSCSender();
	
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
	
	if(bSenderIsConnected && (ofGetFrameNum()%2==0) ){
		noise = ofNoise(ofGetElapsedTimef());
		random = ofRandom(0.f, 1.f);
		
//		ofxOscMessage m;
//		m.setAddress("noise");
//		//m.addInt32Arg(noise);
//		m.addFloatArg(noise);
//		//m.addStringArg(ofToString(noise));
//		sendOscMessage(m);
		
//		m.clear();
//		m.setAddress("random");
//		m.addFloatArg(random);
//		sendOscMessage(m);
		
//		if(ofGetFrameNum()%((int)ofRandom(100, 300))==0){
//			m.clear();
//			m.setAddress("trigger");
//			m.addFloatArg(manualFloat);
//			sendOscMessage(m);
//		}
	}
	
	
}

void liveGrabberOSC::draw(const animationParams &params){
	karmaModule::draw(params);
}

bool liveGrabberOSC::reset(){
	lastReceivedParamName="";
	
	connectOSCSender();

    return true; // tmp
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
	
	
	if(ImGui::InputInt("LiveGrabber sending port", &oscSendParams.port)){
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
	if(ImGui::InputText("LiveGrabber remote host", &addrBuffer[0], 64, ImGuiInputTextFlags_EnterReturnsTrue)){
		oscSendParams.host = ofToString(addrBuffer);
		connectOSCSender();
	}
	
	ImGui::Separator();
	ImGui::Separator();
	
	oscMutex.lock();
	ImGui::TextWrapped("Last Received Param: %s", lastReceivedParamName.c_str());
	oscMutex.unlock();
	
	if( ImGui::CollapsingHeader( "Params", "liveGrabberOSC", true, true ) ){
		ImGui::SliderFloat("noise", &noise, 0.0, 1.0);
		ImGui::SliderFloat("random", &random, 0.0, 1.0);
		if(ImGui::SliderFloat("manualFloat", &manualFloat, 0.0, 1.0)){
			ofxOscMessage m;
			m.setAddress("manualFloat");
			m.addFloatArg(manualFloat);
			sendOscMessage(m);
		}
		if(ImGui::Button("manualTrigger")){
			ofxOscMessage m;
			m.setAddress("manualTrigger");
			m.addTriggerArg();
			//m.addImpulseArg();
			sendOscMessage(m);
		}
	}
	
	ImGui::End();
}

void liveGrabberOSC::drawMenuEntry() {
	karmaModule::drawMenuEntry();
}

// writes the module data to XML. xml's cursor is already pushed into the right <module> tag.
bool liveGrabberOSC::saveToXML(ofxXmlSettings& xml) const{
    bool ret = karmaModule::saveToXML(xml);
    
    xml.addValue("OSCListeningPort", (int) oscSendParams.port);
    xml.addValue("OSCListeningHost", oscSendParams.host);
    
    return ret;
}

// load module settings from xml
// xml's cursor is pushed to the root of the <module> tag to load
bool liveGrabberOSC::loadFromXML(ofxXmlSettings& xml){
    
    bool ret=karmaModule::loadFromXML(xml);
    
    oscSendParams.port = xml.getValue("OSCListeningPort", (int) oscSendParams.port );
    oscSendParams.host = xml.getValue("OSCListeningHost", oscSendParams.host);
    connectOSCSender();
    
    //initialise(animationParams.params);
    
    return ret; // todo
}

bool liveGrabberOSC::connectOSCSender(){
	//sender.setup(KM_LG_OSC_ADDR, KM_LG_OSC_PORT_OUT);
	//sender.setup("192.168.9.26", 2345); // antoine
    try {
        sender.setup(oscSendParams.host, oscSendParams.port);
        bSenderIsConnected = true;
    } catch (...) {
        bSenderIsConnected = false;
    }
	
	return bSenderIsConnected;
}

bool liveGrabberOSC::sendOscMessage(ofxOscMessage& _msg){
	if( bSenderIsConnected ){
		sender.sendMessage(_msg);
		return true;
	}
	
	return false;
}

// (addr starts with a slash)
bool liveGrabberOSC::sendOscMessage(const string& _addr, const string& _value){
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


liveGrabberAnalysisData liveGrabberOSC::liveGrabberAnalysis;// = mirData();

const static ::module::factory::moduleDependencies  liveGrabberOSCDependencies({"OSCRouter"});
MODULE_REGISTER( liveGrabberOSC , "liveGrabberOSC", liveGrabberOSCDependencies );
