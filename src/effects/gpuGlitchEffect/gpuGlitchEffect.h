//
//  gpuGlitchEffect.h
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
#include "shaderEffect.h"
#include "karmaUtilities.h"

#define gpuGlitchEffectDefaultFrag "../shaderEffect/pingPongShader.frag"
#define gpuGlitchEffectDefaultVert "../shaderEffect/pingPongShader.vert"

struct animationParams;

// todo: add a GPU memory extraction feature for stats and handling no more GPU allocatable errors.

class gpuGlitchEffect : public basicEffect {
	
public:
	// constructors
	gpuGlitchEffect();
	~gpuGlitchEffect();
	
	// global effect functions
	bool initialise(const animationParams& params);
	bool render(karmaFboLayer& renderLayer, const animationParams& params);
	void update(karmaFboLayer& renderLayer, const animationParams& params);
	void reset();
	
	// #########
	// GPUGLITCHEFFECT METHODS
	void refreshGlitches();
	
	// #########
	// GUI STUFF
	virtual bool printCustomEffectGui();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene);
	
protected:
	ofFbo fbo;
	ofFbo::Settings fboSettings;
	int maxAllocations;
	
private:
	
	
};

#define GUIGpuGlitchPanel "GPU Glitch"
