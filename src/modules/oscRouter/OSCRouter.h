//
//  Created by Daan de Lange on 30/04/2016.
//
//	This class routes OSC messages (to ofEvents and other)
//  Routes register using a factory design pattern.
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "OSCNode.h"
#include "karmaModule.h"

#define KM_OSC_PORT_IN 12000
#define KM_SA_OSC_PORT_IN 12001

// based upon this model to register an OSC router
// http://gamedev.stackexchange.com/a/17759

class OSCRouter : public ofxOscReceiver, public karmaModule {
	friend class ofxImGui;
	
public:
	OSCRouter( );
	~OSCRouter();
	// fix for no default constructor
	OSCRouter& operator=( const OSCRouter& crap ) { return *this; }
	
	
	// virtual methods from karmaModule
	virtual bool enable();
	virtual bool disable();
	virtual void update(const animationParams& params);
	virtual void draw(const animationParams& params);
	virtual bool reset();
	virtual void showGuiWindow();
	virtual void drawMenuEntry();
	virtual bool saveToXML(ofxXmlSettings& xml) const;
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// virtual methods from ofxOscReceiver
	void ProcessMessage( const osc::ReceivedMessage &m, const osc::IpEndpointName& remoteEndpoint );
	
	// OSC Router methods
	bool addNode( OSCNode* _node );
	bool removeNode( OSCNode* _node );
	bool startOSC( int _port = KM_OSC_PORT_IN );
	void reconnectKMSA();
	
protected:
	// gui
	//ofxGuiGroup gui;
	//ofParameter<string> guiNumRoutes;
	//ofParameter<string> guiStatus;
	//ofParameter<bool> bGuiEnabled;
	list<OSCNode* > nodes;
	ofMutex OSCMutex;
	int OSCListeningPort;
	
private:
	
};


// GUI translations
#define GUIOSCRouterStatus		("Status")
#define GUIOSCRouterNumRoutes	("Nb Routes")
