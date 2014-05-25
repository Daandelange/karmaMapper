#include "karmaMapper.h"

#define KARMA_DEBUG true;

//--------------------------------------------------------------
void karmaMapper::setup(){
	// customize parameters
	
	
	// environment setup
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	//ofEnableAlphaBlending();
	
	rwi.setup();
	
	soundAnalyser.setup();
	
	// complete black background = nice start
	ofBackground(0);
	
	// SOUND STREAM FOR karmaSoundAnalyser
	//
	// 0 output channels,
	// 2 input channels
	// 44100 samples per second
	// BUFFER_SIZE samples per buffer
	// 4 num buffers (latency)
	//ofSoundStreamSetup(0,2,this, 44100, BUFFER_SIZE, 4);
}

//--------------------------------------------------------------
void karmaMapper::update(){
	rwi.update();
	soundAnalyser.update();
}

//--------------------------------------------------------------
void karmaMapper::draw(){
	ofBackground(0, 100);
	
	// apply effects
	rwi.draw();
	
	// show debugging info
	#ifdef KARMA_DEBUG
	
	ofPushStyle();
	//rectMode(); // todo
		
	ofPoint pos(10, 40);
		
	// print real-time framerate
	ofSetHexColor(0xFFFFFF);
	//ofSetColor(250);
	ofNoFill();
	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate())+"fps", pos);
	//pos += ofPoint(0, 20); // line height
		
	ofPopStyle();
	
	#endif // endif KARMA_DEBUG
	
	soundAnalyser.drawExample();
}

void karmaMapper::exit(){
	rwi.exit();
}

//--------------------------------------------------------------
void karmaMapper::keyPressed(int key){
	
	switch(key){
		
			
		case '[': // imageMapper animation mode
		case ']':
			// bla
			break;
			
		case 'e':
			rwi.isInEditMode = !rwi.isInEditMode;
			//rwi.editNextShape();
			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
			
		default:
			//if(debug) cout  << "Unrecognized key: " << key << endl;
			break;
	}
}

//--------------------------------------------------------------
void karmaMapper::keyReleased(int key){
	
}

//--------------------------------------------------------------
void karmaMapper::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void karmaMapper::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void karmaMapper::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void karmaMapper::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void karmaMapper::windowResized(int w, int h){

}

//--------------------------------------------------------------
void karmaMapper::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void karmaMapper::dragEvent(ofDragInfo dragInfo){ 

}

void karmaMapper::audioReceived (float * input, int bufferSize, int nChannels){
	// temp until ofEvents().audioReceived is bindable
	soundAnalyser._audioReceived( input, bufferSize, nChannels);
}