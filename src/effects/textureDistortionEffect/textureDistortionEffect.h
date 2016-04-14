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
#include "ofxDelaunay.h"

#define KM_TDE_HANDLE_RADIUS 5.0f

struct animationParams;

struct distortionPoint {
	int texCoordIndex = -1;
	ofPoint texCoordOffset = ofPoint(0,0);
	basicPoint* shapeCenter = nullptr;
	int shapeVertIndex = -1;
	string shapeName = "";
};

class textureDistortionEffect : public shaderEffect {
	
public:
	// constructors
	textureDistortionEffect();
	~textureDistortionEffect();
	
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
	bool generateGridFromShapes();
	void reTriangulateFromPoints();
	
	// listeners
	void mouseClicked(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene);
	
	// controller functions
	bool randomizePresets();
	
protected:
	
	bool bIsInEditMode;
	map< basicPoint*, distortionPoint> distortedPoints; // pointRef, textCoord, texIndex
	//ofVbo vbo;
	basicPoint* clicked;
	
	ofxDelaunay triangulation;
	bool bDrawTriangulation;
	
private:
	
	
};

#define GUItextureDistortionEffectPanel "Texture Distorsion"