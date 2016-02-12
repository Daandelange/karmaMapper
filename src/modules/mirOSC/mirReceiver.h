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
#include "OSCRouter.h"
//#include "OSCNode.h"
#include "ofEvents.h"
#include "mirEvents.h"
#include "singletonModule.h"

// this class handles OSC events comming from mirReceiver.pd, caches the data and calculate some more with it.
// also fires MIR events

class mirReceiver : public OSCNode, public singletonModule<mirReceiver> {
	
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
	
	// basic functions
	//bool startListening();
	//bool stopListening();
	bool connectToOSCRouter(){};
	
	// listeners
	//void oscIn();
	static ofEvent<mirBangEventArgs> mirBangEvent;
	static ofEvent<mirTempoEventArgs> mirTempoEvent;
	static ofEvent<mirOnSetEventArgs> mirOnSetEvent;
	
	// mir getter
	static mirData mirCache;
	
protected:
	
	// setting variables
	
	
	// analysis variables
	
	
	ofMutex oscMutex; // needed because audioIn() runs on a separate thread
	
private:
	
	
};


