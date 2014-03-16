//
//  edgeTransitions.h
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 14/3/14.
//
//

#ifndef __illuminatiVisualiser__edgeTransitions__
#define __illuminatiVisualiser__edgeTransitions__

//#include <iostream>
#include "basicEffect.h"
#include "ofMain.h"

// utility class
class edge {
	
public:
	edge(ofPoint _pts[]);
	~edge();
	
	ofPoint from[2];
	ofPoint to[2];
	
	void animate();
	float myLerp(float from, float to, float progress);
	
	bool done;
	ofColor color;
	
private:
	float speed;
	float transitionProgress;
};


//
class edgeTransitions : public basicEffect {
	
public:
	edgeTransitions();
	~edgeTransitions();
	
	// inherited by basicEffect
	void spawn();
	void animate();
	void reset();
	void pointsUpdated();
	
	void addEdges();
	
	float animator; // todo --> variable that can be externally animated
	
private:
	vector<edge> edges;
	//list<edge> edgee;
};



#endif /* defined(__illuminatiVisualiser__edgeTransitions__) */
