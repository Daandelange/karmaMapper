//
//  fboEraser.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 10/10/15.
//
//  - - - -
//
//  Applies on vertexShape
//	Distorts it's vertexes

#include "fboEraser.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

fboEraser::fboEraser(){
	fboEraser::reset();
}

fboEraser::~fboEraser(){
	ofRemoveListener(mirReceiver::mirTempoEvent, this, &fboEraser::tempoEventListener);
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool fboEraser::initialise(const animationParams& params){
	// init values
	basicEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	reset();
	
	return bInitialised;
}

bool fboEraser::render(karmaFboLayer& renderLayer, const animationParams &params){
	if(!isReady()) return false;
	
	if( bInvert ){
		// render to the other fbo (use it as a pre-allocated cache fbo)
		renderLayer.begin(true);
		
		ofPushStyle();
		
		ofClearAlpha();
		ofFill();
		ofSetColor(0.0f, 255.0f );
		ofDrawRectangle(0,0, renderLayer.getWidth(), renderLayer.getHeight());
		
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		glBlendFunc(GL_ONE, GL_ONE);
		ofSetColor(0.0f, 255.0f );
		
		for(auto s=shapes.begin(); s!=shapes.end(); ++s){
			(*s)->sendToGPU();
		}
		
		ofPopStyle();
		
		renderLayer.end();
		renderLayer.getFBO().updateTexture( renderLayer.getFBO().getId() );
	}
	
	renderLayer.begin();
	
	// set drawing environment
	ofPushStyle();
	
	
	if( bClearAlways ){	// erase BG ?
		glEnable(GL_BLEND);
		//if(bInvert)
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		//else glBlendEquation(GL_FUNC_SUBTRACT);
		glBlendFunc(GL_ONE, GL_ONE);
		
		ofSetColor(0.0f, fClearAlwaysOpacity*255.0f );
		ofFill();
		
		if(shapes.size()==0){
			ofDrawRectangle(0,0, renderLayer.getWidth(), renderLayer.getHeight());
		}
		else {
			if(bInvert){
				renderLayer.getFBO().draw(0,0);
			}
			else {
				for(auto s=shapes.begin(); s!=shapes.end(); ++s){
					(*s)->sendToGPU();
				}
			}
		}
		
		glDisable(GL_BLEND);
	}
	
	// clear on MIR ?
	effectMutex.lock();
	if( bClearOnMir && fClearOnMirValue > 0 ){
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		//else glBlendEquation(GL_FUNC_SUBTRACT);
		glBlendFunc(GL_ONE, GL_ONE);
		
		ofSetColor(0.0f, fClearOnMirValue*255.0f );
		ofFill();
		
		if(shapes.size()==0){
			ofDrawRectangle(0,0, renderLayer.getWidth(), renderLayer.getHeight());
		}
		else {
			if(bInvert){
				renderLayer.getFBO().draw(0,0);
			}
			else {
				for(auto s=shapes.begin(); s!=shapes.end(); ++s){
					(*s)->sendToGPU();
				}
			}
		}
		
		glDisable(GL_BLEND);
	}
	effectMutex.unlock();
	
//	effectMutex.lock();
//	
//	effectMutex.unlock();
	
	ofPopStyle();
	
	// flush the pipeline! :D
	renderLayer.end(false);
	
	return true;
}

// updates shape data
void fboEraser::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	// do basic Effect function
	basicEffect::update( renderLayer, params );
	
	ofScopedLock(effectMutex);
	if(bClearOnMir && fClearOnMirValue > 0){
		// todo: makee this timed (not FPS dependent)
		fClearOnMirValue-=0.1*fClearOnMirOpacity;
	}
}

// resets all values
void fboEraser::reset(){
	basicEffect::reset();
	
	// effect type must match with class
	effectType = "fboEraser";
	
	bInvert = false;
	bClearAlways = true;
	fClearAlwaysOpacity = 1.f;
	bClearOnMir = false;
	fClearOnMirValue = 0;
	fClearOnMirOpacity = 1.f;
	
	// do stuff
	ofEnableAlphaBlending();
	
	ofRemoveListener(mirReceiver::mirTempoEvent, this, &fboEraser::tempoEventListener);
	ofAddListener(mirReceiver::mirTempoEvent, this, &fboEraser::tempoEventListener);
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
}

// this effect is ping-pong only.
// too: make it both
//bool fboEraser::setUsePingPong(const bool &_usePingpong){
//	bUsePingpong = true;
//}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool fboEraser::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUIfboEraserPanel ) ){
		
		ImGui::TextWrapped("Erases the background of the FBO (bottom layer). Can also be used as mask (top layer).");
		ImGui::TextWrapped("Uses shapes if bound to any shape, otherwise it will erase everything.");
		
		ImGui::Separator();
		ImGui::Separator();
		
		effectMutex.lock();
		ImGui::Checkbox("Inverted", &bInvert);
		effectMutex.unlock();
		
		ImGui::Separator();
		ImGui::Separator();
		
		ImGui::Checkbox("Clear Always", &bClearAlways);
		if(bClearAlways){
			ImGui::SameLine();
			ImGui::SliderFloat("Opacity", &fClearAlwaysOpacity, 0.f, 1.f);
		}
		effectMutex.lock();
		ImGui::Checkbox("Clear on Tempo", &bClearOnMir);
		if(bClearOnMir){
			ImGui::SameLine();
			ImGui::SliderFloat("Mir Opacity", &fClearOnMirOpacity, 0.f, 1.f);
		}
		effectMutex.unlock();
		
		ImGui::Separator();
		if(ImGui::Checkbox("Use Ping-Pong FBO", &bUsePingpong)){
			setUsePingPong(bUsePingpong);
		}
		
//		ImGui::Separator();
//		ImGui::Separator();
//		ImGui::TextWrapped("Controls");
//
//		ImGui::Separator();
	}
	
	return true;
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool fboEraser::saveToXML(ofxXmlSettings& xml) const{
	bool ret = basicEffect::saveToXML(xml);
	
	xml.addValue("clearInverted", bInvert);
	xml.addValue("clearAlways", bClearAlways);
	xml.addValue("clearAlwaysOpacity", fClearAlwaysOpacity);
	xml.addValue("clearOnMir", bClearOnMir);
	xml.addValue("clearOnMirOpacity", fClearOnMirOpacity);
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool fboEraser::loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene){
	bool ret = basicEffect::loadFromXML(xml, _scene);
	
	bInvert = xml.getValue("clearInverted", false);
	bClearAlways = xml.getValue("clearAlways", true);
	fClearAlwaysOpacity = xml.getValue("clearAlwaysOpacity", 1.0f);
	bClearOnMir = xml.getValue("clearOnMir", false);
	fClearOnMirOpacity = xml.getValue("clearOnMirOpacity", 1.0);
	
	return ret;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -


// - - - - -
// fboEraser Methods
// - - - - -
void fboEraser::tempoEventListener(mirTempoEventArgs &_args){
	ofScopedLock lock(effectMutex);
	
	if(!isReady()) return;
	
	if(_args.isTempoBis){
		fClearOnMirValue=fClearOnMirOpacity;
	}
}


// register effect type
EFFECT_REGISTER( fboEraser , "fboEraser" );
