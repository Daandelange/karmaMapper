//
//  abletonLink.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 24/03/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "abletonLink.h"
#include "basicEffect.h"

ofEvent<abletonLinkSyncEventArgs> abletonLink::abletonLinkSyncEvent;
//ofEvent<abletonLinkTempoEventArgs> abletonLink::abletonLinkTempoEvent;
//ofEvent<abletonLinkNoteEventArgs> abletonLink::abletonLinkNoteEvent;
//ofEvent<abletonLinkFloatEventArgs> abletonLink::abletonLinkFloatEvent;

// STATIC MEMBER INITIALISATION
ableton::Link* abletonLink::link = new ableton::Link(120);

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -

abletonLink::abletonLink() {
	
	// karmaModule setup
	bInitialised = true;
	bEnabled = false;
	moduleName = "abletonLink";
	moduleType = "IO";
	
	//alsFilePath = "";
	bEnableSyncEvents = true;
	
	setupLink();
	
	bRequestTimeReset = false;
	abletonBeatOffset = 0;
	abletonTimeOffset = ableton::Link::Clock().micros();//
}

abletonLink::~abletonLink(){
	disable();
}

// - - - - - - - -
// OSCNode PARENT FUNCTIONS
// - - - - - - - -

// tell router if a message can be handled by this OSCNode instance
bool abletonLink::canHandle(const ofxOscMessage &_msg) const {

	return (_msg.getAddress().compare(0, KM_AL_PREFIX_LEN, KM_AL_PREFIX) == 0);
}

