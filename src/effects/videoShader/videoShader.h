//
//  videoShader.h
//  karmaMapper
//
//  Created by Daan de Lange on 10/10/15.
//
//
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "shaderEffect.h"
#include "animationParams.h"
#include "mirReceiver.h"
#include "ofxFPS.h"

#ifdef TARGET_OSX
	#include "ofxUVC.h"
#endif

#ifdef KM_ENABLE_SYPHON
	#include "ofxSyphon.h"
#endif

#define WEBCAM_FPS_HISTORY_SIZE 120

struct animationParams;

// todo: make video decoding threaded
// todo: 

enum videoMode {
	// note: each mode must have a unique key, never replace the one of another
	VIDEO_MODE_FILE = 0, // read image from file
	VIDEO_MODE_SYPHON = 1, // read movie from file
	VIDEO_MODE_UVC_WEBCAM = 2,
};

struct ofxUVCCameraSetting {
	int vendorId, productId, interfaceNum;
	string name;
};

struct webcamSettingsStruct {
	int targetFPS = 30;
	int width = 800;
	int height = 600;
};

// Important: lock() when accessing player or bUseThreadedFileDecoding

class videoShader : public shaderEffect, public ofThread {
	
public:
	// constructors
	videoShader();
	~videoShader();
	
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
	
	// #########
	// videoEffect FUNCTIONS
	void setVideoMode(const enum videoMode& mode);
	bool loadVideoFile( const string &_path);
	bool selectUVCWebcam(string _cam="");
#ifdef KM_ENABLE_SYPHON
	bool connectToSyphonServer( const ofxSyphonServerDescription& _addr );
#endif
	void setUseThread( const bool& _useThread );
	
protected:
	videoMode videoMode;
	
	// VIDEO FILES
	float playBackSpeed;
	ofVideoPlayer player;
	string videoFile;
	bool bUseThreadedFileDecoding;
	ofThreadChannel<ofPixels> images_to_update;
	virtual void threadedFunction();
	
	// UVC webcam
	ofQTKitGrabber UVCWebcam;
	webcamSettingsStruct webcamSettings;
	ofxFps webcamFPSCounter;
	string activeCamera;
	float webcamFPSHistory[WEBCAM_FPS_HISTORY_SIZE];
	
#ifdef TARGET_OSX
	
	// UVC Control (mac only)
	ofxUVC UVCController;
	ofxUVCCameraSetting* selectedUVCControlCamera;
	list<ofxUVCCameraSetting> UVCUsbPresets;
	
	// Syphon
	#ifdef KM_ENABLE_SYPHON
		ofxSyphonClient syphonSource;
		ofxSyphonServerDescription syphonAddr;
		ofxSyphonServerDirectory dir;
//	void syphonServerAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
//	void syphonServerRetired(ofxSyphonServerDirectoryEventArgs &arg);
	#endif
	
#endif
	
private:
	
	
};

#define GUIvideoShaderPanel "Video Shader"