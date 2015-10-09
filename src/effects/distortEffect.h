//
//  distortEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"
#include "vertexShape.h"
#include "basicEffect.h"
#include "animationParams.h"

struct animationParams;

class distortEffect : public basicEffect {
	
public:
	// constructors
	distortEffect();
	~distortEffect();
	
	// global effect functions
	bool initialise(const animationParams& params);
	bool render(const animationParams& params);
	void update(const animationParams& params);
	void reset();
	
	// controller functions
	bool randomizePresets();
	
	
	
protected:
	
	
private:
	
	
};
