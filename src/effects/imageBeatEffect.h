//
//  imageBeatEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "basicEffect.h"
#include "imageBeatEffectItem.h"
#include "ofxAbletonLiveSet.h"
#include "mirReceiver.h"
#include "durationReceiver.h"

class imageBeatEffect : public basicEffect {
	
public:
	imageBeatEffect();
	~imageBeatEffect();
	
	// overrided stuff
	virtual bool initialise();
	virtual bool render();
	virtual void update();
	virtual void reset();
	
	// custom functions
	bool grabSomeShapes();
	
	// listeners
	void floatListener( durationFloatEventArgs &_args );
	//void noteEventListener(ofx::AbletonLiveSet::LSNoteEvent & noteEvent);
	//void tempoEventListener( mirTempoEventArgs &_args );
	void onSetEventListener( mirOnSetEventArgs &_args );
	
protected:
	imageBeatEffectItem getRandomBeatItem();
	imageBeatEffectItem	getBeatItem(basicShape* _shape);
	map<int, vector<int> > shapeGroups; // <groupID, vector<shapeIndexes> >
	list<imageBeatEffectItem> beatItems;
	ofMutex effectMutex;
	//vector<float> beatsToAdd;
	vector<basicShape*> beatsToAdd;
	
	//basicShape* fromShape;
	//basicShape* toShape;
	
private:
	float beatItemSpeed;
	float intensity=1;
	int totalbeatItemsNb;
};
