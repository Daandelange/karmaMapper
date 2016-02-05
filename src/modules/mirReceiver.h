//
//  mirReceiver
//  karmaMapper
//
//  Created by Daan de Lange on 06/05/2015.
//
//
//

#pragma once

#include "ofMain.h"
#include "OSCNode.h"
#include "ofEvents.h"
#include "mirEvents.h"

// this class handles OSC events comming from mirReceiver.pd, caches the data and calculate some more with it.
// also fires MIR events

class mirReceiver : public OSCNode {
	
public:
	mirReceiver( );
	~mirReceiver();
	
	// singleton stuff
	static mirReceiver& getInstance(){
		static mirReceiver instance; // Guaranteed to be destroyed and instantiated on first use
		return instance;
	}
	
	// prevents accidentally creating copies of your singleton
	mirReceiver(mirReceiver const&)     = delete;
	void operator=(mirReceiver const&)  = delete;
	
	// Parent functions
	bool canHandle( const ofxOscMessage &_msg ) const;
	bool handle( const ofxOscMessage &_msg );
	void detachNode();
	
	// basic functions
	bool start();
	bool stop();
	bool enable(const bool& _status);
	bool isEnabled() const;
	
	// listeners
	//void oscIn();
	static ofEvent<mirBangEventArgs> mirBangEvent;
	static ofEvent<mirTempoEventArgs> mirTempoEvent;
	static ofEvent<mirOnSetEventArgs> mirOnSetEvent;
	
	// mir getter
	static mirData mirCache;
	
protected:
	
	bool bInitialised;
	bool bEnabled;
	
	// setting variables
	
	
	// analysis variables
	
	
	ofMutex oscMutex; // needed because audioIn() runs on a separate thread
	
private:
	
	
};


