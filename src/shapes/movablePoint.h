//
//  movablePoint.h
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 24/2/14.
//
//

#pragma once

#include "ofxMSAInteractiveObject.h"


class movablePoint : public ofxMSAInteractiveObject {
	
public:
	movablePoint();
	~movablePoint();
	
	void setup();
	void exit();
	void update();
	void draw();
	
	void enableInteraction();
	void disableInteraction();
	
	void setEditable( bool status );
	
	void makeParent( vector<movablePoint>& _children );
	void removeChildren();
	
	void makeActive();
	void notActive();
	
	ofPoint getPos();
	void setPosSize(ofPoint _pos, int _diameter);
	void setPos(ofPoint _pos);
	//void setPosRelative(ofPoint _pos);
	//int x,y,width,height;

	virtual void onRollOver(int x, int y);
	virtual void onRollOut();
	virtual void onMouseMove(int x, int y);
	virtual void onDragOver(int x, int y, int button);
	virtual void onDragOutside(int x, int y, int button);
	virtual void onPress(int x, int y, int button);
	virtual void onRelease(int x, int y, int button);
	virtual void onReleaseOutside(int x, int y, int button);
	virtual void keyPressed(int key);
	virtual void keyReleased(int key);
	
private:
	vector<movablePoint>* children;
	bool isEditable;
	bool isActive;
	bool isParentOfOthers;
	ofColor color;
	
};