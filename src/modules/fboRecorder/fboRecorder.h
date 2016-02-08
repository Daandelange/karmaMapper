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

// Threaded FBO recording

class fboRecorder : ofxVideoRecorder {
	
public:
	fboRecorder( );
	~fboRecorder();
	
	// prevents accidentally creating copies of your singleton
	///fboRecorder(fboRecorder const&)     = delete;
	//void operator=(fboRecorder const&)  = delete;
	
	// Enablers
	bool startRecording(string _fileName="", int _w=0, int _h=0);
	bool stopRecording();
	ofTexture& getTexture();
	
	// Recording
	bool beginFrame();
	bool endFrame(bool _showBuffer=true);
	
	// getters
	bool isRecording() const;
	
protected:
	
	// recording
	bool bRecording = false;
	bool bFrameStarted = false;
	ofFbo fbo;
	//ofxVideoRecorder videoRecorder;
	
	//ofMutex oscMutex; // needed because audioIn() runs on a separate thread
	
private:
	
	
};


