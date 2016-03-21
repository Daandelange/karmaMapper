//
//  fboRecorder.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 10/05/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "fboRecorder.h"

// forward declarations are needed for event listening
//class animationController;
//ofEvent<karmaControllerDrawEventArgs> animationController::karmaControllerBeforeDraw;
//ofEvent<karmaControllerDrawEventArgs> animationController::karmaControllerAfterDraw;

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
fboRecorder::fboRecorder( ){
	
	// karmaModule setup
	bInitialised = true;
	bEnabled = false;
	bFlipVertical = false;
	moduleName = "fboRecorder";
	moduleType = "fboRecorder";
	
	bRecording = false;
	bFrameStarted = false;
	
	videoRecBitRate = KM_FBOREC_DEFAULT_BITRATE;
	videoRecAA = KM_FBOREC_DEFAULT_AA;
	videoRecFPS = KM_FBOREC_DEFAULT_FPS;
	videoRecShowOutput = true;
	fboRecMode = VIDEOREC_MODE_FILE_H264;
	useGrabScreen = false;
}

fboRecorder::~fboRecorder(){
	disable();
}

// - - - - - - - -
// VIRTUALS FROM karmaModule
// - - - - - - - -
bool fboRecorder::enable() {
	return karmaModule::enable();
}

bool fboRecorder::disable() {
	stopRecording();
	
	return karmaModule::disable();
}

void fboRecorder::update(const animationParams &params) {
	karmaModule::update(params);
}

void fboRecorder::draw(const animationParams &params) {
	karmaModule::draw(params);
}

void fboRecorder::showGuiWindow(){
	// this module has no GUI window
	return;
	//karmaModule::showGuiWindow();
}

void fboRecorder::drawMenuEntry(){
	ImGui::TextWrapped("Records the video output and saves it to a video file using a separate thread.");
	
	//ImGui::Selectable("Show Gui Window...", &bShowGuiWindow);
	
	if (ImGui::ListBoxHeader("Video Recording Mode", 3)){
		
		// manually add new modes here
		if ( ImGui::Selectable("VIDEOREC_MODE_FILE_H264", fboRecMode==VIDEOREC_MODE_FILE_H264)) {
			setRecordMode(VIDEOREC_MODE_FILE_H264);
		}
		if ( ImGui::Selectable("VIDEOREC_MODE_FILE_PNG", fboRecMode==VIDEOREC_MODE_FILE_PNG)) {
			setRecordMode(VIDEOREC_MODE_FILE_PNG);
		}
#ifdef KM_ENABLE_SYPHON
		if ( ImGui::Selectable("VIDEOREC_MODE_SYPHON", fboRecMode==VIDEOREC_MODE_SYPHON)) {
			setRecordMode(VIDEOREC_MODE_SYPHON);
		}
#endif
		ImGui::ListBoxFooter();
	}
	ImGui::Checkbox("Use grab screen instead of fbo", &useGrabScreen);
	ImGui::Checkbox("Flip Vertical", &bFlipVertical);
	ImGui::Checkbox("Show recorded output", &videoRecShowOutput);
	ImGui::Separator();
	
	if( isEnabled() ){
		if(!isRecording()){
			if(ImGui::Button("Start Recording")){
				startRecording();
			}
		}
		else {
			if(ImGui::Button("Stop Recording")){
				stopRecording();
			}
			if(fbo.isAllocated()){
				ImGui::TextWrapped("Recorded resolution: %gx%gpx", fbo.getWidth(), fbo.getHeight());
			}
		}
	}
	
	switch (fboRecMode) {
		case VIDEOREC_MODE_FILE_H264 :
		case VIDEOREC_MODE_FILE_PNG :
			ImGui::TextWrapped("Video File Settings");
			ImGui::InputInt("Bitrate", &videoRecBitRate);
			ImGui::InputInt("AA quality", &videoRecAA);
			ImGui::InputInt("Target video FPS", &videoRecFPS);
			break;
#ifdef KM_ENABLE_SYPHON
		case VIDEOREC_MODE_SYPHON :
			ImGui::TextWrapped("Syphon Settings (not yet)");
			break;
#endif
		default:
			break;
	}
	
	
}

// writes the module data to XML. xml's cursor is already pushed into the right <module> tag.
bool fboRecorder::saveToXML(ofxXmlSettings& xml) const{
	bool ret = karmaModule::saveToXML(xml);
	
	if(!ret) return false;
	
	xml.addValue("fboRecMode", static_cast<int>(fboRecMode));
	xml.addValue("videoRecFPS", (int) videoRecFPS);
	xml.addValue("videoRecAA", (int) videoRecAA);
	xml.addValue("videoRecBitRate", (int) videoRecBitRate);
	xml.addValue("videoRecShowOutput", videoRecShowOutput);
	xml.addValue("useGrabScreen", useGrabScreen);
	xml.addValue("bFlipVertical", bFlipVertical);
	
	return ret;
}

