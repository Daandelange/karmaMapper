//
//  fboRecorder.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 10/05/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "fboRecorder.h"


// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
fboRecorder::fboRecorder( ){
	bRecording = false;
	bFrameStarted = false;
}

fboRecorder::~fboRecorder(){
	stopRecording();
}

// - - - - - - - -
// Recording
// - - - - - - - -

// filename can be empty (will use timestamp)
// set _w and _h in px to use a huge off-screen framebuffer if you want
bool fboRecorder::startRecording(string _fileName, int _w, int _h){
	if(isRecording()) return;
	
	if( _w==0 || _h==0 ){
		_w = ofGetWindowWidth();
		_h - ofGetWindowHeight();
	}
	
	fbo.allocate(_w, _h, GL_RGB, 4); // 4 = anti-aliasing
	//fbo.getTexture();
	
	fbo.begin();
	ofClear(0,0,0);
	//ofEnableSmoothing(); // makes no difference
	fbo.end();
	
	// videoRecorder(ofFilePath::getAbsolutePath("ffmpeg")); // use this is you have ffmpeg installed in your data folder
	
	// run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
	ofxVideoRecorder::setVideoCodec("mpeg4");
	//ofxVideoRecorder::setVideoCodec("h264");
	ofxVideoRecorder::setVideoBitrate("30000k");
	
	//videoRecorder.setAudioCodec("mp3");
	//videoRecorder.setAudioBitrate("192k");
	
	// tmp
	string file(_fileName);
	if(file.compare("")==0) file = "videoRecorder"+ofGetTimestampString()+".mov";
	ofxVideoRecorder::setup(file, fbo.getWidth(), fbo.getHeight(), 60);

	bRecording = fbo.isAllocated();
	
	ofLogNotice("fboRecorder") << "Started recording to file «" << file << "»" << endl;
	
	 return bRecording;
}

bool fboRecorder::stopRecording(){
	ofxVideoRecorder::close();
	//ofxVideoRecorder::~ofxVideoRecorder();
	bRecording=false;
}

// - - - - - - - -
// Recording
// - - - - - - - -
bool fboRecorder::beginFrame(){
	if(!isRecording()) return false;
	
	if(!bFrameStarted){
		bFrameStarted=true;
		
		fbo.begin();
		//ofClear(0);
		
		/*/ tmp
		ofSetColor(255,0,0,100);
		ofFill();
		ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
		ofSetColor(0,0,255,100);
		ofNoFill();
		ofDrawLine(0,0,800,80);*/
	}
	
	return true;
}

bool fboRecorder::endFrame(bool _showBuffer){
	if(!isRecording()) return false;

	else if(!bFrameStarted) return false;
	
	fbo.end();
	//fbo.getTexture().getTextureData().bFlipTexture = false;
	bFrameStarted=false;
	if(_showBuffer) fbo.draw(0,0); // show recorded image
	
	ofPixels pix;
	pix.allocate(fbo.getWidth(),fbo.getHeight(), ofGetImageTypeFromGLType(GL_RGB));
	fbo.readToPixels(pix);
	
	//ofSaveImage(pix, "mahSupahRecoring.png", OF_IMAGE_QUALITY_BEST);
	ofxVideoRecorder::addFrame(pix);
	
	return true;
}

// GETTERS
bool fboRecorder::isRecording() const {
	return bRecording;
}
