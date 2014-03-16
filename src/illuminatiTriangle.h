//
//  illuminatiTriangle.h
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 24/2/14.
//
//

#pragma once

//#include <iostream>
#include "ofxMSAInteractiveObject.h"
#include "movablePoint.h"

// include all effects here
#include "basicEffect.h"
#include "imageMapper.h"
#include "edgeTransitions.h"
#include "particleEmitter.h"

class illuminatiTriangle : public ofxMSAInteractiveObject {
	
public:
	illuminatiTriangle();
	~illuminatiTriangle();
	
	void setup();
	void exit();
	void update();
	void draw();
	void drawWireFrame();
	
	void enableEditMode();
	void disableEditMode();
	void switchEditMode();
	void updateEditModeToChildren();
	
	// effects handling stuff
	void initEffects();
	void animateEffects();
	void updateEffectPoints();
	void updateData(float _eq);
	
	virtual void onRollOver(int x, int y);
	virtual void onRollOut();
	virtual void onMouseMove(int x, int y);
	virtual void onDragOver(int x, int y, int button);
	virtual void onDragOutside(int x, int y, int button);
	virtual void onPress(int x, int y, int button);
	virtual void onRelease(int x, int y, int button);
	virtual void onReleaseOutside(int x, int y, int button);
	virtual void keyPressed(int key);
	virtual void keyReleased(int key);
	
	imageMapper imgAnim1;
	imageMapper imgAnim2;
	edgeTransitions edgeTransitioner;
	particleEmitter pEmitter;
	
	bool a1;
	
private:
	movablePoint points[4];
	//vector<movablePoint> points;
	//movablePoint crap;
	bool isInEditMode;
	int numPoints = 4; // shouldn't be changed
	
	
	
};