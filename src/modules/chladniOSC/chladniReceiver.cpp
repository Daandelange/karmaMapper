//
//  chladniReceiver.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 05/06/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "chladniReceiver.h"
#include "basicEffect.h"

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
			
			if(_msg.getNumArgs()==1){
				float value = 0;
				value=_msg.getArgAsFloat(0);
				chladniFloatEventArgs args;
				args.name=track;
				//args.type="curve";
				args.value=value;
				ofNotifyEvent(chladniFloatEvent, args);
			}
			else if ( _msg.getNumArgs() == 2 ){
				// todo
			}
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
	
	start();
	
	bHasError = ret;
	bEnabled = true;
	
	return ret;
}

bool chladniReceiver::disable(){
	bool ret = karmaModule::disable();
	
	// disconnect
	ret *= OSCRouter::getInstance().removeNode(this);
	bEnabled = false;
	
	stop();
	
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
	
	if( ImGui::CollapsingHeader( "OSC Setup / Config (Pd)", "chladniReceiverOSC", true, true ) ){
		
		ImGui::TextWrapped("Pd OSC connection");
		ImGui::Indent();
		ImGui::TextWrapped("Configure how you connect to PureData patch /utilities/chladni-plate-generator.pd");
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
		
		if( chladniRC::getInstance().pureDataIsConnected()){
			ImGui::TextWrapped("Status: Connected");
		}
		else {
			ImGui::TextWrapped("Status: Not Connected");
		}
		
		if(ImGui::Button("Reconnect")){
			connectOSCSender();
		}
		
		// todo: launch pd from OF ?
		// see: https://forum.openframeworks.cc/t/using-shell-script-to-launch-external-apps-from-of/3853
		// see: http://stackoverflow.com/questions/646217/how-to-run-a-bash-script-from-c-program
		
		ImGui::Unindent();
		
		ImGui::Separator();
		
		if( chladniRC::getInstance().pureDataIsConnected() && ImGui::CollapsingHeader( "Chladni-plate Pd Controls", "chladniReceiverPdCommands", true, true ) ){
			
			ImGui::TextWrapped("Pd OSC Controls");
			ImGui::Indent();
			ImGui::TextWrapped("Control the chladni Pd-patch !");
			
			
			if(ImGui::Button("Trigger change")){
				sendOscMessage("/km/chladni/randomChange", "1");
			}
			
			static float frequency = 64.05f;
			if( ImGui::SliderFloat("Frequency", &frequency, 30.f, 140.f) ){
				sendOscMessage( "/km/chladni/setFreq", frequency );
			}
			
			static int delay = 600;
			if( ImGui::SliderInt("Delay (ms)", &delay, 0, 1000) ){
				sendOscMessage( "/km/chladni/setDelay", delay );
			}
			
//			static broadcast
//			if(ImGui::Checkbox("react to notes")){
//				
//			}
			
			ImGui::Unindent();
		}
	}
	ImGui::Separator();
	
	static bool wasOpen = false;
	if( ImGui::CollapsingHeader( "Arduino Setup (water control)", "chladniReceiverArduino", true, true ) ){
		
		static std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
			
		// refresh list ?
		if(!wasOpen){
			devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
			
			wasOpen = true;
		}
		
		ImGui::Indent();
		
		ImGui::TextWrapped("Communicates with a USB Arduino device. Needs patch: /src/modules/serialController_v1/karmaMapperSerial_v1/karmaMapperSerial_v1.ino");
		
		if(ImGui::Button("Connect to first Arduino device")){
			chladniRC::getInstance().setupArduinoSerial();
		}
		
		if(ImGui::ListBoxHeader("Connect to device...")){
			
			if (!devicesInfo.empty()){
				for (std::size_t i = 0; i < devicesInfo.size(); ++i) {
					if(ImGui::Selectable( ( devicesInfo[i].getDescription() + " (" + devicesInfo[i].getPort() + ")" +"###"+devicesInfo[i].getDescription()).c_str(), devicesInfo[i].getPort().compare( chladniRC::getInstance().getArduinoDevicePort() )==0 )){
						
						if( chladniRC::getInstance().connectToArduino(devicesInfo[i]) ){
							// todo: remember device ?
						}
						else {
							// todo: notify failure
							
						}
					}
				}
			}
			else {
				ImGui::TextWrapped("[None Available]");
			}
			ImGui::ListBoxFooter();
		}
		
		if( chladniRC::getInstance().arduinoIsConnected()){
			ImGui::TextWrapped("Status: Connected to: %s", chladniRC::getInstance().getArduinoDevicePort().c_str() );
		}
		else {
			ImGui::TextWrapped("Status: Not Connected");
		}
		
		ImGui::Separator();
		
		// ping
		if(chladniRC::getInstance().arduinoIsConnected()){
			if(ImGui::Button("Ping Device")){
				chladniRC::getInstance().pingArduino();
			}
			ImGui::TextWrapped("Last ping return: %s", chladniRC::getInstance().getLastArduinoPingMessage().c_str() );
			
			ImGui::Separator();
			
			// Water flow
			ImGui::TextWrapped("Water Flow Control:");
			ImGui::Indent();
			for(int ev=0; ev<KM_CHLADNI_NUM_ELECTROVALVES; ++ev){
				float flow = chladniRC::getInstance().getSolenoidFlow(ev);
				if( ImGui::SliderFloat((ofToString("EV")+ofToString(ev)).c_str(), &flow, 0.f, 1.f) ){
					chladniRC::getInstance().setSolenoidFlow(ev, flow);
				}
			}
			ImGui::Unindent();
			
			ImGui::Separator();
			
			// Led strips
			ImGui::TextWrapped("LED strips:");
			ImGui::Indent();
			for(int ls=0; ls<KM_CHLADNI_NUM_LED_STRIPS; ++ls){
				ImGui::TextWrapped("LED strip #%d", ls);
				ImGui::Indent();
				float intensityManu = chladniRC::getInstance().getLEDStripIntensityManu(ls);
				ImGui::SliderFloat((ofToString("LED strip ")+ofToString(ls)+" manu value").c_str(), &intensityManu, 0.f, 1.f);
				float intensityAuto = chladniRC::getInstance().getLEDStripIntensityAuto(ls);
				if(ImGui::SliderFloat((ofToString("LED strip ")+ofToString(ls)+" auto value").c_str(), &intensityAuto, 0.f, 1.f)){
					chladniRC::getInstance().setLEDStripIntensityAuto(ls, intensityAuto);
				}
				ImGui::Unindent();
			}
			ImGui::Unindent();
			
			// Flowmeters
			ImGui::TextWrapped("Flow meters:");
			ImGui::Indent();
			for(int fm=0; fm<KM_CHLADNI_NUM_LED_STRIPS; ++fm){
				ImGui::TextWrapped("Flow Meter %d", fm);
				float value = chladniRC::getInstance().getFlowMeterRate(fm);
				ImGui::SliderFloat("Flow rate", &value, 0.f, 1.f);
			}
		}
		
		ImGui::Unindent();
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
	ofAddListener(ofx::AbletonLiveSet::alsLinkEventHandler::alsTrackEvent, this, &chladniReceiver::onAlsTrackEvent );
	ofAddListener(ofx::AbletonLiveSet::alsLinkEventHandler::alsNoteEvent, this, &chladniReceiver::onAlsNoteEvent );
	
	return isEnabled()==true;
}

bool chladniReceiver::stop(){
	bEnabled = false;
	
	// unbind events
	ofRemoveListener(ofx::AbletonLiveSet::alsLinkEventHandler::alsTrackEvent, this, &chladniReceiver::onAlsTrackEvent );
	ofRemoveListener(ofx::AbletonLiveSet::alsLinkEventHandler::alsNoteEvent, this, &chladniReceiver::onAlsNoteEvent );
	
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
			m.addStringArg(_value);
		}
		
		return sendOscMessage(m);
	}
	
	return false;
}

