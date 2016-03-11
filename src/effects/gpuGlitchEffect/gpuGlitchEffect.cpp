//
//  gpuGlitchEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 10/10/15.
//
//  - - - -
//
//  Gets some nice glitched out of yout GPU!
//	Computer dreams! :D

#include "gpuGlitchEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

gpuGlitchEffect::gpuGlitchEffect(){
	effectType = "gpuGlitchEffect";
	gpuGlitchEffect::reset();
}

gpuGlitchEffect::~gpuGlitchEffect(){
	
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool gpuGlitchEffect::initialise(const animationParams& params){
	// init values
	shaderEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	onSetCalls = 0;
	fTimeFactor = 1.0f;
	bUseShadertoyVariables = false;
	bUseMirVariables = false;
	
	// do stuff
	ofEnableAlphaBlending();
	
	gpuGlitchEffect::reset();
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
	
	return bInitialised;
}

bool gpuGlitchEffect::render(karmaFboLayer& renderLayer, const animationParams &params){
	return true;
}

// updates shape data
void gpuGlitchEffect::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	shaderEffect::update( renderLayer, params );
	
	ofScopedLock lock(effectMutex);
	
	if( !isReady() ) return;
	
	
}

// resets all values
void gpuGlitchEffect::reset(){
	shaderEffect::reset();
	
	//ofScopedLock lock(effectMutex);
	effectMutex.lock();
	
	// effect type must match with class
	effectType = "gpuGlitchEffect";
	vertexShader = effectFolder(gpuGlitchEffectDefaultVert);
	fragmentShader = effectFolder(gpuGlitchEffectDefaultFrag);

	setUseCustomFbo(false);
	setUsePingPong(true);
	bUseShadertoyVariables = false;
	bUseMirVariables = false;
	bUseTextures = false;
	
	effectMutex.unlock();
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool gpuGlitchEffect::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUIGpuGlitchPanel, "GUIShaderPanel", true, false ) ){
		ImGui::TextWrapped("Gets some textures directly from your unallocated GPU memory. (computer dreams!)");
		
		ImGui::Separator();
		
		if(ImGui::Button("Refresh")){
			
		}
	}
	
	shaderEffect::printCustomEffectGui();
	
	return true;
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool gpuGlitchEffect::saveToXML(ofxXmlSettings& xml) const{
	
 	bool ret = shaderEffect::saveToXML(xml);
	
	
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool gpuGlitchEffect::loadFromXML(ofxXmlSettings& xml){
	bool ret = shaderEffect::loadFromXML(xml);
	
	loadShader( effectFolder(gpuGlitchEffectDefaultVert), effectFolder(gpuGlitchEffectDefaultFrag) );
	
	
	
	return shader.isLoaded();
}

// - - - - -
// gpuGlitchEffect Methods
// - - - - -



// register effect type
EFFECT_REGISTER( gpuGlitchEffect , "gpuGlitchEffect" );
