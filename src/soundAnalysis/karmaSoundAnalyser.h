//
//  karmaSoundAnalyser.h
//  karmaMapper
//
//  Created by Daan de Lange on 25/5/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

// these files come from https://github.com/patriciogonzalezvivo/sNodes/tree/master/sNode-FFT/src
#include "fft.h"
#include "FFTOctaveAnalyzer.h"
//#include "ofSoundStream.h"

#define BUFFER_SIZE 512

class karmaSoundAnalyser {

public:

	karmaSoundAnalyser();
	~karmaSoundAnalyser();
	
	void setup();
	void update();
	void drawExample();
	
	void audioReceived( ofAudioEventArgs& Args );
	void _audioReceived( float * input, int bufferSize, int nChannels );
	
private:
    FFTOctaveAnalyzer FFTanalyzer;
    fft myfft;
    
    float left[BUFFER_SIZE];
    float right[BUFFER_SIZE];
    float magnitude[BUFFER_SIZE];
    float phase[BUFFER_SIZE];
    float power[BUFFER_SIZE];
    float freq[BUFFER_SIZE/2];
	float avg_power;
	
};
