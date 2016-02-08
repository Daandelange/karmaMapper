//
//  videoStreamEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

// tmp disabled
#ifdef KM_PLEASESKIPME

#pragma once

#include "ofMain.h"
#include "basicEffect.h"
#include "ofxAbletonLiveSet.h"

class videoStreamEffect : public basicEffect {
	
public:
	videoStreamEffect();
	~videoStreamEffect();
	
	virtual void spawn();
	virtual bool render();
	virtual void update();
	virtual void reset();
	
	bool grabShape();
	bool grabStream();
	
	void noteEventListener(ofx::AbletonLiveSet::LSNoteEvent & noteEvent);
	
	
protected:
	ofVideoGrabber grabber;
	int currentStream = -1;
	
private:
	
};

// END KM_PLEASESKIPME
#endif