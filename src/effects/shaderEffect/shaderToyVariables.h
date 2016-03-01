//
//  shaderToyVariables.h
//  karmaMapper
//
//  Created by Daan de Lange on 31/01/16.
//
//

/* - - - -
	Use these variables in your shader file

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform float     iTimeDelta;            // render time (in seconds)
uniform int       iFrame;                // shader playback frame
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
uniform vec4      iDate;                 // (year, month, day, time in seconds)
uniform float     iSampleRate;           // sound sample rate (i.e., 44100)
 
- - - - */

#pragma once

struct shaderToyVariables {
	float iResolution[3] = { 0.f, 0.f, 0.f };
	float iGlobalTime = 0.f;
	float iGlobalTimeScale = 1.0f;
	float iTimeDelta = 0.f;
	int iFrame = 0;
	float iChannelTime[4] = { 0.f, 0.f, 0.f, 0.f };
	float iChannelResolution[4*3] =  {
		0.f, 0.f, 0.f,
		0.f, 0.f, 0.f,
		0.f, 0.f, 0.f,
		0.f, 0.f, 0.f
	};
	float iMouse[4] = { 0.f, 0.f, 0.f, 0.f };
	float iDate[4] = {
		(float) ofGetYear(),	// year
		(float) ofGetMonth(),	// month
		(float) ofGetDay(),		// day
		(float) ofGetElapsedTimef()
	};
};