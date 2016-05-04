//
//  cpuWebcamAnalyser.h
//  karmaMapper
//
//  Created by Daan de Lange on 27/04/16.
//
//
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "videoShader.h"
#include "animationParams.h"
#include "mirReceiver.h"
#include "ofxFPS.h"
#include "ofxFaceTracker.h"

struct animationParams;

enum webcamAnalysisMode {
	// note: each mode must have a unique key, never replace the one of another
	WEBCAM_ANALYSIS_FACEDETECT = 0, // detect faces and spawn them
};

// todo: make video decoding threaded
// todo: make UVC controls from videoShader includeable here for re-use.
// todo: disable webcam when the effect disables

class cpuWebcamAnalyser : public videoShader  {
	
public:
	// constructors
	cpuWebcamAnalyser();
	~cpuWebcamAnalyser();
	
	// global effect functions
	bool initialise(const animationParams& params);
	bool render(karmaFboLayer& renderLayer, const animationParams& params);
	void update(karmaFboLayer& renderLayer, const animationParams& params);
	void reset();
	
	// #########
	// GUI STUFF
	virtual bool printCustomEffectGui();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene);
	
	// controller functions
	bool randomizePresets();
	
	// #########
	// videoEffect FUNCTIONS
	void setAnalysisMode(const enum webcamAnalysisMode& mode);
	//void setUseThread( const bool& _useThread );
	
protected:
	virtual void threadedFunction();
	enum webcamAnalysisMode analysisMode;
	
	// for face detection
	ofxFaceTracker faceTracker;
	int fFaceTrackerOrientationTolerance;// (angle)
	int minTimeBetweenShots; // secs
	int targetDistanceBetweenEyes; // px
	unsigned int canTakeShot;
	bool bDebugFaceTracking;
	float lastShotTime;
	
	ofTexture lastShot;
	
private:
	
	
};

#define GUIcpuWebcamAnalyserPanel "Webcam Analyser"