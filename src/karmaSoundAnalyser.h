//
//  karmaSoundAnalyser.h
//  karmaMapper
//
//  Created by Daan de Lange on 17/03/2015.
//
//
//

#pragma once

#include "ofMain.h"

#define SOUND_BUFFER_SIZE 256

class karmaSoundAnalyser {
	
public:
	karmaSoundAnalyser( );
	~karmaSoundAnalyser();
	
	// basic functions
	bool start();
	bool stop();
	bool isEnabled() const;
	
	// listeners
	void audioIn(float * input, int bufferSize, int nChannels);
	
protected:
	
	
private:
	bool bEnabled;
	
	float reactivity;
	
	float rawBalance;
	float volumeRight, volumeLeft, volumeMono;
	float rightBuffer[ SOUND_BUFFER_SIZE ], leftBuffer[SOUND_BUFFER_SIZE];
	
	ofSoundStream soundStream;
	
};
