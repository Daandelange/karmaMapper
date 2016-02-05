//
//  durationRC
//  karmaMapper
//
//  Created by Daan de Lange on 10/05/2015.
//
//
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

// this class sends OSC commands to Duration
// Thus allowing you to remote control it.

class durationRC : public ofxOscSender {
	
public:
	durationRC( );
	~durationRC();
	
	// prevents accidentally creating copies of your singleton
	durationRC(durationRC const&)     = delete;
	void operator=(durationRC const&)  = delete;
	
	// basic functions
	bool setupOSC();
	
	// RC commands
	bool rewindAndPlay();
	bool play();
	bool stop();
	
	// getters
	bool isConnected() const;
	
protected:
	
	bool bConnected;
	
	// setting variables
	
	
	// analysis variables
	
	
	ofMutex oscMutex; // needed because audioIn() runs on a separate thread
	
private:
	
	
};


