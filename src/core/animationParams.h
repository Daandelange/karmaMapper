//
//  animationParams.h
//  karmaMapper
//
//  Created by Daan on 07/10/15.
//
//

#pragma once

struct seasonsYear{
	seasonsYear(): winter(0), spring(0), summer(0), autumn(0){};
	
	float winter, spring, summer, autumn;
};

struct colorSet {
	colorSet() :
	main(ofFloatColor(0,0,0,255)),
	secondary(ofFloatColor(0,0,0,255))
	{};
	
	ofFloatColor main, secondary;
};

struct animationParams {
	
	animationParams():
	fps(0),
	elapsedUpdates(0),
	elapsedFrames(0),
	elapsedTime(0.f),
	uniqueID(""),
	uniqueIDAlt(""),
	userChain("")
	{};
	
	float fps;
	unsigned int elapsedUpdates;
	unsigned int elapsedFrames;
	float elapsedTime;
	
	// used for customising effects and synchronising their
	string uniqueID; // hex string
	string uniqueIDAlt;
	
	// chain typed by user // keylogger
	string userChain;
	
	seasonsYear seasons;
	colorSet staticColors;
	colorSet staticColorsAlt;
	colorSet varyingColors;
	colorSet userColors;
	
};