// load module settings from xml
// xml's cursor is pushed to the root of the <module> tag to load
bool fboRecorder::loadFromXML(ofxXmlSettings& xml){
	
	bool ret=karmaModule::loadFromXML(xml);
	if(!ret) return false;
	
	//OSCListeningPort = xml.getValue("OSCListeningPort", KM_OSC_PORT_IN );
	videoRecFPS = xml.getValue("videoRecFPS", KM_FBOREC_DEFAULT_FPS);
	videoRecAA = xml.getValue("videoRecAA", KM_FBOREC_DEFAULT_AA);
	videoRecBitRate = xml.getValue("videoRecBitRate", KM_FBOREC_DEFAULT_BITRATE);
	videoRecShowOutput = xml.getValue("videoRecShowOutput", true);
	bFlipVertical = xml.getValue("bFlipVertical", bFlipVertical);
	useGrabScreen = xml.getValue("useGrabScreen", false);
	setRecordMode (static_cast<enum videoRecMode>(xml.getValue("fboRecMode", VIDEOREC_MODE_FILE_H264)));
	
	//initialise(animationParams.params);
	
	return ret; // todo
}

// - - - - - - - -
// Recording
// - - - - - - - -

// filename can be empty (will use timestamp)
// set _w and _h in px to use a huge off-screen framebuffer if you want
bool fboRecorder::startRecording(string _fileName, int _w, int _h){
	if(isRecording()) return true;
	
	
	// prepare FBO
	if( _w==0 || _h==0 ){
		_w = ofGetWindowWidth();
		_h = ofGetWindowHeight();
	}
	fbo.allocate(_w, _h, GL_RGBA, videoRecAA);
	//ofSetWindowShape(_w, _h);
	
	//fbo.getTexture().setTextureWrap( GL_WRAP_BORDER, GL_WRAP_BORDER);
	
	fbo.begin(false);
	ofClear(0,0,0);
	//ofEnableSmoothing(); // makes no difference
	fbo.end();
	
	if(!fbo.isAllocated()){
		ofLogWarning("fboRecorder::startRecording()") << "Could not allocate FBO. Maybe your GPU can't handle any more.";
		return false;
	}
	
	// video mode dependent
	switch ( fboRecMode ){
		case VIDEOREC_MODE_FILE_H264 :
		case VIDEOREC_MODE_FILE_PNG : {
			fbo.getTexture().getTextureData().bFlipTexture = true;
			// videoRecorder(ofFilePath::getAbsolutePath("ffmpeg")); // use this is you have ffmpeg installed in your data folder
			
			// run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
			//ofxVideoRecorder::setVideoCodec("mpeg4");
			
			if(fboRecMode==VIDEOREC_MODE_FILE_PNG) {
				ofxVideoRecorder::setVideoCodec("png"); // heavy but very good results
			}
			else {
				ofxVideoRecorder::setVideoCodec("h264"); // good size, acceptable quality
			}
			ofxVideoRecorder::setVideoBitrate(ofToString(videoRecBitRate)+"k");
			
			//videoRecorder.setAudioCodec("mp3");
			//videoRecorder.setAudioBitrate("192k");
			
			string file(_fileName);
			if (file.compare("")==0){
				file = "videoRecorder"+ofGetTimestampString()+".mov";
			}
			
			ofxVideoRecorder::setup( ofToDataPath( file ), fbo.getWidth(), fbo.getHeight(), KM_FBOREC_DEFAULT_FPS);
			ofxVideoRecorder::start();
			
			bRecording = ofxVideoRecorder::isRecording();
			
			if( bRecording ) {
				ofLogNotice("fboRecorder::startRecording()") << "Started recording to file «" << file << "»";
			}
			
			break;
		}
#ifdef KM_ENABLE_SYPHON
		case VIDEOREC_MODE_SYPHON : {
			fbo.getTexture().getTextureData().bFlipTexture = false;
			fbo.begin();
			//ofDisableArbTex();
			fbo.end();
			syphonServer.setName("fboRecorder Output");
			bRecording = true;
			break;
		}
#endif
		default: {
			return false;
			break;
		}
	}
	

	ofAddListener( animationController::karmaControllerBeforeDraw, this, &fboRecorder::beforeDraw );
	ofAddListener( animationController::karmaControllerAfterDraw, this, &fboRecorder::afterDraw );
	
	return bRecording;
}

