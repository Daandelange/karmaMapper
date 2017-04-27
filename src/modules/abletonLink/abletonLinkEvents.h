//
//  abletonLinkEvents.h
//  karmaMapper
//
//  Created by Daan on 07/05/15.
//
//	Include this file in classes communicating with the mirReceiver

#pragma once

struct abletonLinkSyncEventArgs {
	bool isPlaying = false;
	float currentTime = 0.f;
	double currentBeats = 0;
	double currentPhase = 0;
	int bpm = 120;
	string what = "";
};

// forward declarations are needed for event listening
//class abletonLink;
//ofEvent<abletonLinkSyncEventArgs> abletonLink::abletonLinkSyncEvent;
