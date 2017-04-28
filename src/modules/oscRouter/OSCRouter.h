//
//  Created by Daan de Lange on 30/04/2016.
//
//	This class routes OSC messages (to ofEvents and other)
//  Routes register using a factory design pattern.
//

#pragma once

#include "ofMain.h"
#include "singletonModule.h"
#include "ofxOsc.h"
#include "OSCNode.h"

#define KM_OSC_PORT_IN 12000
#define KM_SA_OSC_PORT_IN 12001
#define KM_SA_OSC_ADDR "localhost"

// based upon this model to register an OSC router
// http://gamedev.stackexchange.com/a/17759

using namespace ofxImGui;

class OSCRouter : public singletonModule<OSCRouter>, public ofxOscReceiver {
	//friend class Gui;

public:
	OSCRouter();
	~OSCRouter();
	
	// fix for no default constructor
	//OSCRouter& operator=( const OSCRouter& crap ) { return *this; }
	
	// singleton stuff
//	static OSCRouter& getInstance(){
//		static OSCRouter instance(true); // Guaranteed to be destroyed and instantiated on first use
//		return instance;
//	}
	// prevents accidentally creating copies of your singleton
	//OSCRouter(OSCRouter const&)     = delete;
	//void operator=(OSCRouter const&)  = delete;
	
	
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
	bool clearAllNodes();
	bool startOSC( int _port = KM_OSC_PORT_IN );
	bool stopOSC();
	void reconnectKMSA();
	static void ImGuiShowOSCRouterConnectionTester();
	
protected:
	// gui
	//ofxGuiGroup gui;
	//ofParameter<string> guiNumRoutes;
	//ofParameter<string> guiStatus;
	//ofParameter<bool> bGuiEnabled;
	list<OSCNode* > nodes;
	ofMutex oscMutex;
	int OSCListeningPort;
	bool bIsListening;
	
	// todo: should not be here...
    ofxOscSender sender;
	
	
private:
	
	// prevents accidentally creating copies of your singleton
	OSCRouter(OSCRouter const&)     = delete;
	void operator=(OSCRouter const&)  = delete;
//	OSCRouter& operator=(const OSCRouter& other){
//		// always keep the singleton version
//		// todo: could be a better condition...
//		if (isSingleton ||  other.isSingleton) {
//			return getInstance();
//		}
//		else return *this;
//	}
	
	// called by getInstance()
	OSCRouter(bool _isSingleton);
};


// GUI translations
#define GUIOSCRouterStatus		("Status")
#define GUIOSCRouterNumRoutes	("Nb Routes")
