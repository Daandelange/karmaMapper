#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : controller(server){
	controller = animationController(server);
}

ofApp::~ofApp(){
	
}

void ofApp::setup(){
	//fboHack.resize(1500,1200);
	// drawing environment
	ofEnableAlphaBlending();
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	
	// setup shapes
	//server = shapesServer();
	server.setup();
	
	editor=new shapesEditor(server);
	editor->enableEditMode();
	
	// load image
	//background.load("vendome_de_face.jpg");
	background.load("vendome_full.jpg");
	
	// prepare to record
    recordOutput = false;
	if(recordOutput){
		recorder.allocate((float)background.width, (float)background.height, GL_RGBA);
	}
	else if(recordOutputBis){
		recorder.allocate((float)background.width, (float)background.height, GL_RGBA);
		
		//saver.setCodecType(0); // Animation Codec
		//saver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_NORMAL);
		//saver.setup(ofGetWidth(), ofGetHeight(), "recording-" + ofToString(ofGetUnixTime()) + ".mov");
	}
	else {
		//background.resize(background.width*((float)ofGetHeight()/background.height), ofGetHeight());
	}
	
	// - - - - - - - - -
	//  EFFECTS INITIALISATION
	// - - - - - - - - -
	
	//sound.loadSound("TEST MIX V0.1.wav");
	
	// start liveset decoder
	ofx::AbletonLiveSet::LiveSet LS;
	ofx::AbletonLiveSet::Parser parser(LS);
	
	//if(!parser.open("mappingvendome.xml")) ofLogNotice("ofApp::setup()", "Could not parse ALS file.");
	
	//liveSetEvents.enableMetronomEvents(LS);
	//liveSetEvents.enableNoteEvents(LS);
	
	// tmp
	ofShowCursor();
	mouseHidden = false;
	
	// tmp
	server.loadShapes("Vendome_Full_Small.xml");
	
	// sound analysis setup
	// streams to default system sound stream
	// then map it to a virtual mic ( soundflower , etc )
	//ofSoundStreamListDevices();
	soundStream.setDeviceID(7); // 7 = soundflower 2 ch
	soundStream.setup(this, 0, 2, 44100, 256, 4);
	analyser.start();
	
	// play music
	//music.load("music.wav");
	//music.setLoop(true);
	//music.play();
}

//--------------------------------------------------------------
void ofApp::update(){
	
	// update shapes ?
	if( editor->isInEditMode() ){
		editor->update();
	}
	
	// display FPS in window title
	ofSetWindowTitle( ofToString(ofGetFrameRate(), 2)+" FPS");
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(recordOutput){
		recorder.begin();
		//glGenFramebuffersEXT(1, &recorder.getFbo());
		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, recorder.getFbo() );
	}
	else if(recordOutputBis){
		recorder.begin();
	}
	
	// render shape editor ?
	if( editor->isInEditMode() ){
		ofSetColor(255);
		background.draw(0,0);
		editor->draw();
	}
	// tmp, should become backgroundEffect
	else{
		ofSetColor(255);
		background.draw(0,0);
	}
	
	if(recordOutput){
	
		ofPixels pix;
		pix.allocate(1500,1200, GL_RGBA);
		recorder.readToPixels(pix);
        ofSaveImage(pix, "blabla.png", OF_IMAGE_QUALITY_BEST);
		
        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        recorder.end();
        
        recorder.draw(0,0);
	}
	
	if(recordOutputBis){
		//recordingImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		ofPixels pix;
		pix.allocate(1500,1200, GL_RGBA);
		recorder.readToPixels(pix);
		//saver.addFrame(pix);
	}
}

void ofApp::exit(){
	ofSoundStreamStop();
}

//--------------------------------------------------------------
// Soon OF will extend audio possibilities and we'll be able to listen to audio events.
// for the moment we have to do it the dirty way (ofApp shouldn't handle this, karmaSoundAnalyser should)
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
	analyser.getInstance().audioIn( input, bufferSize, nChannels);
}

void ofApp::keyPressed(int key){
	
	// toggle edit mode ?
	if(key=='e' || key=='E'){
		editor->switchEditMode();
		
		// spawn effects ?
		if( !editor->isInEditMode() ){
			controller.start();
		}
		// clear effects
		else {
			controller.stop();
		}
	}
	
	else if(key=='h' || key=='H'){
		mouseHidden ? ofShowCursor() : ofHideCursor();
		mouseHidden = !mouseHidden;
		
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//ofVec2f tmp(ofGetWidth(), ofGetHeight());
	//createShape( );
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
