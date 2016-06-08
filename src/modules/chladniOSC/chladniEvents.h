//
//  chladniEvents.h
//  karmaMapper
//
//  Created by Daan on 05/06/2016.
//
//	Include this file in classes communicating with the chladniReceiver

#pragma once

struct chladniBangEventArgs {
	string name = "unknown";
};

struct chladniFloatEventArgs {
	string name = "unknown";
	float value = 0;
};

// forward declaration
//class chladniReceiver;
//ofEvent<chladniTempoEventArgs> chladniReceiver::chladniTempoEvent;
