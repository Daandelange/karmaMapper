//
//  serialControllerV1.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 08/05/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "serialController_v1.h"
#include "ofxImGui.h"

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
serialControllerV1::serialControllerV1() : singletonModule<serialControllerV1>() {
	
	// init essential variables
	moduleType = "serialControllerV1";
	moduleName = "serialControllerV1";
	
	serialControllerV1::reset();
	
}

serialControllerV1::~serialControllerV1() {
	
	//ofScopedLock( oscMutex );
	bIsConnected = false;
	
	outgoingMessages.close();
	incomingMessages.close();
	
	disable();
}

// - - - - - - - -
// karmaModule methods
// - - - - - - - -
bool serialControllerV1::enable(){
	//singletonModule<serialControllerV1>::enable();
	bool ret = karmaModule::enable();
	
	tryConnect(hardwareID);
	
	return ret;
}

bool serialControllerV1::disable(){
	bool ret = karmaModule::disable();
	
	disconnectDevice();
	
	return ret;
}

void serialControllerV1::draw(const animationParams &params){
	
}

void serialControllerV1::update(const animationParams &params){
	
	// get new serial messages from thread
	static karmaSerialMsg<string> message("","");
	while(incomingMessages.tryReceive(message)){
		availableMessages.push_back(message);
	}
}

bool serialControllerV1::reset(){
	disable();
	
	availableMessages.clear();
	
	bIsConnected = false;
	hardwareID = "";
	
	enable();
	
	return true;
}

