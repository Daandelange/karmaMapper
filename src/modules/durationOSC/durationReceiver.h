//
//  durationReceiver
//  karmaMapper
//
//  Created by Daan de Lange on 10/05/2015.
//
//
//

#pragma once

#include "ofMain.h"
#include "OSCNode.h"
#include "OSCRouter.h"
#include "ofEvents.h"
#include "durationEvents.h"
#include "singletonModule.h"

// this class handles OSC events comming from Duration and fires internal ofEvents
// handles OSC adresses prefixed with /duration (name them like that in duration)

// ideas:
// - enhance duration: add a receiver OF class
// - enhance duration: add an OSC prefix setting for all tracks

// todo:
// Use https://github.com/mailiam/ofxOscVariable/ to synchronise variables over osc

// problems compiling this? #MSVC_16 --> http://stackoverflow.com/a/5022140/58565
#define DURATION_TIMELINE_PREFIX "/dt/"
#define DT_PREFIX_LEN sizeof(DURATION_TIMELINE_PREFIX)-1

class durationReceiver : public OSCNode, public singletonModule<durationReceiver> {
	
public:
	durationReceiver( );
	~durationReceiver();
	
	// prevents accidentally creating copies of your singleton
	durationReceiver(durationReceiver const&)     = delete;
	void operator=(durationReceiver const&)  = delete;
	
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
	struct oscParams {
		string host="localhost";
		int port = 2345;
	} oscSendParams;
	bool sendOscMessage(ofxOscMessage& _msg);
	bool sendOscMessage(const string& _addr, const string& _value);
	
	// listeners
	//void oscIn();
	static ofEvent<durationBangEventArgs> durationBangEvent;
	static ofEvent<durationFlagEventArgs> durationFlagEvent;
	static ofEvent<durationFloatEventArgs> durationFloatEvent;
	
	// getters
	bool isEnabled() const;
	
protected:
	
	bool bEnabled;
	
	// communicate back with duration
	ofxOscSender sender;
	bool bSenderIsConnected;
	
	// setting variables
	
	
	// analysis variables
	
	
private:
	
	
};


