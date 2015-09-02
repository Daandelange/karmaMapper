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
#include "ofxGui.h"
#include "ofxGuiExtended.h"

// todo: remove destroy() and spawn() and isReady()
// todo: move vertexShape stuff to it's own class and keep the bare minimum (points, etc)
// todo: rename colors (fgColor,bgColor) and give names like groupColor, primaryColor, secondaryColor, etc.
// todo: add name so we can getShapeByName()

// When a shape exits edit mode, all coordinates are made absolute so they are easy to use for effects. Relative coordinates are easier for manipulation and storage.
// Shape querying and altering happens trough absolute coordinates.

typedef list<ofVec2f>& vec2fListRef; // without the typedef returning this causes compiler errors.

class basicShape {

public:
	basicShape();
	virtual ~basicShape();
	
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
	// BASIC SHAPE GETTERS
	int getGroupID() const;
	bool isReady() const;
	const string getShapeType() const;
	ofRectangle getBoundingBox() const;
	vector<string> getTypes() const;
	bool isType(const string _type) const;
	virtual bool isInside( const ofVec2f _pos, const bool _isPositionAbsolute = true) const;
	ofVec2f* getPositionPtr();
	ofVec2f getPositionUnaltered() const;

	// #########
	// global variables
	static ofVec2f zeroPoint;
	
protected:
	
	// basicShape properties
	bool initialized = false;
	bool hasError = false;
	ofRectangle boundingBox; // contains all shapes
	int groupID; // [-1=none, other = groupID]
	string shapeName;
	vector<string> shapeTypes;
	
	ofVec2f position; // absolute (other shape data will be relative to this)
	
	
private:
	// todo: put variables in here
	//ofVec2f guiPos;

	
	// * * * * * * * * * * * * * * * * * * *
#ifdef KM_EDITOR_APP


public:
	// #########
	// EDITING ESSENTIALS
	virtual void render();
	virtual void buildMenu();
	
	// #########
	// EDITING CONTROL
	bool isInEditMode() const;
	bool switchEditMode();
	virtual bool enableEditMode();
	virtual bool disableEditMode();
	
	// #########
	// EDITING UI CONTROL
	virtual void selectPrevHandle();
	virtual void selectNextHandle();
	virtual bool handleExists(movablePoint* _i);
	virtual void selectHandle(movablePoint* _i);
	virtual void translateActiveHandle(ofVec2f _offset);
	
	// #########
	// EDITING UTILITIES
	virtual bool synchronisePointHandlers();
	virtual void applyScale(ofVec2f scale);
	
	// #########
	// SETTERS
	bool setPosition(const ofVec2f _pos);
	bool setGroupID(const int _id);
	
	// #########
	// GUI LISTENERS
	virtual void keyPressed(ofKeyEventArgs& e);
	virtual bool interceptMouseClick(ofMouseEventArgs &e);
	
protected:
	bool bEditMode;
	
	ofColor fgColor;
	ofColor bgColor;
	static int maxGroupID; // used for color calculation
	int prevMaxGroupID; // used for color calculation
	
	movablePoint* activeHandle = NULL;
	movablePoint positionPointHandler;
	
	// custom HUD GUI elements
	ofRectangle guiToggle;
	ofxPanelExtended* shapeGui = NULL;
	ofxGuiGroupExtended menuParams;
	
	void setColorFromGroupID();
	
// endif KM_EDITOR_APP
#endif

};