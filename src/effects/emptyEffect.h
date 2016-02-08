//
//  emptyEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 19/3/14.
//
//	Use this blank effect as a model to start a new effect.
//


// tmp disabled
#ifdef KM_PLEASESKIPME


#pragma once

#include "ofMain.h"
#include "basicShape.h"
#include "musicEffect.h"

class emptyEffect : public musicEffect {
	
public:
	// constructors
	emptyEffect();
	virtual ~emptyEffect();
	
	// global effect functions
	virtual bool initialise(const animationParams& params);
	virtual bool render(const animationParams& params);
	virtual void update(const animationParams& params);
	virtual void reset();
	
	// controller functions
	virtual bool randomizePresets();
	
	// Event listeners
	// If you use some, make sure to use an ofMutex (http://openframeworks.cc/ofBook/chapters/threads.html)
	
protected:
	
	
private:
	
	
};

// END KM_PLEASESKIPME
#endif