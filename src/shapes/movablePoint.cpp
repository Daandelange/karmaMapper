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
	setup();
}

movablePoint::~movablePoint(){
	//disableInteraction();

	unbindEvents();
}

void movablePoint::setup() {
	isEditable = false;
	isParentOfOthers = false;
	color = ofColor(255,255,255);
	children = NULL;
	mouseHold = false;
	
	blur();
	enable();
	
	// enable ofxMSAInteractiveObject functions
	//enableInteraction();
	
	/*/ bind event listeners
	ofAddListener(ofEvents().mousePressed, this, &movablePoint::_mousePressed);
	ofAddListener(ofEvents().mouseMoved, this, &movablePoint::_mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &movablePoint::_mouseDragged);
	
	ofAddListener(ofEvents().mouseReleased, this, &movablePoint::_mouseReleased);*/
}	
	

void movablePoint::exit() {
	
}
	
	
void movablePoint::update() {
	
}
	
	
void movablePoint::draw() {
	if(!enabled) return;
	
	if(isEditable){
		
		ofPushStyle();
		// draw dragging zone
		if( isMouseOver() ){
			// todo: make this a cross instead of a circle
			ofSetColor(color, 100);
			if( isMousePressed() ) ofSetColor(color, 50);
			
			ofDrawCircle( center, MP_POINT_SIZE );
		}
		
		// draw point or cross
		ofSetColor( color );
		if( active ) ofSetColor(255, 50, 50);
		
		if(isMousePressed() || active ){
			//ofVec2f c = center;
			ofDrawLine( center.x-MP_POINT_SIZE, center.y, center.x+MP_POINT_SIZE, center.y );
			ofDrawLine( center.x, center.y-MP_POINT_SIZE, center.x, center.y+MP_POINT_SIZE );
		}
		else if(isParentOfOthers){
			ofFill();
			ofDrawCircle( center, MP_POINT_SIZE*2);
			ofNoFill();
			ofSetColor( color.getInverted() );
			ofDrawLine( center.x-MP_POINT_SIZE, center.y, center.x+MP_POINT_SIZE, center.y );
			ofDrawLine( center.x, center.y-MP_POINT_SIZE, center.x, center.y+MP_POINT_SIZE );
		}
		else ofDrawCircle( center, MP_POINT_SIZE);
		
		// draw tool tips ?
		if(isParentOfOthers && (isMousePressed() || active)) drawToolTip("Hold alt to move only the anchor point.");
		
		ofPopStyle();
	}
	
	//ofPopStyle();
}

void movablePoint::setEditable(bool status){
	// lets parent triangle set edit state of its movablePoints
	this->isEditable = status;
	
	// (un)bind events
	status?bindEvents():unbindEvents();
}

void movablePoint::makeParent(list<movablePoint>& _children){
	isParentOfOthers = true;
	
	// keep pointer reference to children
	children = &_children;
	
	//cout << children->size() << endl;
}

void movablePoint::removeChildren(){
	isParentOfOthers = false;
	//children->clear();
	children = NULL;
	
}

void movablePoint::focus(){
	active = true;
}

void movablePoint::blur(){
	active = false;
}



void movablePoint::enable(){
	enabled = true;
}

void movablePoint::disable(){
	enabled = false;
}

bool movablePoint::isEnabled() const {
	return enabled;
}

ofVec2f movablePoint::getPos(){
	//return ofPoint(center.x+MP_POINT_SIZE/2,center.y+MP_POINT_SIZE/2);
	return center;
	//ofPoint p = ofPoint(x+width/2,y+height/2);
	//cout << getPosition().x << " - ";
	//return position;
}

/*void movablePoint::setPosSize(ofPoint _pos, int _diameter){
	ofPoint _size(_diameter, _diameter);
	
	
	ofRectangle::setWidth( _size.x );
	ofRectangle::setHeight( _size.y );
}*/

void movablePoint::setPos(ofVec2f _pos){
	// note: hold alt not to alter other points
	if( isParentOfOthers && children->size()>0 && !ofGetKeyPressed(OF_KEY_ALT) ) {
		ofVec2f dif( _pos - center);
		
		// update children positions
		for ( list<movablePoint>::iterator p = children->begin(); p != children->end(); ++p)
			p->translate( dif );
	}
	
	// update position of element
	center = _pos;
}

void movablePoint::translate(ofVec2f _offset){
	// prevent editing ?
	if(!isEditable) return;
	
	_offset += getPos();
	this->setPos( _offset );
}

bool movablePoint::isMouseOver(){
	// mouse over is defined by the distance from center (circle)
	return ofDist(center.x, center.y, ofGetMouseX(), ofGetMouseY()) <= MP_POINT_SIZE;
}

void movablePoint::drawToolTip(string _tip){
	//tool for showing short informational info
	ofSetColor(0,180);
	ofFill();
	ofDrawRectangle(0,0,ofGetWidth(),30);
	ofSetColor(255);
	ofNoFill();
	ofDrawBitmapString(_tip, 10, 20);
}

void movablePoint::bindEvents(){
	ofAddListener(ofEvents().mousePressed, this, &movablePoint::_mousePressed);
	ofAddListener(ofEvents().mouseMoved, this, &movablePoint::_mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &movablePoint::_mouseDragged);
	
	ofAddListener(ofEvents().mouseReleased, this, &movablePoint::_mouseReleased);
}

void movablePoint::unbindEvents(){
	ofRemoveListener(ofEvents().mousePressed, this, &movablePoint::_mousePressed);
	ofRemoveListener(ofEvents().mouseMoved, this, &movablePoint::_mouseMoved);
	ofRemoveListener(ofEvents().mouseDragged, this, &movablePoint::_mouseDragged);
	ofRemoveListener(ofEvents().mouseReleased, this, &movablePoint::_mouseReleased);
}

// todo: make this with an automatic canceling timer ?
bool movablePoint::isMousePressed(){
	return (enabled && mouseHold);
}

bool movablePoint::isActive() const {
	return active;
}

void movablePoint::_mousePressed(ofMouseEventArgs &e){
	// prevent editing ?
	if(!isEditable) return;
	
	// is the zone hovered ?
	if(e.button==0 && isMouseOver()){
		//int x = e.x;
		//int y = e.y;
		//int button = e.button;
		
		// track mouse clicks
		mouseHold=true;
		ofHideCursor();
		focus();
	}
}

void movablePoint::_mouseMoved(ofMouseEventArgs &e){
	// prevent editing ?
	if(!isEditable) return;
	
	//if(isMouseOver()) printf("movablePoint::_mouseMoved(key: %i)\n", e.button);
	
	if(!isMousePressed()) return;
	
	// get new pos
	setPos(ofVec2f(e.x,e.y));
}

void movablePoint::_mouseDragged(ofMouseEventArgs &e){
	// prevent editing ?
	if(!isEditable) return;
	
	// transfer to moved event
	_mouseMoved(e);
}

void movablePoint::_mouseReleased(ofMouseEventArgs &e){
	// prevent editing ?
	if(!isEditable) return;
	
	//printf("movablePoint::_mouseReleased(key: %i)\n", e.button);
	
	// keep track of mouse pression
	if(e.button==0){
		// reset active state (focus() called on click)
		if(mouseHold) blur();
		
		mouseHold = false;
		ofShowCursor();
	}
}
