//
//  fullScreenShape.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "vertexShape.h"

#define FULLSCREEN_SHAPE_NUM_POINTS 4

class fullScreenShape : public vertexShape {
	
public:
	fullScreenShape(const basicPoint _pos/*=basicPoint::nullPoint*/);
	~fullScreenShape();
	
	
	// #########
	// MAIN FUNCTIONS
	virtual void initialiseFullScreenVariables();
	fullScreenShape* getUpcasted();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml );
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// #########
	// fullScreenShape Properties
	
	// Utilities
	void setPointsToScreenRes();
	void onResChanged( ofResizeEventArgs& _args);

	
};

// GUI translations
//#define GUIinfo_numVertexes		("Vertexes")