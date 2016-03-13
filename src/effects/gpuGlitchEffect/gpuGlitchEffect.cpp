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
	bool ret = basicEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	gpuGlitchEffect::reset();
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
	
	return bInitialised * ret;
}

bool gpuGlitchEffect::render(karmaFboLayer& renderLayer, const animationParams &params){
	if(!isReady()) return false;
	
	if(fbo.isAllocated()){
		
		renderLayer.begin();
		
		ofPushStyle();
		ofSetColor(mainColor[0]*255, mainColor[1]*255, mainColor[2]*255, mainColor[3]*255);
		ofFill();
		
		
		// bind the glitched fbo
		//fbo.getTexture().bind();
		fbo.getTexture().bind(); // todo, doesn't work... need to use a shader here ?
		
		// draw shape so GPU gets their vertex data
		for(auto it=shapes.begin(); it!=shapes.end(); ++it){
			(*it)->sendToGPU();
		}
		
		fbo.getTexture().unbind();
		
		ofPopStyle();
		
		//fbo.getTexture().setAlphaMask();
		fbo.getTexture().draw(0,0);
		
		renderLayer.end();
	}
	
	return true;
}

// updates shape data
void gpuGlitchEffect::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	basicEffect::update( renderLayer, params );
	
	//ofScopedLock lock(effectMutex);
	
	//if( !isReady() ) return;
	
	
}

// resets all values
void gpuGlitchEffect::reset(){
	basicEffect::reset();
	
	effectMutex.lock();
	
	setUsePingPong(false);
	
	fboSettings.width = ofGetWidth();
	fboSettings.height = ofGetHeight();
	fboSettings.internalformat = GL_RGBA;
	//fboSettings.numColorbuffers = 1;
	fboSettings.numSamples = ofFbo::maxSamples();
	
	effectMutex.unlock();
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool gpuGlitchEffect::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUIGpuGlitchPanel, "GUIGpuGlitchPanel", true, true ) ){
		ImGui::TextWrapped("Gets some textures directly from your unallocated GPU memory. (computer dreams!)");
		
		ImGui::Separator();
		
		if(ImGui::Button("Refresh")){
			fbo.clear();
			fbo.allocate(fboSettings);
		}
	}
	
	//parentEffect::printCustomEffectGui();
	
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool gpuGlitchEffect::saveToXML(ofxXmlSettings& xml) const{
	
 	bool ret = basicEffect::saveToXML(xml);
	
	
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool gpuGlitchEffect::loadFromXML(ofxXmlSettings& xml){
	bool ret = basicEffect::loadFromXML(xml);
	
	
	
	return ret;
}

// - - - - -
// gpuGlitchEffect Methods
// - - - - -



// register effect type
EFFECT_REGISTER( gpuGlitchEffect , "gpuGlitchEffect" );
