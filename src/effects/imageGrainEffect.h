//
//  imageGrainEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"
#include "musicEffect.h"

// forward declaration;
class imageGrainItem;

class imageGrainEffect : public musicEffect {
	
public:
	// constructors
	imageGrainEffect();
	virtual ~imageGrainEffect();
	
	// global effect functions
	virtual bool initialise();
	virtual bool render();
	virtual void update();
	virtual void reset();
	
	// controller functions
	virtual bool randomizePresets();

	
	
protected:
	
	
private:
	vector<imageGrainItem> items;
	ofImage img;
};

class imageGrainItem {
public:
	imageGrainItem();
	imageGrainItem(ofPoint _position);
	~imageGrainItem();
	
	void update();
	void render( const ofPixels& _pix);
	
	ofPoint position;
	bool isDead;
	vector<ofPoint> distorsions;
};