// proceed the message
bool abletonLink::handle(const ofxOscMessage &_msg) {
	
	// lock mutex
	ofScopedLock myLock(oscMutex);
	
	// todo: lock mutex on gl thread for access to this
	if( !isEnabled() && bEnableSyncEvents ) return false;
	
	// Ableton Link does not support timeline syncing.
	// This signal indicates Live starts playing the timeline from 0
	if( _msg.getAddress().compare("/midi/clock/start") == 0 ){
		// OFXOSC_TYPE_NONE
//		abletonLinkSyncEventArgs args;
//		args.isPlaying=true;
//		ofNotifyEvent(abletonLink::abletonLinkSyncEvent, args);
		
		bRequestTimeReset = true;
		return true; // we're done
	}
	
	// todo : get /midi/clock/continue ?
	
	// get incoming track playhead info
	// tmp disabled, using Ableton Link, should become an option
	else if( false && _msg.getAddress().compare("/midi/clock/position") == 0 ){
		
		if( _msg.getNumArgs() > 0 ){
			ofxOscArgType type = _msg.getArgType(0);
			if(type == OFXOSC_TYPE_INT32){
				int time1 = _msg.getArgAsInt32(0);
				int time2 = _msg.getArgAsInt32(1);
				cout << time1 << " - " << time2 << endl;
			}
		}
		
		return true; // we're done
	}
	
	// incoming track data
	if( false && canHandle(_msg) ){
		string track=_msg.getAddress().substr(KM_AL_PREFIX_LEN,_msg.getAddress().npos);
		
		ofxOscArgType type = OFXOSC_TYPE_NONE;
		
		if( _msg.getNumArgs() > 0 ) type = _msg.getArgType(0);
		
		if( type == OFXOSC_TYPE_FLOAT){
			
			if(_msg.getNumArgs()==1){
				
			}
		}
		else if(type == OFXOSC_TYPE_INT32){
			
		}
		// handle bangs
		else if(type == OFXOSC_TYPE_NONE){
			
			if( track.compare("bang") == 0 ){
				{ // send bangEvent
					
				}
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
void abletonLink::detachNode() {
	disable();
	// detach events ?
}

// Ableton Link stuff
void abletonLink::setupLink() {
	
//	if(link != nullptr){
//		link->enable(false);
//		//delete link;
//	}
	
	link->setNumPeersCallback([this](std::size_t /*peers*/){
		// nop
	});
	link->setTempoCallback([this](const double /*bpm*/){
		// nop
	});
	link->enable(true);
	
}

double abletonLink::getTempo(){
	if(link == nullptr){
		return 0.0;
	}
	//auto tmp = link->captureAppTimeline().tempo();
	return link->captureAppTimeline().tempo();
}

void abletonLink::setQuantum(double quantum){
	this->quantum_ = quantum;
}

double abletonLink::getQuantum(){
	return quantum_;
}

bool abletonLink::linkIsEnabled() const {
	if(link == nullptr){
		return false;
	}
	return link->isEnabled();
}

std::size_t abletonLink::getNumPeers(){
	if(link == nullptr){
		return 0;
	}
	return link->numPeers();
}

void abletonLink::updateLink(){
	
	if(link == nullptr){
		return ALStatus;
	}
	
	const auto time = link->clock().micros();
	auto timeline = link->captureAppTimeline();
	ALStatus.beat  = timeline.beatAtTime(time, quantum_);
	ALStatus.phase = timeline.phaseAtTime(time, quantum_);
	ALStatus.time = time;
}

bool abletonLink::enableLink(bool _enable){
	if(_enable){
		if(link == nullptr){
			setupLink();
		}
		else if(link == nullptr){
			return false;
		}
		
		if(bEnableSyncEvents){
			// todo
		}
		
		return linkIsEnabled();
	}
	else {
		if(link != nullptr){
			link->enable(false);
			return true;
		}
		if(link == nullptr) return true;
		
		return false;
	}
}

ableton::Link::Clock abletonLink::getClock() {
	return link->clock();
}

ableton::Link::Timeline abletonLink::getTimeline() {
	return link->captureAppTimeline();
}


// - - - - - - - -
// VIRTUALS FROM karmaModule
// - - - - - - - -
bool abletonLink::enable(){
	bool ret = karmaModule::enable();
	
	bHasError = ret;
	
	bHasError *= enableLink(true);
	bRequestTimeReset = false;
	
	// try connect
	ret *= OSCRouter::getInstance().addNode(this);
	
	return ret;
}

bool abletonLink::disable(){
	bool ret = karmaModule::disable();
	
	enableLink(false);
	
	// try connect
	ret *= OSCRouter::getInstance().removeNode(this);
	
	return ret;
}

void abletonLink::update(const animationParams &params){
	karmaModule::update(params);
	
	if(!isEnabled()) return;
	
	updateLink();
	
	// timeline reset ?
	ofScopedLock myLock(oscMutex);
	if(bRequestTimeReset){
		abletonBeatOffset = ALStatus.beat;
		abletonTimeOffset = ALStatus.time;
		
		if(bEnableSyncEvents){
			//getAbletonElapsedTime();
			// fire event
			abletonLinkSyncEventArgs args;
			args.bpm = getTempo();
			args.currentBeats = getAbletonElapsedBeats();
			args.currentPhase = ALStatus.phase;
			args.currentTime = getAbletonElapsedTime();
			args.isPlaying = true;
			args.what = "start";
			abletonLinkSyncEvent.notify(args);
		}
		
		bRequestTimeReset = false;
	} 
}

void abletonLink::draw(const animationParams &params){
	karmaModule::draw(params);
}

bool abletonLink::reset(){
	
	//return parseALSFile();
}

void abletonLink::showGuiWindow(){
	if(!bShowGuiWindow) return;
	
	ImGui::SetNextWindowSize(ImVec2(400,ofGetHeight()*0.8), ImGuiSetCond_Once);
	ImGui::Begin( ((string)"Module: ").append(karmaModule::getName()).append("###module-").append( ofToString(this) ).c_str() , &bShowGuiWindow );
	ImGui::TextWrapped("Ableton Link to get events from Link and synchronize with others.");
	
	ImGui::Separator();
	
	if( ImGui::CollapsingHeader( "Link Status", "abletonLinkStatus", true, true ) ){
		
		
		ImGui::TextWrapped("Ableton Link status: %s", ((linkIsEnabled())?"On":"Off"));
		ImGui::TextWrapped("Ableton Link peers: %lu", getNumPeers());
		ImGui::TextWrapped("To enable Ableton Link, click think button top left in Live 9.6+. It should automatically connect with this module.");
		
		//ImGui::TextWrapped("OSC Listening : %s", ((OSCNode::)?"On":"Off"));
		ImGui::TextWrapped("OSC listens on port 9001 for syncing information coming from liveOSC. Install plugin and enable in Ableton Live preferences. Announcing is not done yet, so please notify Ableton that we're listening on port 9001 with any other standard OSC app.");
		
		OSCRouter::ImGuiShowOSCRouterConnectionTester();
		
//		ImGui::SameLine();
//		if(ImGui::Button("Reload")){
//			
//		}
		
		ImGui::Separator();
		
		if(linkIsEnabled()){
			if( ImGui::CollapsingHeader( "Link Quantum", "abletonLinkQuantum", true, true ) ){
				
				int quantum = (int) ceil( getQuantum() );
				float phase = ALStatus.phase;
				float beat = ALStatus.beat;
				
				ImGui::TextWrapped("Beat: %f", getAbletonElapsedBeats() );
				
				ImGui::TextWrapped("Ableton Time: %f sec", getAbletonElapsedTime() );
				ImGui::SliderFloat("Phase", &phase, 0, quantum);
				double tempo = getTempo();
				ImGui::TextWrapped("Tempo: %f", tempo);
				
			}
		}
	}
	
	ImGui::Separator();
	
	if( ImGui::CollapsingHeader( "Event Params", "abletonLinkEventParams", true, true ) ){

		if(ImGui::Checkbox("Enable Sync Events", &bEnableSyncEvents)){
			// todo: apply setting
		}
	}
	
	ImGui::End();
}

void abletonLink::drawMenuEntry() {
	karmaModule::drawMenuEntry();
}

// writes the module data to XML. xml's cursor is already pushed into the right <module> tag.
bool abletonLink::saveToXML(ofxXmlSettings& xml) const{
    bool ret = karmaModule::saveToXML(xml);
	
	//xml.addValue("abletonLinkALSFilePath", alsFilePath);
    xml.addValue("abletonLinkbEnableSyncEvents", bEnableSyncEvents);
    
    return ret;
}

// load module settings from xml
// xml's cursor is pushed to the root of the <module> tag to load
bool abletonLink::loadFromXML(ofxXmlSettings& xml){
    
    bool ret=karmaModule::loadFromXML(xml);
	
	//alsFilePath = xml.getValue("abletonLinkALSFilePath", "");
	bEnableSyncEvents = xml.getValue("abletonLinkbEnableSyncEvents", bEnableSyncEvents );
	//parseALSFile();
    
    return ret;
}

// GETTERS
double abletonLink::getAbletonElapsedBeats() const{
	if(link == nullptr) return 0.f;
	
	return ALStatus.beat - abletonBeatOffset;
}

float abletonLink::getAbletonElapsedTime() const{
	if(link == nullptr) return 0.f;
	
	std::chrono::duration<float> secTime = std::chrono::duration_cast< std::chrono::milliseconds >( link->clock().micros() - abletonTimeOffset );
	return secTime.count();
}


const static ::module::factory::moduleDependencies  abletonLinkDependencies({});
MODULE_REGISTER( abletonLink , "abletonLink", abletonLinkDependencies );
