//
//  emptyEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 19/3/14.
//
//	Use this blank effect as a model to start a new effect.
//

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
	virtual bool initialise();
	virtual bool render();
	virtual void update();
	virtual void reset();
	
	// controller functions
	virtual bool randomizePresets();
	
protected:
	
	
private:
	
	
};
