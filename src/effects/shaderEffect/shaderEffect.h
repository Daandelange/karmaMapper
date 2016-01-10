//
//  shaderEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 10/10/15.
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

#define ShaderEffectDefaultFrag "defaultShader.frag"
#define ShaderEffectDefaultVert "defaultShader.vert"

struct animationParams;

// todo : add statistics to effects. Track their rendering time so we have an overview of their rendering time.
// todo: add a GPU memory extraction feature for stats and handling no more GPU allocatable errors.

class shaderEffect : public basicEffect {
	
public:
	// constructors
	shaderEffect();
	~shaderEffect();
	
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
	
	bool loadShader(string _vert, string _frag);
	virtual void registerShaderVariables(const animationParams &params);
	virtual void onSetEventListener(mirOnSetEventArgs &_args);
	
protected:
	bool setShaderToyVariables; // todo
	int onSetCalls;
	string vertexShader, fragmentShader;
	ofShader shader;
	ofFbo fbo; // for compatibility issues, we need a specific fbo object
	
private:
	
	
};

#define GUIShaderPanel "Shaders"