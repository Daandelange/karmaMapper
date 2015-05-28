//
//  MeshRenderer3D.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "basicEffect.h"
#include "ofxAbletonLiveSet.h"
#include "ofxAssimpModelLoader.h"

class MeshRenderer3D : public basicEffect {
	
public:
	MeshRenderer3D();
	~MeshRenderer3D();
	
	virtual bool render();
	virtual void update();
	virtual void reset();
	
	bool grabShape();
	bool loadModel(string model);
	
	void noteEventListener(ofx::AbletonLiveSet::LSNoteEvent & noteEvent);
	
	
protected:
	
	ofLight light;
	
	ofMesh mesh;
	float angle = 0;
	
	ofPoint min, max;
	ofVec3f meshSize;
	vector<float> modelScale;
	
private:
	
};
