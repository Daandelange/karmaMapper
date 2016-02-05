//
//  durationEvents.h
//  karmaMapper
//
//  Created by Daan on 10/05/15.
//
//	Include this file in classes communicating with the durationReceiver

#pragma once

struct durationBangEventArgs {
	string track = "unknown";
};

struct durationFlagEventArgs {
	string track = "unknown";
	string flag = "";
};

// for trackTypes: curve, lso
struct durationFloatEventArgs {
	string track = "unknown";
	string type = "none";
	float value = 0;
};

struct durationSwitchEventArgs {
	string track = "unknown";
	bool enabled = false;
};

// forward declaration
//class durationReceiver;
//ofEvent<durationTempoEventArgs> durationReceiver::durationTempoEvent;
