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

class lineEffect : public basicEffect {
	
public:
	lineEffect();
	~lineEffect();

	virtual bool render();
	virtual void update();
	virtual void reset();
	
	//bool grabSomeShapes();
	
	void noteEventListener(ofx::AbletonLiveSet::LSNoteEvent & noteEvent);
	
protected:
	lineEffectLine getRandomLine(const bool onSameShape=false);
	map<int, vector<int> > shapeGroups; // <groupID, vector<shapeIndexes> >
	list<lineEffectLine> lines;
	
	
private:
	
};
