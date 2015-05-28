#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : controller(server,osc) {
	//controller = animationController(server, osc);
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
	
	// - - - - - - - - -
	//  EFFECTS INITIALISATION
	// - - - - - - - - -
	
	
	
	// start Ableton Liveset Decoder
	ofx::AbletonLiveSet::LiveSet LS;
	ofx::AbletonLiveSet::Parser parser(LS);
	
	if(!parser.open("vendome_daan_v1.0/mappingvendome.als")) ofLogNotice("ofApp::setup()", "Could not parse ALS file.");
	
	//liveSetEvents.enableMetronomEvents(LS);
	liveSetEvents.enableNoteEvents(LS);
	
	// tmp
	ofShowCursor();
	mouseHidden = false;
	
	// tmp
	//server.loadShapes("Vendome_Full_Small.xml");
	server.loadShapes("Vendome_1500_1200.xml");
	
	// sound analysis setup
	// streams to default system sound stream
	// then map it to a virtual mic ( soundflower , etc )
	//ofSoundStreamListDevices();
	//soundStream.setDeviceID(7); // 7 = soundflower 2 ch
	//soundStream.setup(this, 0, 2, 44100, 256, 4);
	//analyser.start();
	ofSoundStreamStop();
	
	// setup the OSC Router
	osc.start();
	
	// tmp
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
	
	// render shape editor ?
	if( editor->isInEditMode() ){
		//ofSetColor(255);
		editor->draw();
	}
	
	else{
		ofSetColor(255);
	}

}

void ofApp::exit(){
	if( !editor->isInEditMode() ){
		controller.stop();
	}
	
	ofSoundStreamStop();
	
	osc.stop();
	//analyser.stop();
}

//--------------------------------------------------------------
// Soon OF will extend audio possibilities and we'll be able to listen to audio events.
// for the moment we have to do it the dirty way (ofApp shouldn't handle this, karmaSoundAnalyser should)
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
	//if(analyser.isEnabled()) analyser.getInstance().audioIn( input, bufferSize, nChannels);
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
