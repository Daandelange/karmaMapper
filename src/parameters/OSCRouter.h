//
//  Created by Daan de Lange on 30/04/2016.
//
//	This class routes OSC messages (to ofEvents and other)
//  Routes register using a factory design pattern.
//

#pragma once

#include "ofMain.h"
//#include "ofxUI.h"
#include "ofxGui.h"
#include "ofxGuiExtended.h"
#include "ofxOsc.h"
#include "OSCNode.h"

#define KM_OSC_PORT 12000

// based upon this model to register an OSC router
// http://gamedev.stackexchange.com/a/17759

class OSCRouter : public ofxOscReceiver {
	
public:
	OSCRouter( );
	~OSCRouter();
	// fix for no default constructor
	OSCRouter& operator=( const OSCRouter& crap ) { return *this; }
	
	// virtual (over-rided) functions
	void ProcessMessage( const osc::ReceivedMessage &m, const osc::IpEndpointName& remoteEndpoint );
	
	// basic functions
	bool start( int _port = KM_OSC_PORT );
	bool stop();
	bool reset();
	bool addNode( OSCNode* _node );
	bool removeNode( OSCNode* _node );
	bool isEnabled() const;
	
	// listeners
	void update( ofEventArgs& event );
	
protected:
	// gui
	//ofxGuiGroup gui;
	//ofParameter<string> guiNumRoutes;
	//ofParameter<string> guiStatus;
	//ofParameter<bool> bGuiEnabled;
	list<OSCNode* > nodes;
	
private:
	bool bEnabled;
	
};


// GUI translations
#define GUIOSCRouterStatus		("Status")
#define GUIOSCRouterNumRoutes	("Nb Routes")
