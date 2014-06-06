//
//  basicEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"

class basicEffect {
	
public:
	basicEffect();
	~basicEffect();
	
	virtual void spawn();
	virtual void render();
	virtual void update();
	virtual void reset();
	virtual void destroy();
	//virtual void transitionIn();
	//virtual void transitionOut();
	
	//void updateTransitionState();
	void setShader(ofShader& _shader);
	void bindWithShape(basicShape* _shape);
	
	bool isVisible;
	unsigned int lifeSpan;
	unsigned long long startTime; // compare against ofGetSystemTime();
	
	
protected:
	int transitionState; // [0=faded Out, 1=faded In]
	
	basicShape* pShape;
	bool hasShape;
	
};
