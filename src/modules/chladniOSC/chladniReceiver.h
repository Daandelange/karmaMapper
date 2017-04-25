//
//  chladniReceiver
//  karmaMapper
//
//  Created by Daan de Lange on 05/06/2016.
//
//
//

#pragma once

#include "ofMain.h"
#include "OSCNode.h"
#include "OSCRouter.h"
#include "ofEvents.h"
#include "chladniEvents.h"
#include "chladniRC.h"
#include "singletonModule.h"

// this class handles OSC events comming from a PureData chladni / cymatics wave generator and fires internal ofEvents
// handles OSC adresses prefixed with /chladni (name them like that in pd)

// problems compiling this? #MSVC_16 --> http://stackoverflow.com/a/5022140/58565

#define CHLADNI_PREFIX "/chladni/"
#define CHLADNI_PREFIX_LEN sizeof(CHLADNI_PREFIX)-1


struct oscParamsStruct {
	string host="localhost";
	int port = 12002;
};

class chladniReceiver : public OSCNode, public singletonModule<chladniReceiver> {
	
public:
	chladniReceiver( );
	~chladniReceiver();
	
	// prevents accidentally creating copies of your singleton
	chladniReceiver(chladniReceiver const&)     = delete;
	void operator=(chladniReceiver const&)  = delete;
	
	// virtuals from OSCNode
	virtual bool canHandle( const ofxOscMessage &_msg ) const;
	virtual bool handle( const ofxOscMessage &_msg );
	virtual void detachNode();
	
	// VIRTUALS FROM karmaModule
	virtual bool enable();
	virtual bool disable();
	virtual void update(const animationParams& params);
	virtual void draw(const animationParams& params);
	virtual bool reset();
	virtual void showGuiWindow();
	virtual void drawMenuEntry();
	virtual bool saveToXML(ofxXmlSettings& xml) const;
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// basic functions
	bool start();
	bool stop();
	
	// OSC sending stuff
	bool connectOSCSender();
	oscParamsStruct oscSendParams;
	bool sendOscMessage(ofxOscMessage& _msg);
	bool sendOscMessage(const string& _addr, const string& _value);
	bool sendOscMessage(const string& _addr, const float& _value);
	
	// listeners
	//void oscIn();
	static ofEvent<chladniBangEventArgs> chladniBangEvent;
	static ofEvent<chladniFloatEventArgs> chladniFloatEvent;
	
	// getters
	bool isEnabled() const;
	
protected:
	
	bool bEnabled;
	
	// communicate back with chladni
	ofxOscSender sender;
	bool bSenderIsConnected;
	
	// setting variables
	
	
	// analysis variables
	
	
private:
	
	
};


