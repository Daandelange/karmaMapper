//
//  illuminatiTriangle.cpp
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 24/2/14.
//
//

#include "movablePoint.h"

movablePoint::movablePoint(){
	//printf("movablePoint::movablePoint() - hello!\n");
	
	// enable ofxMSAInteractiveObject functions
	enableInteraction();
	
	//set( _x, _y, 10, 10);
	
	//hovered = false;
	//selected = false;
	isEditable = false;
	color = ofColor(255,255,255);
	
	// spawn at random location
	// todo: spawn at saved position
	
}

movablePoint::~movablePoint(){
	disableInteraction();
}

void movablePoint::setup() {
	//printf("movablePoint::setup() - hello!\n");
	
}	
	

void movablePoint::exit() {
	
}
	
	
void movablePoint::update() {
	//cout << isMouseOver() << endl;
}
	
	
void movablePoint::draw() {
	//cout << "movablePoint::draw() [" << isEditable << "]" << endl;
	//cout << isEditable << "::" << position << " • " << width << "*" << height << endl;
	//ofPushStyle();
	
	if(isEditable){
		if(isMousePressed()) ofSetColor(color, 255);
		else if(isMouseOver()) ofSetColor(color, 255);
		else ofSetColor(color, 30);
	
		ofFill();
		ofCircle(x+width/2, y+height/2, (width+height)/2/2);
	}
	
	//ofPopStyle();
}

void movablePoint::enableInteraction(){
	disableAllEvents();
	enableMouseEvents();
	enableAppEvents();
}

void movablePoint::disableInteraction(){
	disableMouseEvents();
}

void movablePoint::setEditable(bool status){
	// lets parent triangle set edit state of its movablePoints
	this->isEditable = status;
	//cout << isEditable << endl;
	
	isEditable?enableInteraction():disableInteraction();
}

ofPoint movablePoint::getPos(){
	return ofPoint(this->x+this->width/2,this->y+height/2);
	//ofPoint p = ofPoint(x+width/2,y+height/2);
	//cout << getPosition().x << " - ";
	//return position;
}
	
void movablePoint::onRollOver(int x, int y) {
	//printf("movablePoint::onRollOver(x: %i, y: %i)\n", x, y);
}
	
void movablePoint::onRollOut() {
	
}
	
void movablePoint::onMouseMove(int x, int y){
	//hovered=true;
	//printf("movablePoint::onMouseMove(x: %i, y: %i)\n", x, y);
}
	
void movablePoint::onDragOver(int x, int y, int button) {
	//printf("movablePoint::onDragOver(x: %i, y: %i, button: %i)\n", x, y, button);
	
	if(!isEditable) return;
	
	this->x = x - width/2;
	this->y = y - height/2;
}
	
void movablePoint::onDragOutside(int x, int y, int button) {
	//printf("movablePoint::onDragOutside(x: %i, y: %i, button: %i)\n", x, y, button);
	
	if(!isEditable) return;
	
	this->x = x - width/2;
	this->y = y - height/2;
	
	//updateParentTriangle();
}
	
void movablePoint::onPress(int x, int y, int button) {
	//printf("movablePoint::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
}
	
void movablePoint::onRelease(int x, int y, int button) {
	//printf("movablePoint::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
}
	
void movablePoint::onReleaseOutside(int x, int y, int button) {
	//printf("movablePoint::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
}
	
void movablePoint::keyPressed(int key) {
	//printf("movablePoint::keyPressed(key: %i)\n", key);
}
	
void movablePoint::keyReleased(int key) {
	//printf("movablePoint::keyReleased(key: %i)\n", key);
}

