//
//  particleEmitter.cpp
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 14/3/14.
//
//

#include "particleEmitter.h"

// animation makes some edges appear and transitioning

particleEmitter::particleEmitter(){
	
}

particleEmitter::~particleEmitter(){
	
}

void particleEmitter::spawn(){
	
}

void particleEmitter::animate(){
	particle* e;
	for(int i=0; i<particles.size(); i++){
		e=&particles[i];
		
		if(e->done) particles.erase(particles.begin()+i);
		else e->animate();
	}
}

void particleEmitter::reset(){
	
}

void particleEmitter::pointsUpdated(){
	
}

void particleEmitter::addParticles(){
	
	// get a starting line
	ofPoint from = points[ (int) ofRandom(-.4f, 2.4f) ];;
	ofPoint to = points[ (int) ofRandom(-.4f, 2.4f) ];
	while( to == from)
		to = points[ (int) ofRandom(-.4f, 2.4f) ];
	
	int amount = ofRandom(10,30);
	for(int i=0; i<amount; i++){
		ofPoint pos = myLerp(from, to, ofRandom(0,1) );
		ofPoint dir = pos - points[3];
		
		int rNum = ofRandom(10,25);
		for(int ii=0; ii<rNum; ii++) particles.push_back( particle(pos, dir) );
	}
	
}

// EDGE class
particle::particle(ofPoint _position, ofPoint _direction){
	
	position = _position;
	direction = _direction;
	
	speed = ofRandom(.02f,.06f);
	lifeSpan = 1.0f;
	color = color.fromHsb(ofRandom(0,255), 150, 250);
	done = false;
}

particle::~particle(){
	
}

void particle::animate(){
	if(lifeSpan<0.00001){
		done=true;
		return;
	}
	
	ofSetColor(color);
	// render line
	ofLine(position,position-direction*lifeSpan);
	
	// incremment
	lifeSpan-=0.03f;
	position+=direction*speed;
}

ofPoint particleEmitter::myLerp(ofPoint from, ofPoint to, float progress){
	return from+(to-from)*progress;
}

