//
//  meshRenderer3D.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//



//
//  textureDistortionEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 06/02/2016.
//
//
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "shaderEffect.h"
#include "ofxAssimpModelLoader.h"

struct animationParams;

class meshRenderer3D : public shaderEffect {
	
public:
	meshRenderer3D();
	~meshRenderer3D();
	
	// global effect functions
	bool initialise(const animationParams& params);
	bool render(karmaFboLayer& renderLayer, const animationParams& params);
	void update(karmaFboLayer& renderLayer, const animationParams& params);
	void reset();
	
	// #########
	// GUI STUFF
	virtual bool printCustomEffectGui();
	
	// #########
	// CUSTOM METHODS
	bool grabShape();
	bool loadModel(string model);
	
	// Listeners
	//void noteEventListener(ofx::AbletonLiveSet::LSNoteEvent & noteEvent);
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene);
	
	// controller functions
	bool randomizePresets();
	
protected:
	
	ofLight light;
	
	ofMesh mesh;
	float angle = 0;
	string meshPath;
	
	ofPoint min, max;
	ofVec3f meshSize;
	vector<float> modelScale;
	
private:
	
};

#define GUImeshRenderer3DEffectPanel "Mesh Renderer 3D"