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

// useful links for later
// http://www.geeks3d.com/20140213/glsl-shader-library-fish-eye-and-dome-and-barrel-distortion-post-processing-filters/
// OpenCV camera calibration
// http://www.geeks3d.com/20140213/glsl-shader-library-fish-eye-and-dome-and-barrel-distortion-post-processing-filters/
// http://davis.wpi.edu/~matt/courses/morph/2d.htm
// 

struct animationParams;

struct distortionPoint {
	int texCoordIndex = -1;
	ofDefaultVec2 texCoordOffset = ofDefaultVec2(0,0);  // relative
	ofDefaultVec2 texCoordOffsetChanging = ofDefaultVec2(0,0); // relative
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
	bool bReactToMusic;
	bool bIsInEditMode;
	map< basicPoint*, distortionPoint> distortedPoints; // pointRef, textCoord, texIndex
	//ofVbo vbo;
	basicPoint* clicked;
	
	ofxDelaunay triangulation;
	bool bDrawTriangulation;
	
private:
	
	
};

#define GUItextureDistortionEffectPanel "Texture Distorsion"