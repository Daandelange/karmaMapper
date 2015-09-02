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
	//ofVec2f& getRandomVertex();
	list<ofVec2f> & getPoints();
	int getNumPoints();
	ofVec2f* getRandomVertexPtr();
	ofVec2f* getCenterPtr();
	// idea: add gravity alterable values: point, averagePosition, etc.
	
	
private:
	//bool hovered, selected;
	//unsigned int numPoints;
	
	// vertexShape Properties
	list<ofVec2f> points; // relative coordinates
	list<ofVec2f> absolutePoints; // copy of above but using absolute coordinates
	list<movablePoint> pointHandlers;

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
	bool handleExists(movablePoint* _i);
	void selectHandle(movablePoint* _i);
	void translateActiveHandle(ofVec2f _offset);
	//void removeHandle(movablePoint* _i);
	
	// #########
	// EDITING UTILITIES
	bool synchronisePointHandlers();
	virtual void applyScale(ofVec2f scale);
	
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