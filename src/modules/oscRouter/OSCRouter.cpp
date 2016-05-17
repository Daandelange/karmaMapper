//
//  OSCRouter.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 30/05/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "OSCRouter.h"
#include "ofxImGui.h"

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
OSCRouter::OSCRouter() : singletonModule<OSCRouter>() {
	
	//if(!isSingleton) getInstance() = this;
	
	// init essential variables
	moduleType = "OSCRouter";
	moduleName = "OSCRouter";
	nodes.clear();
	bIsListening = false;
	
}

OSCRouter::~OSCRouter() {
	
	//ofScopedLock( oscMutex );
	bIsListening = false;
	
	disable();
	
	clearAllNodes();
}

// - - - - - - - -
// karmaModule methods
// - - - - - - - -
bool OSCRouter::enable(){
	bool ret = karmaModule::enable();
	
	// OSC
	ret *= startOSC();
	
	return ret;
}

bool OSCRouter::disable(){
	bool ret = karmaModule::disable();
	
	ret *= stopOSC();
	
	return ret;
}

void OSCRouter::draw(const animationParams &params){
	
}

void OSCRouter::update(const animationParams &params){
	
}

bool OSCRouter::reset(){
	disable();
        return true;
}

void OSCRouter::showGuiWindow(){
	if(!bShowGuiWindow) return;
	
	ImGui::SetNextWindowSize(ImVec2(400,ofGetHeight()*0.8));
	ImGui::Begin( ((string)"Module: ").append(getName()).append("###module-").append( ofToString(this) ).c_str() , &bShowGuiWindow );
	if( ImGui::InputInt("OSC Listening port", &OSCListeningPort , 1, 10, ImGuiInputTextFlags_EnterReturnsTrue)){
		startOSC(OSCListeningPort);
	}
	if( ImGui::Checkbox("OSC Listening", &bIsListening)){
		if (bIsListening) startOSC(OSCListeningPort);
		else stopOSC();
	}
	
	ImGui::Separator();
	ImGui::Text("Number of nodes: %lu", nodes.size() );
	
	if(nodes.size()>0){
		ImGui::Separator();
		ImGui::Separator();
		ImGui::TextWrapped("Connected OSC Nodes:");
		ImGui::Indent();
		
		for(auto n=nodes.begin(); n!=nodes.end(); ++n){
			if(ImGui::Button( ((string)("Unbind###node-"+ ofToString(&*n))).c_str())) {
				(*n)->detachNode();
				removeNode(*n);
				break;
			}
			ImGui::SameLine();
			ImGui::TextWrapped("%s (%s)", (*n)->getName().c_str(), ofToString(&*n).c_str());
		}
		ImGui::Unindent();
	}
	
	ImGui::End();
}

void OSCRouter::drawMenuEntry() {
	karmaModule::drawMenuEntry();
}

// writes the module data to XML. xml's cursor is already pushed into the right <module> tag.
bool OSCRouter::saveToXML(ofxXmlSettings& xml) const{
	bool ret = karmaModule::saveToXML(xml);
	
	xml.addValue("OSCListeningPort", OSCListeningPort);
	
	return ret;
}

// load module settings from xml
// xml's cursor is pushed to the root of the <module> tag to load
bool OSCRouter::loadFromXML(ofxXmlSettings& xml){
	
	bool ret=karmaModule::loadFromXML(xml);
	
	clearAllNodes();
	
	OSCListeningPort = xml.getValue("OSCListeningPort", KM_OSC_PORT_IN );
	
	//initialise(animationParams.params);
	
	return ret; // todo
}

