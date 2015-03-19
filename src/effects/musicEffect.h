//
//  musicEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"
#include "basicEffect.h"
#include "karmaSoundAnalyser.h"

// parent class for effects reacting to music

class musicEffect : public basicEffect {
	
public:
	// constructors
	musicEffect();
	virtual ~musicEffect(){}
	
	// global effect functions
	virtual bool initialise();
	virtual bool render();
	virtual void update();
	virtual void reset();
	
	
protected:
	void renderVariables();
	
private:
	
	
};
