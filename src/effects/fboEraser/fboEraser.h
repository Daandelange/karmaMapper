//
//  fboEraser.h
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

struct animationParams;

class fboEraser : public basicEffect {
	
public:
	// constructors
	fboEraser();
	~fboEraser();
	
	// global effect functions
	bool initialise(const animationParams& params);
	bool render(karmaFboLayer& renderLayer, const animationParams& params);
	void update(karmaFboLayer& renderLayer, const animationParams& params);
	void reset();
	//virtual bool setUsePingPong(const bool& _usePingpong);
	
	// #########
	// GUI STUFF
	virtual bool printCustomEffectGui();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	virtual void tempoEventListener(mirTempoEventArgs &_args);
	
protected:
	bool bInvert;
	bool bClearAlways;
	float fClearAlwaysOpacity;
	bool bClearOnMir;
	float fClearOnMirOpacity;
	float fClearOnMirValue;
	
private:
	
	
};

#define GUIfboEraserPanel "FBO Eraser"