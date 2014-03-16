//
//  edgeTransitions.cpp
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 14/3/14.
//
//

#include "edgeTransitions.h"

// animation makes some edges appear and transitioning

edgeTransitions::edgeTransitions(){
	
}

edgeTransitions::~edgeTransitions(){
	
}

void edgeTransitions::spawn(){
	
}

void edgeTransitions::animate(){
	edge* e;
	for(int i=0; i<edges.size(); i++){
		e=&edges[i];
		
		// todo URGENT
		if(e->done) edges.erase(edges.begin()+i);
		else e->animate();
	}
}

void edgeTransitions::reset(){
	
}

void edgeTransitions::pointsUpdated(){
	
}

void edgeTransitions::addEdges(){
	edges.push_back( edge(points) );
}

// EDGE class
edge::edge(ofPoint _pts[]){
	
	// randomize start and arrival
	ofPoint corner = _pts[ (int) ofRandom(-.4f, 3.4f) ];
	from[0] = corner;
	from[1] = _pts[ ((int) ofRandom(-.4f, 3.4f)) ];
	to[0] = _pts[ ((int) ofRandom(-.4f, 3.4f)) ];
	to[1] = _pts[ ((int) ofRandom(-.4f, 3.4f)) ];
	
	while( to[0] == from[0])
		to[0] = _pts[ (int) ofRandom(-.4f, 3.4f) ];
	while( from[1] == to[1] )
		from[1] = _pts[ ((int) ofRandom(-.4f, 3.4f)) ];
	while( to[1]==from[1]  )
		to[1] = _pts[ (int) ofRandom(-.4f, 3.4f) ];
	
	speed = ofRandom(.02f,.06f);
	color = color.fromHsb(ofRandom(0,255), 150, 250);
	done = false;
}

edge::~edge(){
	
}

void edge::animate(){
	transitionProgress += speed;
	if(transitionProgress>1) done=true;
	else {
		ofSetColor(color);
		// render line
		ofLine(
			   myLerp(from[0].x, to[0].x, transitionProgress),
			   myLerp(from[0].y, to[0].y, transitionProgress),
			   myLerp(from[1].x, to[1].x, transitionProgress),
			   myLerp(from[1].y, to[1].y, transitionProgress)
		);
	}
}

float edge::myLerp(float from, float to, float progress){
	return from+(to-from)*progress;
}

