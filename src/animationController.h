//
//  animationController.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/03/2015.
//
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"
#include "effects.h"
#include "shapesServer.h"

class animationController {
	
public:
	animationController( shapesServer& _scene );
	~animationController();
	// fix for no default constructor
	animationController& operator=(const animationController& crap) { return *this; }
	
	// basic functions
	bool start();
	bool stop();
	bool isEnabled() const;
	
	// listeners
	void update(ofEventArgs& event);
	void draw(ofEventArgs& event);
	
protected:
	
	// effects stuff
	bool isEffectsIndex(int i);
	vector<basicEffect*> effects;
	
	shapesServer& scene;
	
private:
	bool bEnabled;
	
};
