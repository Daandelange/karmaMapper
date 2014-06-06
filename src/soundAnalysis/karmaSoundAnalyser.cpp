//
//  karmaSoundAnalyser.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 25/5/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "karmaSoundAnalyser.h"

// partially inspired from https://github.com/patriciogonzalezvivo/sNodes/tree/master/sNode-FFT

// MARK: Init

// todo: implement lib Aubio

karmaSoundAnalyser::karmaSoundAnalyser(){
	/*
	 note: for the moment OF seems to lack this callback
	 use instead: (ofApp::virtual function seems to work properly)
	 - - - -
	 ofApp::_audioReceived (float * input, int bufferSize, int nChannels){
		karmaSoundAnalyserInstance.audioReceived( input, bufferSize, nChannels);
	 }
	*/
	ofAddListener( ofEvents().audioReceived, this, &karmaSoundAnalyser::audioReceived );
}

karmaSoundAnalyser::~karmaSoundAnalyser(){
	ofRemoveListener( ofEvents().audioReceived, this, &karmaSoundAnalyser::audioReceived );
}

void karmaSoundAnalyser::setup() {
	//FFTanalyzer.setup(44100, BUFFER_SIZE/2, 2);
	FFTanalyzer.setup(44100, BUFFER_SIZE/4, 2); // 2nd arg = num EQ bands
	FFTanalyzer.peakHoldTime = 2; // hold longer
	FFTanalyzer.peakDecayRate = 0.8f; // decay slower
	FFTanalyzer.linearEQIntercept = 0.9f; // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope = 0.01f; // increasing gain at higher frequencies
	
	// samples are "interleaved"
	for (int i = 0; i < BUFFER_SIZE; i++){
		left[i] = 0;
		//right[i] = 0;
	}
    
    // 0 output channels,
	// 1 input channel
	// 44100 samples per second
	// BUFFER_SIZE samples per buffer
	// 4 num buffers (latency)
	ofSoundStreamSetup(0, 1, 44100, BUFFER_SIZE, 4);
}


// MARK: Loop

void karmaSoundAnalyser::update() {
	/* do the FFT	*/
	myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
	//myfft.inversePowerSpectrum(0,(int)BUFFER_SIZE/2,BUFFER_SIZE, left, &magnitude[0],&phase[0]);
	
	for (int i = 0; i < (int)(BUFFER_SIZE/2); i++){
		freq[i] = magnitude[i];
	}
	
	FFTanalyzer.calculate(freq);
	
}

// outputs an example visualisation/usage of the data:
void karmaSoundAnalyser::drawExample(){
	ofPushStyle();
	
	ofFill();
	
	int nBars = FFTanalyzer.nAverages;
	float barWidth = (float)ofGetWidth()/(float)nBars;
	
	for (int i = 0; i < nBars; i++){
		float valuePeak = ofMap(FFTanalyzer.peaks[i], 0, 40, 0, 1);
		float valueAver = ofMap(FFTanalyzer.averages[i], 0, 40, 0, 1);
		ofFloatColor color = ofFloatColor(1.0,0.4,0.0);
		color.setHue(0.5-valuePeak*0.5);
		ofSetColor(color);
		ofRect((i*barWidth),ofGetHeight(),barWidth,-valuePeak*ofGetHeight());
		ofSetColor(255, 100);
		ofRect((i*barWidth),ofGetHeight(),barWidth,-valueAver*ofGetHeight());
	}
	
	ofPopStyle();
}

// to transmit sound analysis data to shader
ofTexture karmaSoundAnalyser::generateDataImage(){
	int nBars = FFTanalyzer.nAverages;
	
	// the output image has the following structure:
	// - row1 contains the average FFT value for each buffer entry
	// - row2 contains the peak FFT value for each buffer entry
	// - row3 contains other synthetized sound analysis information (eg: a 7 band FFT EQ)
	ofPixels pixels;
	pixels.allocate(nBars, 3, OF_PIXELS_MONO);
	
	
	// push sound analysis data into the data image
	for (int i = 0; i < nBars; i++){
		// fill row 1 with avarage values
		pixels[ i*pixels.getNumChannels() ] = (unsigned char) ofMap(FFTanalyzer.peaks[i], 0, 40, 0, 255);
		
		// fill row 2 with peak values
		pixels[ i*pixels.getNumChannels() + nBars ] = (unsigned char) ofMap(FFTanalyzer.averages[i], 0, 40, 0, 255);
		
		// for the moment, leave row 3 empty
		pixels[ i*pixels.getNumChannels() + nBars*2 ] = (unsigned char) 0;
	}
	
	ofTexture out;
	out.allocate(BUFFER_SIZE, 3, GL_LUMINANCE);
	out.loadData(pixels, GL_LUMINANCE);
	
	return out;
	
}

void karmaSoundAnalyser::audioReceived( ofAudioEventArgs& Args ){
	// args: float *input, int bufferSize, int nChannels
	_audioReceived( Args.buffer, Args.bufferSize, Args.nChannels);
	
	/*/ samples are "interleaved"
	for (int i = 0; i < Args.bufferSize; i++){
		left[i] = Args.buffer[i*2];
		right[i] = Args.buffer[i*2+1];
	}*/
}

void karmaSoundAnalyser::_audioReceived(float *input, int bufferSize, int nChannels){
	
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*nChannels];
		//right[i] = input[i*nChannels+1];
	}
}