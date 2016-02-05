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
//#include "ofxTextBox.h"

class vertexShape : public basicShape {
	
public:
	vertexShape(const basicPoint _pos);
	~vertexShape();
	
	// #########
	// MAIN FUNCTIONS
	void initialiseVertexVariables();
	virtual void sendToGPU();
	vertexShape* getUpcasted();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml );
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// #########
	// UTILITIES
	virtual void calculateBoundingBox();
	virtual void onShapeModified();
	virtual void onShapeEdited();
	virtual void resetToScene();
	
	// #########
	// Vertex Shape Properties
	
	// Utilities
	//basicPoint& getRandomVertex();
	list<basicPoint> & getPoints( const basicShapePointType& _type = POINT_POSITION_RELATIVE );
	int getNumPoints();
	basicPoint* getRandomVertexPtr( const basicShapePointType& _type = POINT_POSITION_RELATIVE );
	basicPoint* getNextVertexPtr(basicPoint& _p, const basicShapePointType& _type = POINT_POSITION_RELATIVE, bool _getPrev = false);
	basicPoint* getCenterPtr();
	// idea: add gravity alterable values: point, averagePosition, etc.
	
	static list<basicPoint> zeroList;
	
protected:
	//bool hovered, selected;
	//unsigned int numPoints;
	
	// vertexShape Properties
	list<basicPoint> points; // relative coordinates
	list<basicPoint> changingPoints; // relative alterable coordinates
	list<basicPoint> absolutePoints; // copy of above but using absolute coordinates

#ifdef KM_EDITOR_APP

public:
	// #########
	// EDITING ESSENTIALS
	virtual void render();
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
	ofParameter<string> menuNumVertexes;

	// instructions stuff
	//ofxTextBox instructions;
	bool showInstructions;
	
private:
	// constructor helpers
	void buildVertexMenu();
	
	
// endif KM_EDITOR_APP
#endif

};

// GUI translations
#define GUIinfo_numVertexes		("Vertexes")