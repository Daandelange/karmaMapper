//
//  imageEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "basicEffect.h"
//#include "ofGLProgrammableRenderer.h"
#include "mirReceiver.h"
#include "durationReceiver.h" // todo: should be removed
//#include "mirEvents.h"

class imageEffect : public basicEffect {
	
public:
	imageEffect();
	~imageEffect();
	
	// global effect functions
	virtual bool initialise();
	virtual bool render();
	virtual void update();
	virtual void reset();
	
	bool loadImageFromFile(string _);
	bool listenToOscIntensity(string _OSCPath);
	
	//bool grabSomeShapes();
	
	// listeners
	void floatListener( durationFloatEventArgs &_args );
	//void noteEventListener(ofx::AbletonLiveSet::LSNoteEvent & noteEvent);
	//void tempoEventListener( mirTempoEventArgs &_args );
	void onSetEventListener( mirOnSetEventArgs &_args );
	
protected:
	
	// video stuff
	ofTexture cachedImage;
	ofShader imageShader;
	//ofMutex effectMutex;
	
	float opacity = 0;
	string listenToIntensity = "";
	
private:
	
};