void serialControllerV1::showGuiWindow(){
	if(!bShowGuiWindow) return;
	
	ImGui::SetNextWindowSize(ImVec2(400,ofGetHeight()*0.8));
	ImGui::Begin( ((string)"Module: ").append(getName()).append("###module-").append( ofToString(this) ).c_str() , &bShowGuiWindow );
	
	ImGui::TextWrapped("This module enables effects to easily do threaded serial communication with the karmaMapper Arduino Leonardo / Olimex hardware controller.");
	
	if (ImGui::CollapsingHeader( "Devices", "Devices", true, true)){
		
		if( ImGui::Checkbox("Connected to a device", &bIsConnected)){
			// click refreshes status
			bIsConnected = device.isOpen();
		}
		
		if(ImGui::Button("Disconnect")){
			disconnectDevice();
		}
		ImGui::SameLine();
		if(ImGui::Button("Reconnect")){
			if(!tryConnect()){
				ImGui::OpenPopup("sc_reconnect_failed");
			}
		}
		
		if(bIsConnected){
			ImGui::TextWrapped("Hardware ID: %s", hardwareID.c_str() );
			ImGui::TextWrapped("Port: %s", device.getPortName().c_str() );
			ImGui::Text("Thread running: %d", isThreadRunning());
		}
		
		if(ImGui::BeginPopup("sc_reconnect_failed")){
			ImGui::TextWrapped("Failed to connect... :(");
			if(ImGui::Button("Ok")){
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		
		static std::vector<ofx::IO::SerialDeviceInfo> availableDevices = ofx::IO::SerialDeviceUtils::listDevices();
		
		ImGui::Separator();
		ImGui::Separator();
		ImGui::ListBoxHeader("");
		for(auto d=availableDevices.begin(); d!=availableDevices.end(); ++d){
			if(ImGui::Selectable( (((hardwareID.compare((*d).getHardwareId())==0)?"[x] ":"[ ] ")+(*d).getDescription()+" ("+(*d).getPort()+")").c_str()) ){
				tryConnect((string)(*d).getHardwareId());
			}
		}
		ImGui::ListBoxFooter();
		ImGui::SameLine();
		if(ImGui::Button("Refresh")){
			availableDevices = ofx::IO::SerialDeviceUtils::listDevices();
		}
	}
	
	ImGui::Separator();
	ImGui::Separator();
	
	if (ImGui::CollapsingHeader( "Messages", "Messages", true, true)){
		
		if(ImGui::Button("Send Ping")){
			karmaSerialMsg<string> msg("ping", "From ImGui");
			outgoingMessages.send(std::move(msg));
		}
		
		//ImGui::Text("Number of queued outgoing messages: %lu", outgoingMessages.size() );
		ImGui::Text("Number of queued incoming messages: %lu", availableMessages.size() );
		
		ImGui::Separator();
		ImGui::Separator();
		
		ImGui::Text("Most recent messages: ");
		ImGui::Indent();
		ImGui::Columns(2);
		ImGui::Text("Address");
		ImGui::NextColumn();
		ImGui::Text("Value");
		ImGui::NextColumn();
		
		if(availableMessages.size()>0){
			int limit=20;
			for(auto m=availableMessages.rbegin(); m!=availableMessages.rend(); m++){
				
				ImGui::Selectable(m->getAddress().c_str());
				ImGui::NextColumn();
				ImGui::Selectable(m->getValue().c_str());
				ImGui::NextColumn();
				
				if(limit==0){
					break;
				}
				else{
					--limit;
				}
			}
		}
		else {
			ImGui::Text("[ None ]");
		}
		ImGui::Columns(1);
		ImGui::Unindent();
	}
	
	ImGui::Separator();
	ImGui::Separator();
	
//	if(nodes.size()>0){
//		ImGui::Separator();
//		ImGui::Separator();
//		ImGui::TextWrapped("Connected OSC Nodes:");
//		ImGui::Indent();
//		
//		for(auto n=nodes.begin(); n!=nodes.end(); ++n){
//			if(ImGui::Button( ((string)("Unbind###node-"+ ofToString(&*n))).c_str())) {
//				(*n)->detachNode();
//				removeNode(*n);
//				break;
//			}
//			ImGui::SameLine();
//			ImGui::TextWrapped("%s (%s)", (*n)->getName().c_str(), ofToString(&*n).c_str());
//		}
//		ImGui::Unindent();
//	}
	
	ImGui::End();
}

void serialControllerV1::drawMenuEntry() {
	karmaModule::drawMenuEntry();
}

// writes the module data to XML. xml's cursor is already pushed into the right <module> tag.
bool serialControllerV1::saveToXML(ofxXmlSettings& xml) const{
	bool ret = karmaModule::saveToXML(xml);
	
	//xml.addValue("OSCListeningPort", OSCListeningPort);
	xml.addValue("hwID", hardwareID);
	
	return ret;
}

// load module settings from xml
// xml's cursor is pushed to the root of the <module> tag to load
bool serialControllerV1::loadFromXML(ofxXmlSettings& xml){
	
	reset();
	
	bool ret=karmaModule::loadFromXML(xml);
	
	//OSCListeningPort = xml.getValue("OSCListeningPort", KM_OSC_PORT_IN );
	tryConnect(xml.getValue("hwID", hardwareID) );
	
	return ret;
}

// - - - - - - - -
// serialControllerV1 FUNCTIONS
// - - - - - - - -
bool serialControllerV1::tryConnect(string _hwID){
	
	std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
	
#ifdef KM_DEBUG
	ofLogVerbose("serialControllerV1::tryConnect") << "Connected Devices: ";
	
	for (std::size_t i = 0; i < devicesInfo.size(); ++i)
	{
		ofLogVerbose("serialControllerV1::tryConnect") << "\t" << devicesInfo[i];
	}
#endif
	
	// already connected ?
	if(bIsConnected){
		if(hardwareID.compare(_hwID)==0){
			
			if(!isThreadRunning()){
				startThread();
			}
			
			bIsConnected = true;
			
			// already connected
			return true;
		}
		
		// disconnect from device
		disconnectDevice();
	}
	
	if (!devicesInfo.empty()){
		unsigned int deviceID = 0;
		
		// try to match device name
		for (std::size_t i = 0; i < devicesInfo.size(); ++i){
			//ofLogVerbose("serialControllerV1::tryConnect") << "\t" << devicesInfo[i];
			if(devicesInfo[i].getHardwareId().compare(_hwID)==0){
				deviceID = i;
				break;
			}
		}
		
		// Connect to it
		bool success = false;
		try {
			success = device.setup(devicesInfo[deviceID], KM_SERIAL_BAUD_RATE);
		}
		catch (const serial::IOException& exc){
			ofLogNotice("serialControllerV1::tryConnect()") << "Could not connect to device ["<< devicesInfo[deviceID].getDescription() <<" / "<< _hwID <<"] : " << exc.what();
		}
		
		if(success){
			bIsConnected = true;
			hardwareID = devicesInfo[deviceID].getHardwareId();
			
			device.registerAllEvents(this);
			if(!isThreadRunning()) startThread();
			
			// tmp
			karmaSerialMsg<string> msg("ping", "karmaMapper connected");
			outgoingMessages.send(std::move(msg));
			
			ofLogNotice("serialControllerV1::tryConnect") << "Successfully setup " << devicesInfo[deviceID].getDescription() << "(device id=" << deviceID << ")";
			
			return true;
		}
		else{
			bIsConnected = false;
			ofLogNotice("serialControllerV1::tryConnect") << "Unable to setup " << devicesInfo[0];
		}
	}
	else
	{
		ofLogNotice("serialControllerV1::tryConnect") << "No devices connected.";
	}
	return false;
}

bool serialControllerV1::disconnectDevice(){
	device.unregisterAllEvents(this);
	
	if(device.isOpen()){
		// disconnects
		device.setup("");
	}
	
	// strop threads
	//if(isThreadRunning()) waitForThread(true);
	
	bIsConnected = false;
        return bIsConnected;
}

void serialControllerV1::threadedFunction(){
	
	static karmaSerialMsg<string> tmpMessage("","");
	while(outgoingMessages.receive(tmpMessage)){
		
		ofx::IO::ByteBuffer textBuffer(tmpMessage.getAddress()+":"+tmpMessage.getValue());
		
		device.send(textBuffer);
	}
}

void serialControllerV1::onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args){
	// Buffers will show up here when the marker character is found.
	karmaSerialMsg<string> message("addr", args.getBuffer().toString());
	incomingMessages.send( std::move(message) );
	//cout << "onSerialBuffer! : " << message.getValue() << endl;
}

void serialControllerV1::onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args)
{
	// Errors and their corresponding buffer (if any) will show up here.
	//SerialMessage message(args.getBuffer().toString(),
						  //args.getException().displayText(),
						 // 500);
	//serialMessages.push_back(message);
}

