#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : controller(server){
	controller = animationController(server);
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
		background.resize(background.width*((float)ofGetHeight()/background.height), ofGetHeight());
	}
	
	// - - - - - - - - -
	//  EFFECTS INITIALISATION
	// - - - - - - - - -
	
	sound.loadSound("TEST MIX V0.1.wav");
	
	// start liveset decoder
	ofx::AbletonLiveSet::LiveSet LS;
	ofx::AbletonLiveSet::Parser parser(LS);
	
	if(!parser.open("mappingvendome.xml")) ofLogNotice("ofApp::setup()", "Could not parse ALS file.");
	
	liveSetEvents.enableMetronomEvents(LS);
	liveSetEvents.enableNoteEvents(LS);
	//sound.play();
	
	ofShowCursor();
	mouseHidden = false;
	
	server.loadShapes("Vendome_Full.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
	// update shapes ?
	if( editor->isInEditMode() ){
		editor->update();
	}
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
	
	ofClear(0);
	
	ofSetColor(255);
	//background.draw(0,0);
	
	// render shape editor ?
	if( editor->isInEditMode() ){
		background.draw(0,0);
		editor->draw();
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

//--------------------------------------------------------------
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