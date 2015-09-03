//
//  basicPoint.cpp
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 24/2/14.
//  Utility class for creating draggable points
//

#include "basicPoint.h"
#define KM_BASIC_POINT_SIZE 5

basicPoint::basicPoint(){
	basicPoint(0,0);
}

basicPoint::basicPoint(float _f){
	basicPoint(_f, _f);
}

basicPoint::basicPoint(float _x, float _y): x(_x), y(_y){
	
#ifdef KM_EDITOR_APP
	isEditable = true;
	isParentOfOthers = false;
	isActive = false;
	isMouseHold = false;
	color = ofColor(255,255,255);
	children = NULL;
	isEventBound = false;
#endif
}

basicPoint::~basicPoint(){

#ifdef KM_EDITOR_APP
	unbindMouseEvents();
#endif
}

// - - - - - - - - - - -

#ifdef KM_EDITOR_APP

void basicPoint::draw() {
	if( !isEditable ) return;
	
	ofPushStyle();
	// draw dragging zone
	if( isMouseOver() || isMouseHold ){
		// todo: make this a cross instead of a circle
		ofSetColor(color, 100);
		if( isMouseHold ) ofSetColor(color, 50);
		
		ofDrawCircle( x, y, KM_BASIC_POINT_SIZE );
	}
	
	// draw point or cross
	ofSetColor( color );
	if( isActive ) ofSetColor(255, 50, 50);
	
	if( isMouseHold || isActive ){
		ofDrawLine( x-KM_BASIC_POINT_SIZE, y, x+KM_BASIC_POINT_SIZE, y );
		ofDrawLine( x, y-KM_BASIC_POINT_SIZE, x, y+KM_BASIC_POINT_SIZE );
	}
	else if(isParentOfOthers){
		ofFill();
		ofDrawCircle( x, y, KM_BASIC_POINT_SIZE*2);
		ofNoFill();
		ofSetColor( color.getInverted() );
		ofDrawLine( x - KM_BASIC_POINT_SIZE, y, x + KM_BASIC_POINT_SIZE, y );
		ofDrawLine( x, y - KM_BASIC_POINT_SIZE, x, y + KM_BASIC_POINT_SIZE );
	}
	else ofDrawCircle( x, y, KM_BASIC_POINT_SIZE);
	
	// draw tool tips ?
	if(isParentOfOthers && (isMouseHold || isActive)) drawToolTip("Hold alt to move only the anchor point.");
	
	ofPopStyle();
}

void basicPoint::setEditable(bool status){
	isEditable = status;
}

void basicPoint::makeParent(list<basicPoint>& _children){
#ifdef KM_LOCATE_PROBLEMS
	if( !children ) ofLogWarning("basicPoint::makeParent") << "Setting parent but already set... (continuing)";
#endif
	
	isParentOfOthers = true;
	
	// keep pointer reference to children
	children = &_children;
}

void basicPoint::removeChildren(){
	isParentOfOthers = false;
	children = NULL; // don't delete cause we only borrow the instance
}

void basicPoint::focus(){
	isActive = true;
}

void basicPoint::blur(){
	isActive = false;
}

ofVec2f basicPoint::getPos() const{
	return ofVec2f(x,y);
}

void basicPoint::setPos(const basicPoint& _pos){
	setPos( _pos.x, _pos.y);
}

void basicPoint::setPos( float _x, float _y){
	// note: hold alt not to alter other points
	if( isParentOfOthers && children->size()>0 && !ofGetKeyPressed(OF_KEY_ALT) ) {
		float difX( _x - x);
		float difY( _y - y);
		
		// update children positions
		for ( auto p = children->begin(); p != children->end(); ++p)
			p->translate( difX, difY );
	}
	
	// update position of element
	x = _x;
	y = _y;
}

void basicPoint::translate(const basicPoint& _offset){
	translate( _offset.x, _offset.y );
}

void basicPoint::translate(const float _xOffset, const float _yOffset){
	setPos( _xOffset + x,  _yOffset + y );
}

bool basicPoint::isMouseOver() const{
	return isMouseOver( ofGetMouseX(), ofGetMouseY() );
}

bool basicPoint::isMouseOver(const float _x, const float _y) const{
	// mouse over is defined by the distance from center (circle)
	return ofDist(x, y, _x, _y) <= KM_BASIC_POINT_SIZE;
}

bool basicPoint::isFocused() const {
	return isActive;
}

// - - - - -
// Utility

void basicPoint::drawToolTip( const string _tip){
	//tool for showing short informational info
	ofSetColor(0,180);
	ofFill();
	ofDrawRectangle(0,0,ofGetWidth(),30);
	ofSetColor(255);
	ofNoFill();
	ofDrawBitmapString(_tip, 10, 20);
}

// - - - - -
// Mouse Events

void basicPoint::bindMouseEvents(){
	if(isEventBound) return;
	
	ofAddListener(ofEvents().mouseReleased, this, &basicPoint::_mouseReleased);
	ofAddListener(ofEvents().mouseDragged, this, &basicPoint::_mouseDragged);
	isEventBound = true;
}

void basicPoint::unbindMouseEvents(){
	if(!isEventBound) return;
	
	ofRemoveListener(ofEvents().mouseDragged, this, &basicPoint::_mouseDragged);
	ofRemoveListener(ofEvents().mouseReleased, this, &basicPoint::_mouseReleased);
	isEventBound = false;
}

bool basicPoint::interceptMouseClick(ofMouseEventArgs &e){
	// prevent editing ?
	if(!isEditable) return;
	
	// is the zone hovered ?
	if(e.button==0 && isMouseOver()){
		focus();
		
		// track mouse clicks
		isMouseHold=true;
		bindMouseEvents();
	}
}

void basicPoint::_mouseDragged(ofMouseEventArgs &e){
	
	if( isEditable && e.button==0 && isMouseHold){
		setPos(e.x, e.y);
	}
}

void basicPoint::_mouseReleased(ofMouseEventArgs &e){
	// keep track of mouse pression
	if( e.button==0){
		// reset active state (focus() called on click)
		if(isMouseHold) blur();
		isMouseHold = false;
		
		unbindMouseEvents();
	}
}
#endif