//
//  animationController.cpp
//  workshopMappingCyril
//
//  Created by Daan de Lange on 12/03/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "animationController.h"


// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
animationController::animationController( shapesServer& _scene ): scene(_scene){
	bEnabled = false;
	effects.clear();
	effects.resize(0);
	
	ofAddListener( ofEvents().draw , this, &animationController::draw );
	ofAddListener( ofEvents().update , this, &animationController::update );
}

animationController::~animationController(){
	ofRemoveListener( ofEvents().draw , this, &animationController::draw );
	ofRemoveListener( ofEvents().update , this, &animationController::update );
}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
bool animationController::start(){
	bEnabled = true;
	
	basicEffect* e;
	
	// get shape instance
	//string type="basicEffect";
	//e = stringToShapeClass( type );
	
	// re-enable when uncommenting 2 lines above
	/*if(e==NULL){
	 ofLogError("--> Could not create an effect of type "+type);
	 return;
	 }*/
	
	/*e = new imageBeatEffect();
	 e->bindWithShape( server.getRandomShape() );
	 effects.push_back(e);*/
	
	e = new lineEffect();
	e->bindWithShape( scene.getRandomShape() );
	e->bindWithShape( scene.getRandomShape() );
	e->bindWithShape( scene.getRandomShape() );
	e->bindWithShape( scene.getRandomShape() );
	effects.push_back(e);
	
	/*e = new MeshRenderer3D();
	 e->bindWithShape( server.getRandomShape() );
	 effects.push_back(e);*/
	
	return isEnabled()==false;
}

bool animationController::stop(){
	
	// delete effect pointers
	for(int i=0; i<effects.size();++i) delete effects[i];
	
	effects.clear();
	effects.resize(0);
	
	bEnabled = false;
	return isEnabled()==false;
}

bool animationController::isEnabled() const {
	return bEnabled;
}

// - - - - - - - -
// EVENT LISTENERS
// - - - - - - - -
void animationController::update(ofEventArgs &event){
	if(!isEnabled()) return;
	
	// create animation state
	
	
	// update effects (run mode)
	for(int i=0; i<effects.size(); i++){
		effects[i]->update();
	}
}

void animationController::draw(ofEventArgs& event){
	if(!isEnabled()) return;
	
	// draw effects
	for(int i=0; i<effects.size(); i++){
		effects[i]->render();
	}
}

