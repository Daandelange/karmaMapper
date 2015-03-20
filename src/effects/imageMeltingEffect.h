//
//  imageMeltingEffect.h
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

// todo: split this into a separate imageEffect class.

// forward declaration;
class imageMeltingPoint;

class imageMeltingEffect : public musicEffect {
	
public:
	// constructors
	imageMeltingEffect();
	virtual ~imageMeltingEffect();
	
	// global effect functions
	virtual bool initialise();
	virtual bool render();
	virtual void update();
	virtual void reset();
	
	// controller functions
	virtual bool randomizePresets();

	
	
protected:
	
	
private:
	ofImage sourceImg;
	map<int,ofTexture> shapeTextures; // shapeID, texture
	multimap<int,imageMeltingPoint> flyingTextures; // textureID, point
	
	// custom sound analysis variables
	float lastVolumePeakAmount;
};

class imageMeltingPoint {
public:
	imageMeltingPoint();
	imageMeltingPoint(ofVec2f _position, ofVec2f _velocity);
	~imageMeltingPoint();
	
	void update();
	void render( const ofTexture& _tex);
	
	ofPoint position;
	ofPoint velocity;
	bool isDead;
};