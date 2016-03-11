//
//  imageShader.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 06/02/2016.
//
//  - - - -
//
//  Uses a threaded function to load images from file(s)
//	Depends on ofThread and shaderEffect

#include "imageShader.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

imageShader::imageShader(){
	imageShader::reset();
}

imageShader::~imageShader(){
	
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool imageShader::initialise(const animationParams& params){
	// init values
	basicEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	imageShader::reset();
	
	return bInitialised;
}

bool imageShader::render(karmaFboLayer& renderLayer, const animationParams &params){
	
	if(!isReady()) return false;
	
	
	
	return true;
}

// updates shape data
void imageShader::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	// do basic Effect function
	basicEffect::update( renderLayer, params );
	

}

// resets all values
void imageShader::reset(){
	shaderEffect::reset();
	
	// effect type must match with class
	effectType = "imageShader";
	
//	ofRemoveListener(mirReceiver::mirTempoEvent, this, &imageShader::tempoEventListener);
//	ofAddListener(mirReceiver::mirTempoEvent, this, &imageShader::tempoEventListener);
	
	// reset texture ?
	//texture.
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool imageShader::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUIimageShaderPanel, "GUIimageShaderPanel", true, true ) ){
		
		ImGui::TextWrapped("Draws lines on shapes on the rythm of music.");
		
		ImGui::Separator();
		
		ImGui::LabelText("Number of lines", "%u", 0 );
		//ImGui::ColorEdit4("Color", linesColor, true);
		//ImGui::Checkbox("React to mirTempoEvents");
		//ImGui::SliderFloat("Line Duration (in beats)", &fLineBeatDuration, 1, 4);
		
		ImGui::Separator();
	}
        return true;
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool imageShader::saveToXML(ofxXmlSettings& xml) const{
	bool ret = basicEffect::saveToXML(xml);
	
//	xml.addTag("linesColor");
//	if(xml.pushTag("linesColor")){
//		xml.addValue("r", linesColor[0] );
//		xml.addValue("g", linesColor[1] );
//		xml.addValue("b", linesColor[2] );
//		xml.addValue("a", linesColor[3] );
//		xml.popTag();
//	}
//	xml.addValue("LineDuration", fLineBeatDuration);
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool imageShader::loadFromXML(ofxXmlSettings& xml){
	bool ret = shaderEffect::loadFromXML(xml);
	
	
	
	return ret;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool imageShader::randomizePresets(){
	if(!shaderEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}

// - - - - -
// imageShader Methods
// - - - - -
void imageShader::tempoEventListener(mirTempoEventArgs &_args){
	ofScopedLock lock(effectMutex);
	
	if(shapes.size()<=0) return;
	
	if(_args.isTempoBis) for(auto s=shapes.begin(); s!=shapes.end(); ++s){
		if( (*s)->isType("vertexShape") ){
			
		}
	}
}


// register effect type
EFFECT_REGISTER( imageShader , "imageShader" );
