//
//  karmaSoundAnalyser.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 17/03/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "karmaSoundAnalyser.h"

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
karmaSoundAnalyser::karmaSoundAnalyser( ){
	bEnabled = false;
	rawBalance = 0.f;
	reactivity = .93f; // from 0 to 1
	
	// this sets up streaming for ofApp::audioIn();
	// from there, call karmaSoundAnalyser::audioIn();
	// audio events are not yet bindable/listenable
	// soundStream.setup(ofBaseApp*) should be soundstream.setup(ofBaseSoundInput* )
	//soundStream.printDeviceList();
	//soundStream.setDeviceID(7); // 7 = soundflower 2 ch
	//soundStream.setup(0, 2, 44100, 256, 4);
	
}

karmaSoundAnalyser::~karmaSoundAnalyser(){
	
}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
bool karmaSoundAnalyser::start(){
	bEnabled = true;
	
	return isEnabled()==true;
}

bool karmaSoundAnalyser::stop(){
	
	
	
	bEnabled = false;
	return isEnabled()==false;
}

bool karmaSoundAnalyser::isEnabled() const {
	return bEnabled;
}

// - - - - - - - -
// EVENT LISTENERS
// - - - - - - - -
void karmaSoundAnalyser::audioIn(float *input, int bufferSize, int nChannels){
	
	// samples are "interleaved"
	int numCounted = 0;
	
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < SOUND_BUFFER_SIZE; i++){
		leftBuffer[i]		= input[i*2]*0.5;
		rightBuffer[i]	= input[i*2+1]*0.5;
		
		volumeLeft += leftBuffer[i] * leftBuffer[i];
		volumeRight += rightBuffer[i] * rightBuffer[i];
		volumeMono += volumeLeft + volumeRight;
		
		numCounted+=1;
	}
	
	//this is how we get the mean of rms :)
	volumeMono /= (float)(numCounted*2);
	volumeLeft /= (float)(numCounted);
	volumeRight /= (float)(numCounted);
	
	// this is how we get the root of rms :)
	volumeMono = sqrt( volumeMono );
	volumeLeft = sqrt( volumeLeft );
	volumeRight = sqrt( volumeRight );
	
	//
	//smoothedVol *= reactivity;
	//smoothedVol += (1.f-reactivity) * curVol;
	
}

