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
	// settings:
	smoothingReactivity = .93f; // from 0 to 1
	
	//
	bEnabled = false;
	balance = 0.f;
	balanceSmoothed = 0.f;
	zeroCrossingDirection = 1.f;
	zeroCrossings.resize( round( 44100/SOUND_BUFFER_SIZE ) );
	spectrumVariation.resize( round( 44100/SOUND_BUFFER_SIZE ) );
	balanceScale=1.0f;
	
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

// - - - - - - - -
// EVENT LISTENERS
// - - - - - - - -
void karmaSoundAnalyser::audioIn(float *input, int bufferSize, int nChannels){
	
	ofScopedLock myLock(soundMutex);
	
	// samples are "interleaved"
	int numCounted = 0;
	
	int tmpZeroCrossings = 0;
	float tmpSpectrumVariation = 0;
	
	volumeLeft = 0;
	volumeRight = 0;
	volumeMono = 0;
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume

	for (int i = 0; i < SOUND_BUFFER_SIZE && i<bufferSize; i++){
		leftBuffer[i]		= input[i*2];
		rightBuffer[i]	= input[i*2+1]*0.5;
		
		volumeLeft += leftBuffer[i] * leftBuffer[i];
		volumeRight += rightBuffer[i] * rightBuffer[i];
		volumeMono += (volumeLeft + volumeRight)/2.0f;
		
		numCounted+=1;
		
		// calc zero crossings (yet have to make this a rate)
		float direction = ( (leftBuffer[i]+rightBuffer[i])/2.f > 0 )?1.f:-1.f;
		if( zeroCrossingDirection != direction ){
			tmpZeroCrossings += 1;
			direction *= -1.f;
		}
		
		// calc  spectrum variation
		if(i!=0){
			tmpSpectrumVariation += abs( leftBuffer[i-1] + leftBuffer[i]);
			tmpSpectrumVariation += abs( rightBuffer[i-1] + rightBuffer[i]);
		}
	}
	
	// zero crossing stuff
	for(int i=zeroCrossings.size(); i>=0; i--){
		if(i==0) zeroCrossings[0]=tmpZeroCrossings;
		else zeroCrossings[i] = zeroCrossings[i-1];
	}
	
	// spectrum variation stuff
	for(int i=spectrumVariation.size(); i>=0; i--){
		if(i==0) spectrumVariation[0]=tmpSpectrumVariation/2.0f;
		else spectrumVariation[i] = spectrumVariation[i-1];
	}
	
	//this is how we get the mean of rms :)
	volumeMono /= (float)(numCounted);
	volumeLeft /= (float)(numCounted);
	volumeRight /= (float)(numCounted);
	
	// this is how we get the root of rms :)
	volumeMono = sqrt( volumeMono );
	volumeLeft = sqrt( volumeLeft );
	volumeRight = sqrt( volumeRight );
	//volumeMono = (volumeLeft + volumeRight )/2.f;
	
	// keep smoothed volume
	volumeLeftSmoothed *= smoothingReactivity;
	volumeLeftSmoothed += (1.f-smoothingReactivity) * volumeLeft;
	
	volumeRightSmoothed *= smoothingReactivity;
	volumeRightSmoothed += (1.f-smoothingReactivity) * volumeRight;
	
	volumeMonoSmoothed *= smoothingReactivity;
	volumeMonoSmoothed += (1.f-smoothingReactivity) * volumeMono;
	
	// calc balance variables
	balance = volumeRight-volumeLeft;
	balanceSmoothed *= smoothingReactivity;
	balanceSmoothed += (1.f-smoothingReactivity) * balance;
	
	balanceScale *= .99; // reduce max scale
	if( abs(balance) > abs(balanceScale) ) balanceScale += balance*.2f; // adapt mac scale to current scale
	//if( (balanceScale < 0 && balanceSmoothed > 0) || (balanceScale > 0 && balanceSmoothed < 0) ) balanceScale*=-1;
	balanceScaled = balanceSmoothed/balanceScale;
	bool smoothedisLeft=balanceSmoothed<0;
	bool scaledisLeft=balanceScaled<0;
	if(smoothedisLeft!=scaledisLeft) balanceScaled*=-1.f;
}

// SETTERS
void karmaSoundAnalyser::setSmoothingReactivity(float _smoothingReactivity){
	smoothingReactivity = _smoothingReactivity;
	
}

// GETTERS
bool karmaSoundAnalyser::isEnabled() const {
	return bEnabled;
}

//
int karmaSoundAnalyser::getBufferSize() const {
	return SOUND_BUFFER_SIZE;
}

const float* karmaSoundAnalyser::getBufferLeft() const {
	return leftBuffer;
}

// scaled --> returns a more "useable" value
float karmaSoundAnalyser::getBalance(bool _getScaled) const{
	return _getScaled?balanceScaled:balanceSmoothed;
}

float karmaSoundAnalyser::getVolumeLeft(bool _getSmoothed) const{
	return _getSmoothed?volumeLeftSmoothed:volumeLeft;
}

float karmaSoundAnalyser::getVolumeRight(bool _getSmoothed) const {
	return _getSmoothed?volumeRightSmoothed:volumeRight;
}

float karmaSoundAnalyser::getVolumeMono(bool _getSmoothed) const {
	return _getSmoothed?volumeMonoSmoothed:volumeMono;
}

int karmaSoundAnalyser::getZeroCrossings() const {
	int sum(0);
	for(int i=0; i<zeroCrossings.size(); i++){
		sum += zeroCrossings[i];
	}
	return sum;
}

float karmaSoundAnalyser::getSpectrumVariation() {
	ofScopedLock myLock(soundMutex);
	int sum(0);
	for(int i=0; i<spectrumVariation.size(); i++){
		sum += spectrumVariation[i];
	}
	return sum;
}


