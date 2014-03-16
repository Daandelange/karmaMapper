//
//  illuminatiTriangle.h
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 24/2/14.
//
//

#pragma once

//#include <iostream>
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
	
	ofPoint getPos();
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
	
	bool isEditable;
	
private:
	//bool hovered, selected;
	ofColor color;
	
};