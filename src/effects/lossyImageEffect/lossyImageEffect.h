//
//  lossyImageEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 22/04/2016.
//
//
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "basicEffect.h"
#include "ofxTurboJpeg.h"
#include "animationController.h"
//#include "animationControllerEvents.h"


struct animationParams;

class lossyImageEffect : public basicEffect , public ofThread {
	
public:
	// constructors
	lossyImageEffect();
	~lossyImageEffect();
	
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
	void threadedFunction();
	
	// listeners
	void beforeDraw(  karmaFboLayerDrawEventArgs& _args );
	void afterDraw(  karmaFboLayerDrawEventArgs& _args );
	//void mouseReleased(ofMouseEventArgs& args);
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene);
	
	// controller functions
	bool randomizePresets();
	
protected:
	float fOpacity;
	float fJpegQuality;
	
private:
	bool bHasNewFrameThreaded;
	ofTexture compressedTexture;
	ofThreadChannel<ofPixels> toCompress;
	ofThreadChannel<ofPixels> receiveCompressed;
	
	tjhandle handleCompress;
	tjhandle handleDecompress;
	
};

#define GUIlossyImageEffectPanel "Texture Distorsion"