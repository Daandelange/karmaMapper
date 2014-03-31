//
//  movablePoint.cpp
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 24/2/14.
//  Utility class for creating draggable points
//

#include "movablePoint.h"
#define MP_POINT_SIZE 5

movablePoint::movablePoint(){
	//printf("movablePoint::movablePoint() - hello!\n");

}

movablePoint::~movablePoint(){
	disableInteraction();
}

void movablePoint::setup() {
	//printf("movablePoint::setup() - hello!\n");
	
	//set( _x, _y, 10, 10);
	isEditable = false;
	isParentOfOthers = false;
	color = ofColor(255,255,255);
	isActive = false;
	
	// enable ofxMSAInteractiveObject functions
	enableInteraction();
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
		
		// draw dragging zone
		if( isMouseOver() ){
			ofSetColor(color, 100);
			if( isMousePressed() ) ofSetColor(color, 200);
			
			ofCircle( getCenter(), (width+height)/2 );
		}
		
		// draw point or cross
		ofSetColor( color );
		if( isActive ) ofSetColor(255, 50, 50);
		
		if(isParentOfOthers){
			ofPoint c = getCenter();
			ofLine( c.x-MP_POINT_SIZE, c.y, c.x+MP_POINT_SIZE, c.y );
			ofLine( c.x, c.y-MP_POINT_SIZE, c.x, c.y+MP_POINT_SIZE );
		}
		else ofCircle( getCenter(), MP_POINT_SIZE);
	}
	
	//ofPopStyle();
}

void movablePoint::enableInteraction(){
	disableAllEvents();
	enableMouseEvents();
	//enableAppEvents();
}

void movablePoint::disableInteraction(){
	disableAllEvents();
}

void movablePoint::setEditable(bool status){
	// lets parent triangle set edit state of its movablePoints
	this->isEditable = status;
	//cout << isEditable << endl;
	
	isEditable?enableInteraction():disableInteraction();
}

void movablePoint::makeParent(vector<movablePoint>& _children){
	isParentOfOthers = true;
	
	// keep pointer reference to children
	children = &_children;
	
	//cout << children->size() << endl;
}

void movablePoint::removeChildren(){
	isParentOfOthers = false;
	children->clear();
	
}

void movablePoint::makeActive(){
	isActive = true;
}

void movablePoint::notActive(){
	isActive = false;
}

ofPoint movablePoint::getPos(){
	return ofPoint(this->x+this->width/2,this->y+this->height/2);
	//ofPoint p = ofPoint(x+width/2,y+height/2);
	//cout << getPosition().x << " - ";
	//return position;
}

void movablePoint::setPosSize(ofPoint _pos, int _diameter){
	ofPoint _size(_diameter, _diameter);
	ofRectangle::setPosition( _pos - _size/2 );
	ofRectangle::setWidth( _size.x );
	ofRectangle::setHeight( _size.y );
}

void movablePoint::setPos(ofPoint _pos){
	ofRectangle::setPosition( _pos - ((width+height)/2)/2 );
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
	
	// prevent editing ?
	if(!isEditable) return;
	
	// get new pos
	float tmpX = x - width/2;
	float tmpY = y - height/2;
	
	if( isParentOfOthers && children->size()>1) {
		//movablePoint<int>::iterator<int,movablePoint> = &children;
		//std::set< movablePoint*>::iterator caca = children->begin();
		ofPoint dif( tmpX  - this->x, tmpY - this->y);
		
		// update children positions
		for ( vector<movablePoint>::iterator p = children->begin(); p != children->end()-1; ++p){
			
			p->translate( dif );
		}
		
		// also save its own position
		this->x = tmpX;
		this->y = tmpY;
	}
	else {
		this->x = tmpX;
		this->y = tmpY;
	}
	
}
	
void movablePoint::onDragOutside(int x, int y, int button) {
	//printf("movablePoint::onDragOutside(x: %i, y: %i, button: %i)\n", x, y, button);
	
	// simply
	onDragOver(x, y, button);
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

