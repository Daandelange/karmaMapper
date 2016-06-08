//
//  chladniReceiver.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 05/06/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "chladniReceiver.h"
#include "basicEffect.h"
#include "chladniRC.h"

// forward declarations are needed for event listening
ofEvent<chladniBangEventArgs> chladniReceiver::chladniBangEvent;
ofEvent<chladniFloatEventArgs> chladniReceiver::chladniFloatEvent;

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
chladniReceiver::chladniReceiver( ){
	
	//
	bEnabled = false;
	
	moduleName = "chladniReceiver";
	moduleType = "chladniReceiver";
}

chladniReceiver::~chladniReceiver(){
	stop();
}

// - - - - - - - -
// OSCNode PARENT FUNCTIONS
// - - - - - - - -

// tell router if a message can be handled by this OSCNode instance
bool chladniReceiver::canHandle(const ofxOscMessage &_msg) const {
	//string tmp = _msg.getAddress().substr(0,CHLADNI_PREFIX_LEN)+" • " + ofToString(CHLADNI_PREFIX_LEN);
	return (_msg.getAddress().compare(0,CHLADNI_PREFIX_LEN, CHLADNI_PREFIX) == 0);
}

// proceed the message
bool chladniReceiver::handle(const ofxOscMessage &_msg) {
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
		string track=_msg.getAddress().substr(CHLADNI_PREFIX_LEN,_msg.getAddress().npos);
		
		ofxOscArgType type = OFXOSC_TYPE_NONE;
		if( _msg.getNumArgs() > 0 ) type = _msg.getArgType(0);
		
		if( type == OFXOSC_TYPE_FLOAT){
            float value = 0;
            if(_msg.getNumArgs()>0) value=_msg.getArgAsFloat(0);
            chladniFloatEventArgs args;
            args.name=track;
            //args.type="curve";
            args.value=value;
            ofNotifyEvent(chladniFloatEvent, args);
        }
        else if(type == OFXOSC_TYPE_INT32){
            
        }
        // handle bangs
        else if(type == OFXOSC_TYPE_NONE){
			
			if( track.compare("bang") == 0 ){
				{ // send bangEvent
					chladniBangEventArgs args;
					args.name=track;
					ofNotifyEvent(chladniBangEvent, args);
				}
			}
			else if(track.compare("ping") == 0 ){
				// todo: respond to ping
				chladniRC::getInstance().sendPing();
			}
			
			else {
				chladniBangEventArgs args;
				args.name=track;
				ofNotifyEvent(chladniBangEvent, args);
			}
        }
		
		// unknown track, could not get information
		else {
			// todo: try to add the signal here
			//ofLogNotice();
		}
		return true;
	}
	
	else {
		//cout << "Not recognized OSC = " << _msg.getAddress() << endl;
	}
	
	// unrecognized messge
	return false;
}

// informs that the nose id gonna be unbound with the node server
void chladniReceiver::detachNode() {
	stop();
	// detach events ?
}

// - - - - - - - -
// VIRTUALS FROM karmaModule
// - - - - - - - -
bool chladniReceiver::enable(){
	bool ret = karmaModule::enable();
	
	// try connect
	ret *= OSCRouter::getInstance().addNode(this);
	connectOSCSender();
	
	bHasError = ret;
	bEnabled = true;
	
	return ret;
}

bool chladniReceiver::disable(){
	bool ret = karmaModule::disable();
	
	// disconnect
	ret *= OSCRouter::getInstance().removeNode(this);
	bEnabled = false;
	
	return ret;
}

void chladniReceiver::update(const animationParams &params){
	
	karmaModule::update(params);
	
}

void chladniReceiver::draw(const animationParams &params){
	karmaModule::draw(params);
}

bool chladniReceiver::reset(){
	
	
	connectOSCSender();
}

void chladniReceiver::showGuiWindow(){
	if(!bShowGuiWindow) return;
	
	ImGui::SetNextWindowSize(ImVec2(400,ofGetHeight()*0.8), ImGuiSetCond_Once);
	ImGui::Begin( ((string)"Module: ").append(karmaModule::getName()).append("###module-").append( ofToString(this) ).c_str() , &bShowGuiWindow );
	ImGui::TextWrapped("This module receives OSC messages trough the OSCRouter module and forwards them to effects and other components.");
	ImGui::TextWrapped("Works together with a chladni plate / cymatics wave generator in PureData.");
	
	ImGui::Separator();
	OSCRouter::ImGuiShowOSCRouterConnectionTester();
	ImGui::Separator();
	
	if( ImGui::CollapsingHeader( "Params", "chladniReceiverOSC", true, true ) ){
        if(ImGui::InputInt("chladni sending port", &oscSendParams.port)){
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
		if(ImGui::InputText("chladni remote host", &addrBuffer[0], 64, ImGuiInputTextFlags_EnterReturnsTrue)){
			oscSendParams.host = ofToString(addrBuffer);
            connectOSCSender();
		}
	}
	
	ImGui::End();
}

void chladniReceiver::drawMenuEntry() {
	karmaModule::drawMenuEntry();
}

// writes the module data to XML. xml's cursor is already pushed into the right <module> tag.
bool chladniReceiver::saveToXML(ofxXmlSettings& xml) const{
	bool ret = karmaModule::saveToXML(xml);
	
	xml.addValue("OSCListeningPort", (int) oscSendParams.port);
	xml.addValue("OSCListeningHost", oscSendParams.host);
	
	return ret;
}

// load module settings from xml
// xml's cursor is pushed to the root of the <module> tag to load
bool chladniReceiver::loadFromXML(ofxXmlSettings& xml){
	
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
bool chladniReceiver::start(){
	bEnabled = true;
	
	// bind events
	//ofAddListener(, chladniReceiver:: *listener, )
	
	return isEnabled()==true;
}

bool chladniReceiver::stop(){
	bEnabled = false;
	
	// unbind events
	
	return isEnabled()==false;
}

bool chladniReceiver::connectOSCSender(){
	//sender.setup(KM_LG_OSC_ADDR, KM_LG_OSC_PORT_OUT);
	//sender.setup("192.168.0.15", 2345); // antoine
	sender.setup(oscSendParams.host, oscSendParams.port);
	bSenderIsConnected = true;
	return bSenderIsConnected;
}

bool chladniReceiver::sendOscMessage(ofxOscMessage& _msg){
	if( bSenderIsConnected ){
		sender.sendMessage(_msg);
		return true;
	}
	
	return false;
}

// (addr starts with a slash)
bool chladniReceiver::sendOscMessage(const string& _addr, const string& _value){
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
void chladniReceiver::oscIn(){
	// todo
}//*/

// GETTERS
bool chladniReceiver::isEnabled() const {
	return bEnabled;
}


const static ::module::factory::moduleDependencies  chladniReceiverDependencies({"OSCRouter"});
MODULE_REGISTER( chladniReceiver , "chladniReceiver", chladniReceiverDependencies );