bool fboRecorder::stopRecording(){
	
	// video mode dependent
	switch ( fboRecMode ){
		case VIDEOREC_MODE_FILE_H264 :
		case VIDEOREC_MODE_FILE_PNG : {
			ofxVideoRecorder::close();
			break;
		}
#ifdef KM_ENABLE_SYPHON
		case VIDEOREC_MODE_SYPHON : {

			break;
		}
#endif
		default: {
			return false;
			break;
		}
	}
	
	//ofxVideoRecorder::~ofxVideoRecorder();
	fbo.clear(); // free memory

	bRecording=false;
	return true;
}

ofTexture& fboRecorder::getTexture(){
	return fbo.getTexture();
}

bool fboRecorder::setRecordMode(enum videoRecMode _mode){
	if(fboRecMode == _mode){
		return true;
	}
	switch ( _mode ){
		case VIDEOREC_MODE_FILE_H264 :
			fboRecMode = VIDEOREC_MODE_FILE_H264;
			break;
			
		case VIDEOREC_MODE_FILE_PNG :
			fboRecMode = VIDEOREC_MODE_FILE_PNG;
			break;
#ifdef KM_ENABLE_SYPHON
		case VIDEOREC_MODE_SYPHON :
			fboRecMode = VIDEOREC_MODE_SYPHON;
			break;
#endif
		default:
			return false;
			break;
	}
	
	return (fboRecMode == _mode);
}

// - - - - - - - -
// Recording
// - - - - - - - -
bool fboRecorder::beginFrame(){
	if(!isRecording()) return false;
	
	if( !useGrabScreen && !bFrameStarted){
		bFrameStarted=true;
		//ofGLProgrammableRenderer
		fbo.begin(true);
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
	
	if(!useGrabScreen){
		if(!bFrameStarted) return false;
		fbo.end();
		//fbo.getTexture().getTextureData().bFlipTexture = false;
		bFrameStarted=false;
	}
	
	static ofTexture tmpTex;
	int w = ofGetWidth();
	int h = ofGetHeight();
	if(!tmpTex.isAllocated()){
		tmpTex.allocate( w, h, GL_RGBA );
	}
	
	switch(fboRecMode){
		case VIDEOREC_MODE_FILE_H264 :
		case VIDEOREC_MODE_FILE_PNG : {
			ofPixels pix;
			pix.allocate(fbo.getWidth(),fbo.getHeight(), ofGetImageTypeFromGLType(GL_RGB));
			
			if(useGrabScreen){
				tmpTex.loadScreenData(0, 0, w, h);
				tmpTex.readToPixels(pix);
			}
			else {
				fbo.readToPixels(pix);
			}
			ofxVideoRecorder::addFrame(pix);
			
			break;
		}
			
#ifdef KM_ENABLE_SYPHON
		case VIDEOREC_MODE_SYPHON: {
			//fbo.updateTexture( fbo.getTexture().texData.textureID );
			if( useGrabScreen ){
				//syphonServer.publishScreen();
				tmpTex.loadScreenData(0, 0, ofGetWidth(), ofGetHeight());
				//tmpTex = fbo.getTexture();
				syphonServer.publishTexture( &tmpTex );
			}
			else {
				fbo.getTexture().getTextureData().bFlipTexture = bFlipVertical;
				//tmpTex = fbo.getTexture();
				syphonServer.publishTexture( &fbo.getTexture() );
			}
			
			break;
		}
#endif
			
		default:
			return false;
			break;
	}
	
	// flush
	tmpTex.clear();
	
	if(_showBuffer){
		if(!useGrabScreen){
#ifdef KM_ENABLE_SYPHON
			if( fboRecMode==VIDEOREC_MODE_SYPHON ){
#else
			if(false){
#endif
				fbo.draw(0, 0, fbo.getWidth(), fbo.getHeight()); // show recorded image
			}
			else {
				fbo.draw(0, fbo.getHeight(),fbo.getWidth(), -fbo.getHeight()); // show recorded image
			}
		}
	}
	
	return true;
}

// LISTENERS
void fboRecorder::beforeDraw(  karmaControllerDrawEventArgs& _args ){
	
	beginFrame();
}

void fboRecorder::afterDraw(  karmaControllerDrawEventArgs& _args ){
	
	endFrame(videoRecShowOutput);
}

// GETTERS
bool fboRecorder::isRecording() const {
	return bRecording;
}

// REGISTER MODULE
const static ::module::factory::moduleDependencies  fboRecorderDependencies({});
MODULE_REGISTER( fboRecorder , "fboRecorder", fboRecorderDependencies );
