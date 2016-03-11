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
#include "shaderToyVariables.h"
#include "karmaUtilities.h"

#define ShaderEffectDefaultFrag "defaultShader.frag"
#define ShaderEffectDefaultVert "defaultShader.vert"

struct animationParams;

// todo: add a GPU memory extraction feature for stats and handling no more GPU allocatable errors.

class shaderEffect : public basicEffect {
	
public:
	// constructors
	shaderEffect();
	~shaderEffect();
	
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
	
	bool loadShader(string _vert, string _frag);
	virtual void registerShaderVariables(const animationParams &params);
	void registerShaderToyVariables();
	void registerMirVariables();
	void setUseCustomFbo(const bool& _useCustomFbo);
	void setTextureMode( const int& _mode);
	
	virtual void onSetEventListener(mirOnSetEventArgs &_args);
	void onResizeListener( ofResizeEventArgs& resize );
	
protected:
	int onSetCalls;
	string vertexShader, fragmentShader;
	ofShader shader;
	ofFbo fbo; // tmp, until there's a layering system
	float fTimeFactor;
	
	shaderToyVariables shaderToyArgs;
	bool bUseShadertoyVariables;
	
	//shaderToyVariables shaderToyArgs;
	bool bUseMirVariables;
	bool bUseCustomFbo;
	bool bUseTextures;
	int textureMode; // note: nothing to do with GL texture modes
	float textureTransform[4]; // offset(x,y) scale(w,h)
	
	vector<ofTexture> textures;
	//vector<float> texturesTime;
	
private:
	
	
};

#define GUIShaderPanel "Shaders"
#define GUIShaderVariablesPanel "Shader Variables..."