/*
bool serialControllerV1::addNode(OSCNode* _node){
	if( _node==NULL ) return false;
	
	ofScopedLock( oscMutex );
	if(!bInitialised) return false;
	
	// no duplicates
	if( std::find(nodes.begin(), nodes.end(), _node)!=nodes.end()){
		ofLogNotice("serialControllerV1::addNode("+ ofToString(&*_node) +")") << "Node already exists, not adding.";
		return true;
	}
	
	nodes.push_back( _node );
	
	return true;
}

bool serialControllerV1::removeNode(OSCNode* _node){
	if( _node==NULL ) return false;
	
	ofScopedLock( oscMutex );
	for(list<OSCNode* >::iterator it=nodes.begin(); it!=nodes.end(); it++){
		if( _node == *it ){
			(*it)->notifyDetached();
			nodes.erase( it );
			return true;
		}
	}
	return true; // guarantees the node doesnt exist
}

bool serialControllerV1::clearAllNodes(){
	
	ofScopedLock( oscMutex );

	// inform nodes to detach listeners
//	if(nodes.size() > 0){
//		for( auto it=nodes.rbegin(); it!=nodes.rend(); ++it){
//			cout << &*it << endl;
//			(*it)->detachNode();
//			nodes.erase( std::next(it).base() );
//		}
//	}
	if(nodes.size() > 0){
		while(nodes.size() > 0){
			auto n = nodes.begin();
			(*n)->detachNode();
			nodes.erase( n );
		}
	}
	
	return nodes.size()==0;
}

// tries to let the KMSA know we're live now
void serialControllerV1::reconnectKMSA(){
    sender.setup(KM_SA_OSC_ADDR, KM_SA_OSC_PORT_IN);
    ofxOscMessage m;
	m.setAddress("/km/reconnectKMSA");
	m.addTriggerArg();
	sender.sendMessage(m);
}

void serialControllerV1::ImGuiShowserialControllerV1ConnectionTester() {
	
	static bool serialControllerV1Connection = false;
	if( ImGui::Button("Test connection with OSC Router" )){
		serialControllerV1Connection = serialControllerV1::getInstance().isEnabled();
		ImGui::OpenPopup("serialControllerV1ConnectionResult");
	}
	if( ImGui::BeginPopup("serialControllerV1ConnectionResult") ){
		ImGui::SameLine();
		
		if (serialControllerV1Connection){
			ImGui::Text("Up & Running! :)");
		}
		else {
			ImGui::Text("Error... Please check if the serialControllerV1 module is enabled.");
		}
		if(ImGui::Button("Ok")){
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::EndPopup();
	}
	
}
 */

// register module type
const static ::module::factory::moduleDependencies  serialControllerV1Dependencies({});
MODULE_REGISTER( serialControllerV1 , "serialControllerV1", serialControllerV1Dependencies );
