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
		
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		
		
		// bind the glitched fbo
		//fbo.getTexture().bind();
		//fbo.getTexture().bind(); // todo, doesn't work... need to use a shader here ?
		
		// draw shape so GPU gets their vertex data
		for(auto it=shapes.begin(); it!=shapes.end(); ++it){
			(*it)->sendToGPU();
		}
		ofDisableBlendMode();
		
		//fbo.getTexture().unbind();
		
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
	
	maxAllocations = 20;
	
	effectMutex.unlock();
}

// - - - - - - -
// GPUGLITCHEFFECT METHODS
// - - - - - - -
void gpuGlitchEffect::refreshGlitches(){
	fbo.clear();
	
	int cnt = 1+ofRandomuf()*maxAllocations;
	vector<ofFbo> tmpFbos(cnt);
	for( auto it=tmpFbos.begin(); it!=tmpFbos.end(); ++it){
		it->allocate(round(fbo.getWidth()*(0.5+ofRandomuf()/2.0) ), round(fbo.getHeight()*(0.5+ofRandomuf()/2.0) ));
	}
//	while(cnt >= 1){
//		
//		cnt --;
//	}
	
	fbo.allocate(fboSettings);
	
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool gpuGlitchEffect::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUIGpuGlitchPanel, "GUIGpuGlitchPanel", true, true ) ){
		ImGui::TextWrapped("Gets some textures directly from your previously allocated GPU memory. (computer dreams!)");
		
		ImGui::Separator();
		
		ImGui::DragInt("max allocations", &maxAllocations);
		ImGui::TextWrapped("More allocations give more chance to get more different glitches.");
		
		if(ImGui::Button("Refresh Glitches")){
			refreshGlitches();
		}
		if (ImGui::IsItemHovered()){
			ImGui::SetTooltip( "%s", "(click several times if not changing)" );
		}
		
		ImGui::TreePush("General GPU information:");
		ImGui::TextWrapped("maxSamples:\t %u", ofFbo::maxSamples() );
		ImGui::TextWrapped("maxColorAttachments:\t %u", ofFbo::maxColorAttachments() );
		ImGui::TextWrapped("maxDrawBuffers:\t %u", ofFbo::maxDrawBuffers() );
		
		ImGui::TreePop();
	}
	
	//parentEffect::printCustomEffectGui();
	return true;
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
