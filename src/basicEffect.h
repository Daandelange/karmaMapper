//
//  basicEffect.h
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 12/3/14.
//
//

#ifndef __illuminatiVisualiser__basicEffect__
#define __illuminatiVisualiser__basicEffect__

//#include <iostream>
#include "ofMain.h"

class basicEffect {
	
public:
	basicEffect();
	~basicEffect();
	
	virtual void spawn();
	virtual void animate();
	virtual void reset();
	virtual void pointsUpdated();
	
	void setPoints(ofPoint _pts[4]);
	
	bool isVisible;
	ofPoint points[4];
	unsigned int lifeSpan;
	
private:
	//bool hovered, selected;
	
	
};

#endif /* defined(__illuminatiVisualiser__basicEffect__) */