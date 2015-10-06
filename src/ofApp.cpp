#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp(): controller( scene ) {
	//controller.setOSCServer(server, osc);
}

ofApp::~ofApp(){
	
}

void ofApp::setup(){

	ofSetEscapeQuitsApp(false);
	ofSetWindowTitle( "karmaMapper::Animator" );
	
	// drawing environment
	//ofEnableAlphaBlending();
	//ofEnableAntiAliasing();
	//ofEnableSmoothing();
	
	// setup scene
	//scene.loadLastUsedScene();
	//scene.loadScene("Vendome_1500_1200.xml");
	scene.loadScene("Vendome_Full_Small.xml");
	
	// enable animation controller
	controller.start();
	
	// start Ableton Liveset Decoder
	//ofx::AbletonLiveSet::LiveSet LS;
	//ofx::AbletonLiveSet::Parser parser(LS);
	//if(!parser.open("vendome_daan_v1.0/mappingvendome.als")) ofLogNotice("ofApp::setup()", "Could not parse ALS file.");
	
	//liveSetEvents.enableMetronomEvents(LS);
	//liveSetEvents.enableNoteEvents(LS);
	
	// tmp
	ofShowCursor();
	mouseHidden = false;

	
	// sound analysis setup
	// streams to default system sound stream
	// then map it to a virtual mic ( soundflower , etc )
	//ofSoundStreamListDevices();
	//soundStream.setDeviceID(7); // 7 = soundflower 2 ch
	//soundStream.setup(this, 0, 2, 44100, 256, 4);
	//analyser.start();
	ofSoundStreamStop();
	
	// setup the OSC Router
	//osc.start();
	
	// tmp
}

//--------------------------------------------------------------
void ofApp::update(){
	// display FPS in window title
	//ofSetWindowTitle( ofToString(ofGetFrameRate(), 2)+" FPS");
}

//--------------------------------------------------------------
void ofApp::draw(){
	
}

void ofApp::exit(){
	
	
	ofSoundStreamStop();
	
	//osc.stop();
	//analyser.stop();
}

//--------------------------------------------------------------
// Soon OF will extend audio possibilities and we'll be able to listen to audio events.
// for the moment we have to do it the dirty way (ofApp shouldn't handle this, karmaSoundAnalyser should)
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
	//if(analyser.isEnabled()) analyser.getInstance().audioIn( input, bufferSize, nChannels);
}

void ofApp::keyPressed(int key){
	if(key=='h' || key=='H'){
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
