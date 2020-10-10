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
#include "karmaParam.h"
#include "mirReceiver.h"
#include "ofxFps.h"

#ifdef TARGET_OSX
	#include "ofQTKitGrabber.h"
    #include "ofxUVC.h"
#endif

#ifdef KM_ENABLE_SYPHON
	#include "ofxSyphon.h"
#endif

#define WEBCAM_FPS_HISTORY_SIZE 120

struct animationParams;

// todo: make video decoding threaded
// todo:

enum videoModeEnum {
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
	
	bool UVCAutoFocus = false;
	float UVCFocus = 0.1f;
	bool UVCAutoWhiteBalance = false;
	float UVCWhiteBalance = 0.5f;
	bool UVCAutoExposure = false;
	float UVCExposure = 0.9f;
	
	float UVCContrast = 0.5f;
	float UVCSaturation = 0.5f;
	float UVCSharpness = 0.5f;
	float UVCGain = 0.5f;
	float UVCBrightness = 0.5f;
	
};

struct kmNewVideoFrameStruct {
	
	kmNewVideoFrameStruct(){
		//newPixels( ofPixels() );
	}
	
	kmNewVideoFrameStruct(ofPixels& _pix) : newPixels(_pix) {
		
	}
	
	ofPixels newPixels;
	float position = 0.f;
};

struct karmaVideoPlaybackSettingsStruct {
	
	bool isPlaying = false;
	bool isPaused = false;
	float seekerPosition = -1.f;
	float playbackSpeed = 1.f;
	float volume = 0.f;
	ofLoopType loopState = OF_LOOP_NORMAL;
	
};

// (todo)
struct karmaVideoMediaInformationStruct {
	
	// empty constructor
	karmaVideoMediaInformationStruct(){
		isVideoFile = false;
	}
	
	karmaVideoMediaInformationStruct( string fullPath ){
		ofFile tmpFile(fullPath);
		if(tmpFile && tmpFile.exists()){
			path = tmpFile.getEnclosingDirectory();
			fileName = tmpFile.getFileName();
			
			// todo: check if it really is ?
			isVideoFile = true;
		}
	}
	
	string path = "";
	string fileName = "";
	bool isVideoFile = false;
	struct karmaVideoPlaybackSettingsStruct playBackSettings;
	
	// todo:
	float mediaDuration = 0.f;
	int mediaDimensions[2] = {0,0};
	//float aspectRatio = 1.f; // width/height
	
	string getFullPath() const {
		if( path.length()>1 && path.compare(path.length()-1, 1, KM_DIRECTORY_SEPARATOR )==0 ){
			return path + fileName;
		}
		else {
			return path + KM_DIRECTORY_SEPARATOR + fileName;
		}
	}
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
	void ImguiShowTextureMode();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene);
	
	// controller functions
	bool randomizePresets();
	
	// #########
	// videoEffect FUNCTIONS
    void setVideoMode( const enum videoModeEnum& mode );
	bool loadVideoFile( const karmaVideoMediaInformationStruct &_media, bool forceRemember=false );
	bool selectUVCWebcam( string _cam="" );
#ifdef KM_ENABLE_SYPHON
	bool connectToSyphonServer( const ofxSyphonServerDescription& _addr );
#endif
	void setUseThread( const bool& _useThread );
	static void setVideoFilePlaybackInformation(ofVideoPlayer& _player, karmaVideoPlaybackSettingsStruct _settings);
	void startVideoMedia();
	void stopVideoMedia();
	void pauseVideoMedia(const bool& paused);
	void seekVideoMedia(const float& _pos);
	void setVideoMediaVolume(const float& _vol);
	void setVideoMediaSpeed(const float& _speed);
	
protected:
    videoModeEnum videoMode;
	
	// VIDEO FILES
	//float playBackSpeed;
	ofVideoPlayer player;
	ofVideoPlayer threadedPlayer;
	ofxFps videoPlayerFPSCounter;
	float videoPlayerFPSHistory[WEBCAM_FPS_HISTORY_SIZE];
    // tmp disabled for qt creator
    karmaThreadedParam<struct karmaVideoMediaInformationStruct> videoMedia;
    karmaThreadedParam<bool> bUseThreadedFileDecoding;
	ofThreadChannel<kmNewVideoFrameStruct> newImagesFromThread;
	virtual void threadedFunction();
	
    // UVC webcam
    webcamSettingsStruct webcamSettings;
    ofxFps webcamFPSCounter;
    string activeCamera;
    float webcamFPSHistory[WEBCAM_FPS_HISTORY_SIZE];

    // qtkit
#if defined( OF_VIDEO_CAPTURE_QTKIT )
 //   #pragma message "Using QTKIT..."
    ofQTKitGrabber UVCWebcam;
#else
    #pragma message "videoShader effect compiling without video capture features."
#endif

	
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