bool chladniReceiver::sendOscMessage(const string& _addr, const float& _value){
	
	if( bSenderIsConnected && !_addr.empty() ){
		ofxOscMessage m;
		m.setAddress(_addr);
		m.addFloatArg(_value);
		
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

// ALS Parser
void chladniReceiver::onAlsTrackEvent( alsTrackEventArgs &args ){
	//sendOscMessage("/km/chladni/trackChange", args.trackName );
	ofxOscMessage m;
	m.setAddress("/km/chladni/trackChange");
	m.addStringArg(args.trackName);
	m.addFloatArg(args.audioClip.duration);
	//cout << args.audioClip.time << " - " << args.trackName << endl;
	sendOscMessage(m);
}

void chladniReceiver::onAlsNoteEvent( alsNoteEventArgs &args ){
	
	ofxOscMessage m;
	m.setAddress("/km/chladni/noteEvent");
	m.addStringArg(args.clipName);
	m.addIntArg(args.note.key);
	
	sendOscMessage(m);
}

// GETTERS
//bool chladniReceiver::isEnabled() const {
//	return bEnabled;
//}


const static ::module::factory::moduleDependencies  chladniReceiverDependencies({"OSCRouter"});
MODULE_REGISTER( chladniReceiver , "chladniReceiver", chladniReceiverDependencies );
