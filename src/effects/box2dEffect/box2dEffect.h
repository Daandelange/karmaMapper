//
//  box2dEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/05/2016.
//
//
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "shaderEffect.h"
#include "animationParams.h"
#include "mirReceiver.h"
#include "liveGrabberOSC.h" // todo: use liveGrabberEvents.h
//#include "ofxBox2d.h"
#include "ofxLiquidFun.h"

struct animationParams;

struct box2dItemProperties {
	ofVec3f physics = ofVec3f(1.0, 0.5, 0.3);
	ofVec2f position = ofVec2f(0);
	float radius = 5;
	ofVec2f velocity = ofVec2f(0,0);
};
struct box2dParticleProperties {
	ofColor color = ofColor(255);
	ofVec2f position = ofVec2f(0);
	//float radius = 5;
	ofVec2f velocity = ofVec2f(0,0);
};


class box2dEffect : public shaderEffect/*, public ofThread */  {
	
public:
	// constructors
	box2dEffect();
	virtual ~box2dEffect();
	
	// global effect functions
	virtual bool initialise(const animationParams& params);
	virtual bool render(karmaFboLayer& renderLayer, const animationParams& params);
	virtual void update(karmaFboLayer& renderLayer, const animationParams& params);
	void reset();
	
	// #########
	// GUI STUFF
	virtual bool printCustomEffectGui();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene);
	
	// controller functions
	virtual bool disableSoonIsNow() ;
	bool randomizePresets();
	
	// #########
	// box2d FUNCTIONS
	void initBox2d();
	void syncBox2dWorldSettings();
	void syncBox2dWithShapes();
	
	// listeners
	void _mousePressed( ofMouseEventArgs& args );
	void liveGrabberNoteEventListener(liveGrabberNoteEventArgs &_args);
	

	
protected:
	//virtual void threadedFunction();
	
	ofxBox2d box2d;
	//vector <shared_ptr<ofxBox2dEdge> > box2dWorld;
	vector <shared_ptr<ofxBox2dCircle> > box2dShapeItems;
	ofxBox2dParticleSystem particles;
	ofThreadChannel< box2dItemProperties > newBox2dItemsFromThread;
	ofThreadChannel< box2dParticleProperties > newBox2dParticlesFromThread;
	//ofThreadChannel< shared_ptr<ofxBox2dCircle> > newparticlesForThread;
	vector <shared_ptr<ofxBox2dEdge> >   box2dEdges;
	struct box2dWorldProperties {
		int fps = 60;
		float gravityAngleDeg = 90.f;
		float gravityIntensity = 5.f;
		int iterationsVelocityTimes = 10;
		int iterationsPositionTimes = 2;
	} box2dWorldProperties;
	
	//bool bFlagBox2dReset;
	
	bool bReactWithLiveGrabber;
	bool bAddParticlesWithMouse;
	int mouseAddAmount;
	bool bDrawBox2dEdges;
	bool bSyncBox2dOnUpdate;
	
	// for stats
	float box2dGlobalRotation;
	ofVec2f box2dGlobalVelocity;
	ofVec2f box2dAveragePosition;
	ofVec3f box2dSpeed; // avg, min, max
	ofVec2f box2dParticlesAveragePosition;
	ofVec2f box2dParticlesAverageVelocity;
	float box2dParticlesSpeed;
	float box2dParticlesCollisionEnergy;
	
private:
	
	
};

#define GUIbox2dEffectPanel "Box 2D Effect"