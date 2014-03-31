//
//  basicEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"

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
	
	void updateTransitionState();
	
	bool isVisible;
	unsigned int lifeSpan;
	
private:
	int transitionState; // [0=faded Out, 1=faded In]
	ofPoint* points[];
	//bool hovered, selected;
	
	
};
