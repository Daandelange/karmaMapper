//
//  mirEvents.h
//  karmaMapper
//
//  Created by Daan on 07/05/15.
//
//	Include this file in classes communicating with the mirReceiver

#pragma once


struct liveGrabberBangEventArgs {
	string what = "unknown";
};

struct liveGrabberTempoEventArgs {
	float bpm = 0;
	//string trackName = "unknown";
	int trackID = -1;
};

struct liveGrabberNoteEventArgs {
	//string track = "unknown";
	string key = "";
	bool isMajor = false;
	
};

struct liveGrabberAnalysisData {
	
public:
//	liveGrabberAnalysisData(){
//		
//	}
	
	float lowFollower = 0.f;
	float midFollower = 0.f;
	float highFollower = 0.f;
	
};