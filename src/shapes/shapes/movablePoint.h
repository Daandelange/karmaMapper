//
//  movablePoint.h
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 24/2/14.
//
//

#pragma once

#include "ofMain.h"

class movablePoint {
	
public:
	movablePoint();
	~movablePoint();
	
	void setup();
	void exit();
	void update();
	void draw();
	
	void setEditable( bool status );
	
	void makeParent( list<movablePoint>& _children );
	void removeChildren();
	
	void focus();
	void blur();
	
	void enable();
	void disable();
	bool isEnabled() const;
	
	ofVec2f getPos();
	//void setPosSize(ofPoint _pos, int _diameter);
	void setPos(ofVec2f _pos);
	void translate(ofVec2f _offset);
	//void setPosRelative(ofPoint _pos);
	ofVec2f center;
	bool isMouseOver();
	bool isMousePressed();
	bool isActive() const;
	
private:
	void drawToolTip(string _tip);
	void bindEvents();
	void unbindEvents();
	
	list<movablePoint>* children;
	bool isEditable;
	bool active;
	bool isParentOfOthers;
	bool enabled;
	bool mouseHold;
	ofColor color;
	void _mousePressed(ofMouseEventArgs &e);
	void _mouseMoved(ofMouseEventArgs &e);
	void _mouseDragged(ofMouseEventArgs &e);
	void _mouseReleased(ofMouseEventArgs &e);
	
};