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

// this class is a singleton in order to let any component access MIR data

// Useful links:
// - https://github.com/openframeworks/openFrameworks/wiki/Sound-programming-notes
// - http://docs.openframeworks.cc/ofSoundBuffer.html
// - http://openframeworks.cc/ofBook/chapters/sound.html <-- rly interesting
// - http://openframeworks.cc/documentation/sound/ofSoundStream.html
// - http://en.wikipedia.org/wiki/Music_information_retrieval

// INTERESTING 3rd PARTY LIBRARIES

// lib Aubio: https://github.com/piem/aubio

// maximilian: https://github.com/micknoise/Maximilian
// functions: spectrum analysis, spectral features, octave analysis, Bark scale analysis, and MFCCs

// lib marsyas: http://marsyasweb.appspot.com/

// information to retrieve:
// - onset (beat detections)
// - sound identification ?
// - (multi)pitch detection
// - zero-crossing rate
// - timbre
// - balance
// - rhythm detection // bpm // tempo
// - FFT

// or use puredata ?
// https://vimeo.com/84516862

class karmaSoundAnalyser {
	
public:
	karmaSoundAnalyser( );
	~karmaSoundAnalyser();
	
	// singleton stuff
	static karmaSoundAnalyser& getInstance(){
		static karmaSoundAnalyser instance; // Guaranteed to be destroyed and instantiated on first use
		return instance;
	}
	
	// prevents accidentally creating copies of your singleton
	karmaSoundAnalyser(karmaSoundAnalyser const&)               = delete;
	void operator=(karmaSoundAnalyser const&)  = delete;
	
	// basic functions
	bool start();
	bool stop();
	
	
	// listeners
	void audioIn(float * input, int bufferSize, int nChannels);
	
	// parameters
	void setSmoothingReactivity( float _smoothingReactivity );
	
	// getters
	bool isEnabled() const;
	int getBufferSize() const;
	const float* getBufferLeft() const;
	float getBalance(bool _getScaled = false ) const;
	float getVolumeLeft( bool _getSmoothed = true ) const;
	float getVolumeRight( bool _getSmoothed = true ) const;
	float getVolumeMono( bool _getSmoothed = true ) const;
	int getZeroCrossings( ) const;
	float getSpectrumVariation( ) ;
	
protected:
	
	
private:
	bool bEnabled;
	
	// setting variables
	float smoothingReactivity;
	
	// analysis variables
	float balance, balanceSmoothed, balanceScaled;
	float volumeRight, volumeLeft, volumeMono;
	float volumeLeftSmoothed, volumeRightSmoothed, volumeMonoSmoothed;
	
	
	// inner variables
	float rightBuffer[ SOUND_BUFFER_SIZE ], leftBuffer[SOUND_BUFFER_SIZE];
	float balanceScale;
	float zeroCrossingDirection;
	vector<int> zeroCrossings; // hold zerocrossings for 1 sec
	vector<float> spectrumVariation;
	
	ofSoundStream soundStream;
	ofMutex soundMutex; // needed because audioIn() runs on a separate thread
	
};
