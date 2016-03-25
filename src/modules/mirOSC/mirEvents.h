//
//  mirEvents.h
//  karmaMapper
//
//  Created by Daan on 07/05/15.
//
//	Include this file in classes communicating with the mirReceiver

#pragma once


struct mirBangEventArgs {
	string what = "unknown";
};

struct mirTempoEventArgs {
	float bpm = 0;
	bool isTempoBis = 0;
	int beatCount = 0;
};

struct mirOnSetEventArgs {
	string source = "unknown";
};

class mirData {
	
public:
	mirData(){
		
	}
	
	float volumeMono=0;
	float volumeRigh=0;
	float volumeLeft=0;
	float bpm = 0;
	float pitch = 0; // in hertz (0-1024[?])
	float balance; //  -1+ -> 1+, not limited
	float zcr = 0; // 0 -> 1+
	bool silence = false;
	bool isPlaying = false;
	float fiddleFFT[7][2] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
};

// forward declaration
//class mirReceiver;
//ofEvent<mirTempoEventArgs> mirReceiver::mirTempoEvent;
