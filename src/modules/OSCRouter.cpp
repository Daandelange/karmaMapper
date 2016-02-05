//
//  OSCRouter.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 30/05/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "OSCRouter.h"


// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
OSCRouter::OSCRouter(  ){
	// init essential variables
	bEnabled = false;
	nodes.clear();
}

OSCRouter::~OSCRouter(){
	// remove listeners
	ofRemoveListener( ofEvents().update , this, &OSCRouter::update );
	
	// inform nodes to detach listeners
	if(nodes.size() > 0) for( auto it=nodes.rbegin(); it!=nodes.rend(); it++){
		(*it)->detachNode();
		nodes.erase( std::next(it).base() );
	}
}
// - - - - - - - -
// PARENT FUNCTION (ofxOscReceiver)
// - - - - - - - -
void OSCRouter::ProcessMessage(const osc::ReceivedMessage &m, const osc::IpEndpointName &remoteEndpoint){
	// first part is the same as parent class
	// convert the message to an ofxOscMessage
	
	if(!bEnabled) return;
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
		else ofLogError("ofxOscReceiver") << "ProcessMessage: argument in message " << m.AddressPattern() << " is not an int, float, or string";
	}
	
	bool handled = false;
	if( !nodes.empty() ) for(list<OSCNode*>::iterator it=nodes.begin(); it!=nodes.end(); it++){
		if((*it)->canHandle(ofMsg)){
			handled = (*it)->handle(ofMsg);
			//if(handled==true) return;
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
// BASIC FUNCTIONS
// - - - - - - - -
bool OSCRouter::start(int _port){
	// listen
	ofAddListener( ofEvents().update , this, &OSCRouter::update );
	
	// OSC
	setup( _port );
	
	reconnectKMSA();
	
	bEnabled = true;
	return isEnabled()==true;
}

bool OSCRouter::stop(){
	bEnabled = false;
	ofRemoveListener( ofEvents().update , this, &OSCRouter::update );
	
	return isEnabled()==false;
}

bool OSCRouter::reset(){
	// how dirty is this ? :p
	this->~OSCRouter();
	OSCRouter();
}

bool OSCRouter::addNode(OSCNode* _node){
	if( _node==NULL ) return false;
	
	nodes.push_back( _node );
}

bool OSCRouter::removeNode(OSCNode* _node){
	if( _node==NULL ) return false;
	
	for(list<OSCNode* >::iterator it=nodes.begin(); it!=nodes.end(); it++){
		if( _node == *it ){
			(*it)->detachNode();
			nodes.erase( it );
			return true;
		}
	}
	return true; // guarantees the node doesnt exist
}

bool OSCRouter::isEnabled() const {
	return bEnabled;
}

// - - - - - - - -
// EVENT LISTENERS
// - - - - - - - -
void OSCRouter::update(ofEventArgs &event){
	if(!isEnabled()) return;
	
	// update GUI data
	//guiStatus = ofToString(isEnabled()?"Running":"Down");
	//guiNumRoutes = ofToString(nodes.size());
	
}

// tries to let the KMSA know we're live now
void OSCRouter::reconnectKMSA(){
	ofxOscSender sender;
	sender.setup("localhost", KM_SA_OSC_PORT_IN);
	ofxOscMessage m;
	m.setAddress("/km/reconnectKMSA");
	m.addTriggerArg();
	sender.sendMessage(m);
}