// - - - - - - - -
// PARENT FUNCTION (ofxOscReceiver)
// - - - - - - - -
void OSCRouter::ProcessMessage(const osc::ReceivedMessage &m, const osc::IpEndpointName &remoteEndpoint){
	// first part is the same as parent class
	// convert the message to an ofxOscMessage
	
	if (!isEnabled()) return;
	// todo: we need a mutex here....
	
	// set the address
	ofxOscMessage ofMsg;
	ofMsg.setAddress( m.AddressPattern() );
	
	// set the sender ip/host
	char endpoint_host[ osc::IpEndpointName::ADDRESS_STRING_LENGTH ];
	remoteEndpoint.AddressAsString( endpoint_host );
	ofMsg.setRemoteEndpoint( endpoint_host, remoteEndpoint.port );
	
	// transfer the arguments
	for ( osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
		 arg != m.ArgumentsEnd();
		 ++arg ){
		if ( arg->IsInt32() )
			ofMsg.addIntArg( arg->AsInt32Unchecked() );
		else if ( arg->IsInt64() )
			ofMsg.addInt64Arg( arg->AsInt64Unchecked() );
		else if ( arg->IsFloat() )
			ofMsg.addFloatArg( arg->AsFloatUnchecked() );
		else if ( arg->IsString() )
			ofMsg.addStringArg( arg->AsStringUnchecked() );
		else if ( arg->IsBlob() ){
			const char * dataPtr;
			osc::osc_bundle_element_size_t len = 0;
			arg->AsBlobUnchecked((const void*&)dataPtr, len);
			ofBuffer buffer(dataPtr, len);
			ofMsg.addBlobArg( buffer );
		}
		else ofLogError("OSCRouter") << "ProcessMessage: argument in message " << m.AddressPattern() << " is not an int, float, or string";
	}
	
	bool handled = false;
	if( !nodes.empty() ){
		
		
		for(list<OSCNode*>::iterator it=nodes.begin(); it!=nodes.end(); it++){
			if((*it)->canHandle(ofMsg)){
				handled = (*it)->handle(ofMsg);
				//if(handled==true) return;
			}
		}
	}
	
	/* cycle trough arguments:
	 for(int i = 0; i < m.getNumArgs(); i++){
		// get the argument type
		msg_string += m.getArgTypeName(i);
		msg_string += ":";
		// display the argument - make sure we get the right type
		if(m.getArgType(i) == OFXOSC_TYPE_INT32){
	 msg_string += ofToString(m.getArgAsInt32(i));
		}
		else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
	 msg_string += ofToString(m.getArgAsFloat(i));
		}
		else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
	 msg_string += m.getArgAsString(i);
		}
		else{
	 msg_string += "unknown";
		}
	 }*/
	// notify all matching routes
	//ofLogNotice("OSCRouter") << "ProcessMessage: massage [" << ofMsg.getAddress() << "] with "<< ofMsg.getNumArgs() << "args."; //ofMessage.getArgName(0);
}

// - - - - - - - -
// OSCRouter FUNCTIONS
// - - - - - - - -
bool OSCRouter::addNode(OSCNode* _node){
	if( _node==NULL ) return false;
	
	ofScopedLock( oscMutex );
	if(!bInitialised) return false;
	
	// no duplicates
	if( std::find(nodes.begin(), nodes.end(), _node)!=nodes.end()){
		ofLogNotice("OSCRouter::addNode("+ ofToString(&*_node) +")") << "Node already exists, not adding.";
		return true;
	}
	
	nodes.push_back( _node );
	
	return true;
}

bool OSCRouter::removeNode(OSCNode* _node){
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

bool OSCRouter::clearAllNodes(){
	
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

bool OSCRouter::startOSC(int _port){
	
	if( !isEnabled() ) return false;
	
	// OSC
	try {
		ofxOscReceiver::setup( _port );
		bIsListening = true;
	} catch(const std::exception& e) {
		ofLogWarning("OSCRouter::startOSC") << "Could not connect: " << e.what() << endl;
		bIsListening = false;
		
		return false;
	}
	OSCListeningPort = _port;
	
	sender.setup(KM_SA_OSC_ADDR, KM_SA_OSC_PORT_IN);
	
	reconnectKMSA();
	
	return bIsListening;
}

bool OSCRouter::stopOSC(){
	
	if(!isEnabled()) return false;
	
	// OSC
	try {
		ofxOscReceiver::setup( -1 );
		bIsListening = false;
	} catch(const std::exception& e) {
		ofLogNotice("OSCRouter::stopOSC") << "Disconnect failed somehow... : " << e.what() << endl;
		bIsListening = false;
		return true;
	}
	
	// disconnect sender
	sender.setup(0, -1);
	
	// todo: tell kmsa KM stopped ?
	
	return !bIsListening;
}

// tries to let the KMSA know we're live now
// todo: should not be in OSCRouter.
void OSCRouter::reconnectKMSA(){
    ofxOscMessage m;
	m.setAddress("/km/reconnectKMSA");
	m.addTriggerArg();
	sender.sendMessage(m);
}

void OSCRouter::ImGuiShowOSCRouterConnectionTester() {
	if( ImGui::CollapsingHeader( "OSC Router Module", "ImGuiShowOSCRouterConnectionTester", true, false ) ){
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
	}
}

// register module type
const static ::module::factory::moduleDependencies  OSCRouterDependencies({});
MODULE_REGISTER( OSCRouter , "OSCRouter", OSCRouterDependencies );
