//
//  chladniRC
//  karmaMapper
//
//  Created by Daan de Lange on 10/05/2015.
//
//
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

// this class sends OSC commands to chladni
// Thus allowing you to remote control it.

class chladniRC : public ofxOscSender {
	
public:
	chladniRC( );
	~chladniRC();
	
	// singleton stuff
	static chladniRC& getInstance(){
		static chladniRC instance; // Guaranteed to be destroyed and instantiated on first use
		return instance;
	}
	
	// prevents accidentally creating copies of your singleton
	chladniRC(chladniRC const&)     = delete;
	void operator=(chladniRC const&)  = delete;
	
	// basic functions
	bool setupOSC();
	
	// RC commands
	bool sendBang(const string& _name);
	bool sendPing();
	bool sendFloat(const string& _name, const float& _float);
	
	
	// getters
	bool isConnected() const;
	
protected:
	
	bool bConnected;
	
	// setting variables
	
	
	// analysis variables
	
	
	ofMutex oscMutex; // needed because audioIn() runs on a separate thread
	
private:
	
	
};


