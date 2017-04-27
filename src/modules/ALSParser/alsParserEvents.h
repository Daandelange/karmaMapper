//
//  alsParserEvents.h
//  karmaMapper
//
//  Created by Daan on 07/05/15.
//
//	Include this file in classes communicating with the mirReceiver

#pragma once

#include "ofxAbletonLiveSet/Model.h"

typedef ofx::AbletonLiveSet::LSTrackEvent alsTrackEventArgs;
typedef ofx::AbletonLiveSet::LSNoteEvent alsNoteEventArgs;
//ofx::AbletonLiveSet::LSNoteEvent

struct alsBangEventArgs {
	string what = "unknown";
};

//struct alsTrackEventArgs {
//	string trackname = "";
//};

struct liveGrabberTempoEventArgs {
	float bpm = 0;
	//string trackName = "unknown";
	int trackID = -1;
};

//struct alsNoteEventArgs {
//	//string track = "unknown";
//	string key = "";
//	bool isMajor = false;
//	
//};
