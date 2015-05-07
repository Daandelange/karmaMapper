//
//  mirEvents.h
//  karmaMapper
//
//  Created by Daan on 07/05/15.
//
//

#pragma once


struct mirBangEventArgs {
	string what = "unknown";
};

struct mirTempoEventArgs {
	float bpm = 0;
	bool isTempoBis = 0;
};

struct mirOnSetEventArgs {
	string source = "unknown";
};

class mirData {
	
public:
	mirData(){
		
	}
	
	float bpm = 0;
	float pitch = 0;
	float zcr = 0;
	bool silence = false;
};

// forward declaration
//class mirReceiver;
//ofEvent<mirTempoEventArgs> mirReceiver::mirTempoEvent;
