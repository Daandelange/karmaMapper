//
//  vertexShape.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"
#include "ofxTextBox.h"

class vertexShape : public basicShape {
	
public:
	vertexShape();
	~vertexShape();
	
	// #########
	// MAIN FUNCTIONS
	virtual void initialiseVariables();
	virtual void sendToGPU();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml );
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// #########
	// UTILITIES
	virtual void calculateBoundingBox();
	virtual void onShapeChanged();
	virtual void resetToScene();
	
	// #########
	// Vertex Shape Properties
	
	// Utilities
	//basicPoint& getRandomVertex();
	list<basicPoint> & getPoints();
	int getNumPoints();
	basicPoint* getRandomVertexPtr();
	basicPoint* getCenterPtr();
	// idea: add gravity alterable values: point, averagePosition, etc.
	
	static list<basicPoint> zeroList;
	
private:
	//bool hovered, selected;
	//unsigned int numPoints;
	
	// vertexShape Properties
	list<basicPoint> points; // relative coordinates
	list<basicPoint> absolutePoints; // copy of above but using absolute coordinates

#ifdef KM_EDITOR_APP

public:
	// #########
	// EDITING ESSENTIALS
	virtual void render();
	virtual void buildMenu();
	bool enableEditMode();
	bool disableEditMode();
	
	
	// #########
	// EDITING UI CONTROL
	void selectPrevHandle();
	void selectNextHandle();
	bool handleExists(basicPoint* _i);
	void selectHandle(basicPoint* _i);
	void translateActiveHandle(basicPoint _offset);
	//void removeHandle(basicPoint* _i);
	
	// #########
	// EDITING UTILITIES
	virtual void applyScale(basicPoint scale);
	
	// #########
	// GUI LISTENERS
	void keyPressed( ofKeyEventArgs& e );
	virtual bool interceptMouseClick(ofMouseEventArgs &e);
	
protected:
	ofxGuiGroupExtended vertexMenu;

	// instructions stuff
	ofxTextBox instructions;
	bool showInstructions;
	
private:
	
// endif KM_EDITOR_APP
#endif

};