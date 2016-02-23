//
//  fboRecorder
//  karmaMapper
//
//  Created by Daan de Lange on 10/05/2015.
//
//
//

#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"
#include "karmaModule.h"
#include "animationController.h"
#include "animationControllerEvents.h"

#ifdef KM_ENABLE_SYPHON
	#include "ofxSyphon.h"
#endif

// Threaded FBO recording

#define KM_FBOREC_DEFAULT_AA 4
#define KM_FBOREC_DEFAULT_FPS 60
#define KM_FBOREC_DEFAULT_BITRATE 40000

enum videoRecMode {
	VIDEOREC_MODE_FILE_PNG,
	VIDEOREC_MODE_FILE_H264,
#ifdef KM_ENABLE_SYPHON
	VIDEOREC_MODE_SYPHON
#endif
};

class fboRecorder : public ofxVideoRecorder, public karmaModule {
	
public:
	fboRecorder( );
	~fboRecorder();
	
	// prevents accidentally creating copies of your singleton
	///fboRecorder(fboRecorder const&)     = delete;
	//void operator=(fboRecorder const&)  = delete;
	
	// VIRTUALS FROM karmaModule
	virtual bool enable();
	virtual bool disable();
	virtual void update(const animationParams& params);
	virtual void draw(const animationParams& params);
	
	virtual void showGuiWindow();
	virtual void drawMenuEntry();
	
	virtual bool saveToXML(ofxXmlSettings& xml) const;
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// Enablers
	bool startRecording(string _fileName="", int _w=0, int _h=0);
	bool stopRecording();
	ofTexture& getTexture();
	bool setRecordMode(enum videoRecMode _mode);
	
	// Recording
	bool beginFrame();
	bool endFrame(bool _showBuffer=true);
	
	// listeners
	void beforeDraw( karmaControllerDrawEventArgs& _args );
	void afterDraw(  karmaControllerDrawEventArgs& _args );
	
	// getters
	bool isRecording() const;
	
protected:
	
	// recording
	bool bRecording = false;
	bool bFrameStarted = false;
	ofFbo fbo;
	//ofxVideoRecorder videoRecorder;
	
	//ofMutex oscMutex; // needed because audioIn() runs on a separate thread
	
	int videoRecBitRate;
	int videoRecFPS;
	int videoRecAA;
	bool videoRecShowOutput;
	enum videoRecMode fboRecMode;
	
	//string fboRecSyphonServerName;
	ofxSyphonServer syphonServer;
	
private:
	
	
	
};


