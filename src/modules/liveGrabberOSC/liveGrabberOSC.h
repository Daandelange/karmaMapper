//
//  liveGrabberOSC
//  karmaMapper
//
//  Created by Daan de Lange on 24/03/2016.
//
//
//

#pragma once

#include "ofMain.h"
#include "OSCRouter.h"
#include "ofEvents.h"
#include "liveGrabberEvents.h"
#include "singletonModule.h"

// this class handles OSC events comming from liveGrabberOSC.pd, caches the data and calculate some more with it.
// also fires MIR events

class liveGrabberOSC : public OSCNode, public singletonModule<liveGrabberOSC> {
	
public:
	liveGrabberOSC( );
	~liveGrabberOSC();
	
	// singleton stuff
	static liveGrabberOSC& getInstance(){
		static liveGrabberOSC instance; // Guaranteed to be destroyed and instantiated on first use
		return instance;
	}
	
	// prevents accidentally creating copies of your singleton
	liveGrabberOSC(liveGrabberOSC const&)     = delete;
	void operator=(liveGrabberOSC const&)  = delete;
	
	// Parent functions from OSCNode
	bool canHandle( const ofxOscMessage &_msg ) const;
	bool handle( const ofxOscMessage &_msg );
	void detachNode();
	void notifyDetached();
	
	// VIRTUALS FROM karmaModule
	virtual bool enable();
	virtual bool disable();
	virtual void update(const animationParams& params);
	virtual void draw(const animationParams& params);
	virtual bool reset();
	void showGuiWindow();
	void drawMenuEntry();
	
	// basic functions
	//bool startListening();
	//bool stopListening();
	bool connectToOSCRouter(){return false;};
	
	// listeners
	//void oscIn();
	static ofEvent<liveGrabberBangEventArgs> liveGrabberBangEvent;
	static ofEvent<liveGrabberTempoEventArgs> liveGrabberTempoEvent;
	static ofEvent<liveGrabberNoteEventArgs> liveGrabberNoteEvent;
	
	// mir getter
	static liveGrabberAnalysisData liveGrabberAnalysis;
	
protected:
	
	// setting variables
	
	
	// analysis variables
	
	
	// Shared thread memory *beware of race conditions, use oscMutex*
	string lastReceivedParamName;
	
	ofMutex oscMutex; // needed because audioIn() runs on a separate thread
	
private:
	
	
};


