#include "karmaMapper.h"

//#define KARMA_DEBUG true;

// todo : pop effects handling out of this class and let an effectsControllerClass handle their renderings, transitions and so on.

//--------------------------------------------------------------
void karmaMapper::setup(){
	// customize parameters
	
	
	// environment setup
	ofBackground(34, 34, 34);
	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();

	//cout << "Shader loaded = " << shader.linkProgram() << endl;
	
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
	
	// populate effects with current setup
	if( rwi.shapeExists(0) ){
		shaderEffect* tmpEffect = new shaderEffect();
		tmpEffect->spawn();
		tmpEffect->bindWithShape( rwi.getShape(0) );
		tmpEffect->loadShader( "", "./shaders/menger_journey_ok.frag" );
		effects.push_back( tmpEffect );
	}
	if( false && rwi.shapeExists(0) ){
		videoEffect* tmpEffect = new videoEffect();
		tmpEffect->spawn();
		tmpEffect->bindWithShape( rwi.getShape(0) );
		tmpEffect->setDirectory("./videoEffect/videos");
		effects.push_back( tmpEffect );
	}
	if( false && rwi.shapeExists(1) ){
		gifEffect* tmpEffect = new gifEffect();
		tmpEffect->spawn();
		tmpEffect->bindWithShape( rwi.getShape(1) );
		tmpEffect->setDirectory("./gifEffect/gifs");
		effects.push_back( tmpEffect );
	}
	
	//shader.linkProgram();
	//cout << "Link status:" << shader.linkProgram() << endl;
	
}

//--------------------------------------------------------------
void karmaMapper::update(){
	rwi.update();
	soundAnalyser.update();
	
	if(!rwi.isInEditMode) for(int e=0; e<effects.size(); e++){
		// &soundAnalyser.generateDataImage().draw(0,0)
		effects[e]->update(  );
	}
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
	
	//soundAnalyser.drawExample();
	//soundAnalyser.generateDataImage().draw(0,0);
	
	// cache sound texture to pass it to the effects
	ofTexture soundTexture( soundAnalyser.generateDataImage() );
	
	// render effects	
	if(!rwi.isInEditMode) for(int e=0; e<effects.size(); e++){
		// &soundAnalyser.generateDataImage().draw(0,0)
		effects[e]->render(  );
	}
}

void karmaMapper::exit(){
	
	// todo: delete each effect (which are pointers)
	//for(){
		
	//}
	
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
			rwi.toggleEditMode();
			
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