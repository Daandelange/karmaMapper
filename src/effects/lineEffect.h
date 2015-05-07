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

class lineEffect : public basicEffect {
	
public:
	lineEffect();
	~lineEffect();

	virtual bool initialise();
	virtual bool render();
	virtual void update();
	virtual void reset();
	
	//bool grabSomeShapes();
	
	// listeners
	void noteEventListener(ofx::AbletonLiveSet::LSNoteEvent & noteEvent);
	void tempoEventListener( mirTempoEventArgs &_args );
	
protected:
	lineEffectLine getRandomLine(const bool onSameShape=false);
	map<int, vector<int> > shapeGroups; // <groupID, vector<shapeIndexes> >
	list<lineEffectLine> lines;
	
	ofMutex lineEffectMutex;
	
private:
	
};
