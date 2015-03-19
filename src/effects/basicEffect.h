//
//  basicEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"
//#include "shapesServer.h"

// Basic parent class for effects

// todo: make a better hasError collection + notification system

// Orgnise all functions of class in a way that you CAN do whatever, but if you don't, it must stay optimized for performence. Encourage interaction with other classes.

// (specific topic)(questionnings)
// todo: constructor(){ classTypes[]="MyClass"] }
// Examples: loopEffect (animates seamlessly from 0.0f to 1.0f and has a loopCount)
//			others animate over time (with lifeTime, etc. )
//			others animate on their own ? (ex: react to music, etc
//				(can there be any other effect specificities?
//				Maybe animator->render(GlobalAnimationStateParams)
//				with information on loop, elapsedTime, seasons?. (They can query music analysis and other data on their own)
// 				Plugins? --> MusicAnalyser, IO Connection, etc.
//			effects with a physical I/O connection
//			effects conneted to a local web server for interaction ? (ex: use user-generated drawings/content)
//			handle one shape or several shapes ? --> if only 1, automatically repeat for all shapes
//			animation preferences ?
//			#shapeAssociation[nb,size,aspect_ratio,etc]
//			#transitionPreferences[in/out]
//			Should an effect also be able to query available shapes? To ask to be moved to another shape? (This shouldn't be forced by the animator) Can an effect ask to be replaced (progressively?) and die? (ex: create organic lifeforms, create interactive games...), Ask for more effects of a certain type and set their initial params? can they ask not to be destroyed yet? etc? Temporarily forbid the use of certain effects on certain shape types ? Etc.
//			Effects preferences ?
//			transitions? make a transition system with custom parenting ? Effects handle their own transitions ?
//				Make effects "connect" protocal to them to each other. They can chose to let other appear/die and notify events to eachother. Two connected effects make a transition
//			How does everything remain connected with eachother ? Maybe 1 main animation class can control the main things and effects can freely move on shapes.
// maybe try { cast class }catch(){ignore}
// maybe compiler can handle this ?
// or force effects to handle more animation modes ? (but can just do nothing)
// how do the effect and the animator interact with each other ?
// the animator calls an effect and handles it's animation state
// the animator can also provide a color palette for a more uniform result.

class basicEffect {
	
public:
	// constructors
	basicEffect();
	virtual ~basicEffect();
	
	// global effect functions
	virtual bool initialise();
	virtual bool render();
	virtual void update();
	virtual void reset();
	void enable();
	void disable();
	
	// effect properties
	bool isReady() const;
	bool isType(const string _type) const;
	string getTypes() const;
	
	// controller functions
	virtual bool randomizePresets();
	//virtual void transitionIn();
	//virtual void transitionOut();
	//void updateTransitionState();
	
	//void setShader(ofShader& _shader);
	void updateBoundingBox();
	
	// shape binding tools
	bool bindWithShape(basicShape* _shape);
	/*virtual void bindWithShapes(vector<basicShape*>& _shapesRef);*/
	bool detachFromAllShapes();
	bool detachFromShape(basicShape* _shape);
	
	// todo: make this read-only
	unsigned int aliveSince;
	unsigned long long startTime; // to compare against ofGetSystemTime();
	
	
protected:
	string effectType;
	//effectParams params;
	
	bool hasError;
	bool isEnabled;
	bool isInitialised;
	bool isLoading;
	
	// todo:
	// animation preferences class
	
	vector<basicShape*> shapes;
	
	ofRectangle overallBoundingBox; // computes boundingbox containing all shapes
	
	
private:
	
	
};
