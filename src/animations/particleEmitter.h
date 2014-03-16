//
//  particleEmitter.h
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 14/3/14.
//
//

#ifndef __illuminatiVisualiser__particleEmitter__
#define __illuminatiVisualiser__particleEmitter__

//#include <iostream>
#include "basicEffect.h"
#include "ofMain.h"

// utility class
class particle {
	
public:
	particle(ofPoint _position, ofPoint _direction);
	~particle();
	
	ofPoint from[2];
	ofPoint to[2];
	
	void animate();
	
	
	bool done;
	ofColor color;
	float lifeSpan;
	
private:
	float speed;
	float transitionProgress;
	ofPoint position, direction;
};


//
class particleEmitter : public basicEffect {
	
public:
	particleEmitter();
	~particleEmitter();
	
	// inherited by basicEffect
	void spawn();
	void animate();
	void reset();
	void pointsUpdated();
	
	void addParticles();
	ofPoint myLerp(ofPoint from, ofPoint to, float progress);
	
	float animator; // todo --> variable that can be externally animated
	
	
private:
	vector<particle> particles;
	
};



#endif /* defined(__illuminatiVisualiser__particleEmitter__) */
