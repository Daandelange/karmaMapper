//
//  imageShader.h
//  karmaMapper
//
//  Created by Daan de Lange on 06/02/2016.
//
//
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "shaderEffect.h"
#include "liveGrabberOSC.h"

struct animationParams;

class imageShader : public shaderEffect {
	
public:
	// constructors
	imageShader();
	~imageShader();
	
	// global effect functions
	bool initialise(const animationParams& params);
	bool render(karmaFboLayer& renderLayer, const animationParams& params);
	void update(karmaFboLayer& renderLayer, const animationParams& params);
	void reset();
	
	// #########
	// GUI STUFF
	virtual bool printCustomEffectGui();
	
	// #########
	// CUSTOM METHODS
	bool loadFromImage(string _imagePath);
	void ImguiShowTextureMode();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene);
	
	// controller functions
	bool disableSoonIsNow();
	bool randomizePresets();
	
	virtual void tempoEventListener(mirTempoEventArgs &_args);
	virtual void liveGrabberNoteEventListener(liveGrabberNoteEventArgs &_args);
	virtual void liveGrabberFloatEventListener(liveGrabberFloatEventArgs &_args);
	
protected:
	
	string imagePath;
	float musicalAlpha;
	float alphaReducer;
	bool bDrawAlways;
	bool bReactToMusic;
	//bool bUseThreadedFileDecoding;
	
	ofThreadChannel<float> alphaFromThread;
	
private:
	
	
};

#define GUIimageShaderPanel "Image Shader"