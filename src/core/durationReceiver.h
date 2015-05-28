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
#include "ofEvents.h"
#include "durationEvents.h"

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

class durationReceiver : public OSCNode {
	
public:
	durationReceiver( );
	~durationReceiver();
	
	// prevents accidentally creating copies of your singleton
	durationReceiver(durationReceiver const&)     = delete;
	void operator=(durationReceiver const&)  = delete;
	
	// Parent functions
	bool canHandle( const ofxOscMessage &_msg ) const;
	bool handle( const ofxOscMessage &_msg );
	void detachNode();
	
	// basic functions
	bool start();
	bool stop();
	
	// listeners
	//void oscIn();
	static ofEvent<durationBangEventArgs> durationBangEvent;
	static ofEvent<durationFlagEventArgs> durationFlagEvent;
	static ofEvent<durationFloatEventArgs> durationFloatEvent;
	
	// getters
	bool isEnabled() const;
	
protected:
	
	bool bEnabled;
	
	// setting variables
	
	
	// analysis variables
	
	
	ofMutex oscMutex; // needed because audioIn() runs on a separate thread
	
private:
	
	
};


