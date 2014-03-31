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
	
	// complete black background = nice start
	ofBackground(0);
}

//--------------------------------------------------------------
void karmaMapper::update(){
	rwi.update();
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
	pos += ofPoint(0, 20); // line height
		
	ofPopStyle();
	
	#endif // endif KARMA_DEBUG
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
			
		case 'a':
			rwi.addShape();//"vectorShape");
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
