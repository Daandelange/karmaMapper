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
	float getBalance(bool _getScaled = false ) const;
	float getVolumeLeft( bool _getSmoothed = true ) const;
	float getVolumeRight( bool _getSmoothed = true ) const;
	float getVolumeMono( bool _getSmoothed = true ) const;
	
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
	
	ofSoundStream soundStream;
	
};
