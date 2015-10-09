//
//  lineEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "basicEffect.h"
#include "lineEffectLine.h"
#include "ofxAbletonLiveSet.h"
#include "mirReceiver.h"
#include "durationReceiver.h"

class lineEffect : public basicEffect {
	
public:
	lineEffect();
	~lineEffect();

	virtual bool initialise(const animationParams& params);
	virtual bool render(const animationParams& params);
	virtual void update(const animationParams& params);
	virtual void reset();
	
	//bool grabSomeShapes();
	
	// listeners
	void floatListener( durationFloatEventArgs &_args );
	void noteEventListener(ofx::AbletonLiveSet::LSNoteEvent & noteEvent);
	void tempoEventListener( mirTempoEventArgs &_args );
	void onSetEventListener( mirOnSetEventArgs &_args );
	
protected:
	lineEffectLine getRandomLine(const bool onSameShape=false);
	lineEffectLine getRandomLine(basicShape* _sh1, basicShape* _sh2);
	//map<int, vector<int> > shapeGroups; // <groupID, vector<shapeIndexes> >
	list<lineEffectLine> lines;
	int tempoCalls;
	
	//ofMutex lineEffectMutex;
	ofFbo renderer;
	
	//void clearWithTransparency(float transparency);
	
	basicShape* fromShape;
	basicShape* toShape;
	
private:
	float intensity;
	int totalLinesNb = 0;
};
