//
//  lineDrawEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 10/10/15.
//
//
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "basicEffect.h"
#include "animationParams.h"
#include "mirReceiver.h"
#include "lineDrawEffectLine.h"

struct animationParams;

// todo : add statistics to effects. Track their rendering time so we have an overview of their rendering time.
// todo: add a GPU memory extraction feature for stats and handling no more GPU allocatable errors.

class lineDrawEffect : public basicEffect {
	
public:
	// constructors
	lineDrawEffect();
	~lineDrawEffect();
	
	// global effect functions
	bool initialise(const animationParams& params);
	bool render(karmaFboLayer& renderLayer, const animationParams& params);
	void update(karmaFboLayer& renderLayer, const animationParams& params);
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
	//ofFbo fbo; // for compatibility issues, we need a specific fbo object
	//float linesColor[4];
	list<lineDrawEffectLine> lines;
	
	float fLineBeatDuration;
	
	bool bStressTestMode;
	float fStressTestMultiplier;
	float fStressTestAddTolerance;
	float fStressTestRemoveTolerance;
	int fStressTestTargetFPS;
	
private:
	
	
};

#define GUILineDrawEffectPanel "Line Draw"