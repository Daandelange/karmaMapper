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

struct animationParams;



enum textureMode {
	TEXTURE_MODE_MOVIE_STREAM, // read image from file
	TEXTURE_MODE_MOVIE_FILE // read movie from file
};

class imageShader : public shaderEffect, public ofThread {
	
public:
	// constructors
	imageShader();
	~imageShader();
	
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
	
	virtual void tempoEventListener(mirTempoEventArgs &_args);
	
protected:
	
	ofTexture texture;
	
	
private:
	
	
};

#define GUIimageShaderPanel "Line Draw"