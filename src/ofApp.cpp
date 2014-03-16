#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// customize parameters
	debug=true;
	nbEqBands = 10;
	numTriangles = 5;
	
	// environment setup
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	//ofEnableAlphaBlending();
	
	// - - - -
	// FFT SETUP
	bufferSize = 512;//2048;
	fft.setup(bufferSize);
	equalizer.resize(nbEqBands);
	
	// spawn triangles
	for(int i=0; i<numTriangles; i++){
		illuminatiTriangle *tr;
		tr = new illuminatiTriangle();
		triangles.push_back( tr );
		triangles[i]->set( (float)(ofGetWidth()/(numTriangles+2)*(i+1)), (float)(ofGetHeight()/2), 30, 30 );
		//triangles[i].enabled = true;
	}
	
}

//--------------------------------------------------------------
void ofApp::update(){
	fft.update();
	analyseMusic();
	
	// todo or find other solution to transmit data from ofApp to basicEffect. Maybe use ofEvents ?
	//for(int i=0; i<numTriangles; i++){
	//	triangles[i]->updateData();
	//}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 100);
	
	// show debugging info
	if(debug){
		ofPushStyle();
		//rectMode(); // todo
		
		ofPoint pos(10, 40);
		
		// print real-time framerate
		ofSetHexColor(0xFFFFFF);
		//ofSetColor(250);
		ofNoFill();
		ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate())+"fps", pos);
		pos += ofPoint(0, 20); // line height
		
		// visualise equaliser
		ofFill();
		pos += ofPoint(0, 15); // margin
		ofPoint eqContainer(200, 60);
		ofPoint eqPart(eqContainer.x/nbEqBands, eqContainer.y);
		for(int i=0; i<nbEqBands;i++){
			float intensity = equalizer[i]*5;
			/// draw band
			float bandHeight (eqPart.y * intensity);
			if(bandHeight>eqPart.y) bandHeight=eqPart.y; // constrain
			
			ofRect(
				   pos.x + eqPart.x*i,
				   pos.y + eqPart.y - bandHeight,
				   eqPart.x,
				   bandHeight
			);
		}
		ofNoFill();
		ofLine(pos.x, pos.y+eqContainer.y, pos.x+eqContainer.x,pos.y+eqContainer.y);
		//pos += ofPoint(0, eqSize.y);
		
			
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	switch(key){
		illuminatiTriangle* t;
			
		case '[': // imageMapper animation mode
		case ']':
			for(int i=0; i<numTriangles; i++){
				t = triangles[i];
				t->imgAnim1.animSpeed += .1f -.2f*(key==']');
				t->imgAnim2.animSpeed += .1f -.2f*(key==']');
			}
			break;
			
		case 'o':
		case 'O':
			for(int i=0; i<numTriangles; i++){
				t = triangles[i];
				t->imgAnim1.hue += 1+20*(key=='O');
				t->imgAnim1.hue = t->imgAnim1.hue%255;
				
				t->imgAnim2.hue += 1+20*(key=='O');
				t->imgAnim2.hue = t->imgAnim1.hue%255;
			}
			break;
		
		case 'i':
			for(int i=0; i<numTriangles; i++){
				t = triangles[i];
				t->edgeTransitioner.addEdges();
			}
			break;
			
		case 'l':
			for(int i=0; i<numTriangles; i++){
				t = triangles[i];
				t->pEmitter.addParticles();
			}
			break;
			
		case 'd':
			debug = !debug;
			debug?ofShowCursor():ofHideCursor();
			break;
			
		case 'e':
			for(int i=0; i<numTriangles; i++){
				t = triangles[i];
				t->switchEditMode();
			}
			break;
			
		case 'u': // switch between images 1 and 2
			for(int i=0; i<numTriangles; i++){
				t = triangles[i];
				t->a1 = !t->a1;
			}
			break;
			
		case '1':
			triangles[0]->switchEditMode();
			break;
			
		case '2':
			triangles[1]->switchEditMode();
			break;
		
		case '3':
			triangles[2]->switchEditMode();
			break;
		
		case '4':
			triangles[3]->switchEditMode();
			break;
			
		case '5':
			triangles[4]->switchEditMode();
			break;
		
		case OF_KEY_SHIFT:
			shiftIsPressed = true;
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
void ofApp::keyReleased(int key){
	switch(key){
			
		case OF_KEY_SHIFT:
			shiftIsPressed = false;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
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

void ofApp::analyseMusic(){
	if(true) return; // temporarily not used so free CPU
	vector<float> buffer;
	buffer = fft.getBins();
	
	// param depending on eq bands
	int precision = 20;
	float eqBandZones[precision];
	for(int i=0; i<precision; i++){
		//eqBandZones[i]=(i*.3f)+log(i)*.7f; // maybe wong formula ?
	}
	
	// calc averages
	int n = buffer.size();
	vector<float> eq;
	vector<int> eqDivider;
	eq.resize(nbEqBands);
	eqDivider.resize(nbEqBands);
	for (int i = 0; i < n; i++) {
		// eq number is mapped to an exponential scale
		float linear_position = ofMap(eqBandZones[i], 0, n, 0, 1);
		float exponential_position = sin(linear_position*HALF_PI);
		
		// eq lookup
		int eqNum = 0;
		while( sin( ofMap(eqNum, 0, nbEqBands, 0, HALF_PI) ) < exponential_position && eqNum < nbEqBands ) eqNum++;
		
		eq[eqNum] += buffer[i]*.9 + .1f*ofMap( (i)^i, 0, n*2, 0, 50); // todo
		eqDivider[eqNum] ++;
	}
	
	// sum them up, keep average
	for (int i=0; i<nbEqBands; i++) equalizer[i] = eq[i]/eqDivider[i];
	
}
