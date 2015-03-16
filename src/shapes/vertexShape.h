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

// todo: rename pointHandlers to vertexPointHandlers
// todo: add virtual to the right functions here

class vertexShape : public basicShape {
	
public:
	vertexShape();
	~vertexShape();
	
	// #########
	// MAIN FUNCTIONS
	virtual void reset();
	virtual void render();
	virtual void drawForEditor();
	virtual void calculateBoundingBox();
	virtual void onShapeChanged();
	//virtual void resetChangedValues();
	
	// load & save
	virtual bool saveToXML(ofxXmlSettings& xml );
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// #########
	// Vertex Shape Properties
	
	
	// Utilities
	//ofVec2f& getRandomVertex();
	list<ofVec2f> & getPoints();
	ofVec2f* getRandomVertexPtr();
	ofVec2f* getCenterPtr();
	// idea: add gravity alterable values: point, averagePosition, etc.
	
	// edit stuff
	bool enableEditMode();
	bool disableEditMode();
	bool synchronisePointHandlers();
	
	void selectPrevHandle();
	void selectNextHandle();
	void selectHandle(movablePoint* _i);
	void translateActiveHandle(ofVec2f _offset);
	//bool isSelectableItem(int _i);
	void removeHandle(movablePoint* _i);
	virtual void applyScale(ofVec2f scale);
	
	// custom functions
	void keyPressed( ofKeyEventArgs& e );
	void mousePressed( ofMouseEventArgs& args );
	//void mouseDragged( ofMouseEventArgs& args );
	//void mouseReleased( ofMouseEventArgs& args );
	
private:
	//bool hovered, selected;
	//unsigned int numPoints;
	
	// vertexShape Properties
	list<ofVec2f> points; // relative coordinates
	list<ofVec2f> absolutePoints; // copy of above but using absolute coordinates
	
	// edit mode stuff
	
	
	// instructions stuff
	ofxTextBox instructions;
	bool showInstructions;
	
	
	
};
