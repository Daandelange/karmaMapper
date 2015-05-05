//
//  basicShape.h
//  karmaMapper
//
//  Created by Daan de Lange on 22/3/14.
//  Defines common methods for shape handling
//

// todo: move load/save functions to basicShape rather than shapesHandler

#pragma once

#include "ofMain.h"
#include "movablePoint.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"

// todo: remove destroy() and spawn() and isReady()
// todo: move vertexShape stuff to it's own class and keep the bare minimum (points, etc)
// todo: rename colors (fgColor,bgColor) and give names like groupColor, primaryColor, secondaryColor, etc.
// todo: add name so we can getShapeByName()
// todo: shapeType should be a vector so it contains all subclasses

// When a shape exits edit mode, all coordinates are made absolute so they are easy to use for effects. Relative coordinates are easier for manipulation and storage.
// Shape quarying and altering happens trough absolute coordinates.

typedef list<ofVec2f>& vec2fListRef; // without the typedef returning this causes compiler errors.

class basicShape {

public:
	basicShape();
	virtual ~basicShape(); // rm virtual ?
	
	// #########
	// MAIN FUNCTIONS
	virtual void reset();
	virtual void render();
	virtual void drawForEditor();
	virtual void calculateBoundingBox();
	virtual void onShapeChanged();
	virtual void resetChangedValues();
	
	virtual bool saveToXML(ofxXmlSettings& xml );
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// #########
	// BASIC SHAPE PROPERTIES
	
	// Getters
	bool isReady() const;
	const string& getShapeType() const;
	ofRectangle getBoundingBox() const;
	int getGroupID() const;
	vector<string> getTypes() const;
	bool isType(const string _type) const;
	virtual bool isInside( const ofVec2f _pos, const bool _isPositionAbsolute = true) const;
	ofVec2f* getPositionPtr();
	ofVec2f getPositionUnaltered() const;

	// #########
	// SETTERS
	bool setPosition(const ofVec2f _pos);
	//bool putPoints(list<ofVec2f>& _points);
	bool setGroupID(const int _id);
	
	// #########
	// EDIT STUFF
	bool isInEditMode() const;
	virtual bool enableEditMode();
	virtual bool disableEditMode();
	bool switchEditMode();
	virtual bool synchronisePointHandlers();
	
	// detailed edit stuff
	virtual void selectPrevHandle();
	virtual void selectNextHandle();
	virtual bool isSelectableHandle(movablePoint* _i);
	virtual void selectHandle(movablePoint* _i);
	virtual void translateActiveHandle(ofVec2f _offset);
	virtual void applyScale(ofVec2f scale);
	
	// GUI
	virtual void keyPressed(ofKeyEventArgs& e);
	virtual void guiEvent(ofxUIEventArgs &e);
	virtual void mouseClicked(ofMouseEventArgs &e);
	
	//ofPoint getCenterOffsetFromBoundingBox();

	bool hasError = true; // changes visual aspect to notify error
	
	// global variables
	static ofVec2f zeroPoint;
	
protected:
	
	// basicShape properties
	bool initialized = false;
	ofRectangle boundingBox; // contains all shapes
	int groupID; // [-1=none, other = groupID]
	static int maxGroupID; // used for color calculation
	int prevMaxGroupID; // used for color calculation
	string shapeType;
	int shapeId; // utility for handling several basicShape instances. (give uuid at creation) // todo
	
	// editor properties
	bool bEditMode;
	ofColor fgColor;
	ofColor bgColor;
	movablePoint* activeHandle; // [-1=none, other=activeID]
	
	ofVec2f position; // absolute (inside points will be relative)
	
	movablePoint positionPointHandler;
	list<movablePoint> pointHandlers;
	
	// custom HUD GUI elements
	ofRectangle guiToggle;
	ofxUICanvas* gui = NULL;
	
	void setColorFromGroupID();
	
	
private:
	// todo: put variables in here
	//ofVec2f guiPos;
};
