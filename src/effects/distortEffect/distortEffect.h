//
//  distortEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"
#include "vertexShape.h"
#include "basicEffect.h"
#include "animationParams.h"
#include "mirReceiver.h"

struct animationParams;

class distortEffect : public basicEffect {
	
public:
	// constructors
	distortEffect();
	~distortEffect();
	
	// global effect functions
	bool initialise(const animationParams& params);
	bool render(const animationParams& params);
	void update(const animationParams& params);
	void reset();
	
	// #########
	// GUI STUFF
	virtual bool printCustomEffectGui();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// controller functions
	bool randomizePresets();
	
	virtual void tempoEventListener(mirTempoEventArgs &_args);
	
	
protected:
	bool bVariateInSeason;
	int seasonVariation; // season number
	//int
	bool bReactToBpm;
	int BPMMetronom;
	float BPMMagnitude;
	float BPMCurrentMagnitude;
	
	
private:
	
	
};

#define GUIDistortPanel "Distorsion Effect"