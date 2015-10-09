//
//  basicPoint.cpp
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 24/2/14.
//  Utility class for creating draggable points
//

#include "basicPoint.h"
#define KM_BASIC_POINT_SIZE 5

#ifdef KM_EDITOR_APP
// for universal access
unsigned int basicPoint::pointSize = KM_BASIC_POINT_SIZE;

// needed forward declaration
class basicShape {
public:
	void onShapeEdited();
	basicPoint* getPositionUnaltered();
};
#endif

basicPoint::basicPoint(): x(0), y(0){
	//cout << "basicPoint::basicPoint()" << endl;
#ifdef KM_EDITOR_APP
	initEditorVariables();
#endif
}

basicPoint::basicPoint(float _f): x(_f), y(_f){
#ifdef KM_EDITOR_APP
	initEditorVariables();
#endif
}

basicPoint::basicPoint(float _x, float _y): x(_x), y(_y){
	//cout << "basicPoint::basicPoint(0,0)" << endl;
#ifdef KM_EDITOR_APP
	initEditorVariables();
#endif
}

basicPoint::~basicPoint(){
	//cout << "basicPoint::~basicPoint()" << endl;
#ifdef KM_EDITOR_APP
	unbindMouseEvents();
#endif

//#ifdef KM_LOG_INSTANCIATIONS
//	ofLogNotice("basicPoint::~basicPoint") << " [" << &*this << "]";
//#endif
}

// - - - - - - - - - - -

#ifdef KM_EDITOR_APP

void basicPoint::initEditorVariables(){
	isEditable = false;
	isParentOfOthers = false;
	isActive = false;
	isMouseHold = false;
	color = ofColor(255,255,255);
	children = NULL;
	isEventBound = false;
	parentShape = NULL;
	isPositionRelative = false;
	hasParentShape = false;
}

void basicPoint::draw() {
	if( !isEditable ) return;

	float absX(x);
	float absY(y);
	if(hasParentShape && parentShape && isPositionRelative){
		absX += parentShape->getPositionUnaltered()->x;
		absY += parentShape->getPositionUnaltered()->y;
	}
	
	ofPushStyle();
	// draw dragging zone
	if( isMouseOver() || isMouseHold ){
		// todo: make this a cross instead of a circle
		ofSetColor(color, 100);
		if( isMouseHold ) ofSetColor(color, 50);
		
		ofDrawCircle( absX, absY, KM_BASIC_POINT_SIZE );
	}
	
	// draw point or cross
	ofSetColor( color );
	if( isActive ) ofSetColor(255, 50, 50);
	
	if( isMouseHold || isActive ){
		ofDrawLine( absX-KM_BASIC_POINT_SIZE, absY, absX+KM_BASIC_POINT_SIZE, absY );
		ofDrawLine( absX, absY-KM_BASIC_POINT_SIZE, absX, absY+KM_BASIC_POINT_SIZE );
	}
	else if(isParentOfOthers){
		ofFill();
		ofDrawCircle( absX, absY, KM_BASIC_POINT_SIZE*2);
		ofNoFill();
		ofSetColor( color.getInverted() );
		ofDrawLine( absX - KM_BASIC_POINT_SIZE, absY, absX + KM_BASIC_POINT_SIZE, absY );
		ofDrawLine( absX, absY - KM_BASIC_POINT_SIZE, absX, absY + KM_BASIC_POINT_SIZE );
	}
	else ofDrawCircle( absX, absY, KM_BASIC_POINT_SIZE);
	
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

void basicPoint::setShape(basicShape &_shape, bool _isPositionRelative){
	parentShape = &_shape;
	hasParentShape = true;
	isPositionRelative = _isPositionRelative;
}

void basicPoint::unbindShape(){
	parentShape = NULL;
	hasParentShape = false;
	isPositionRelative = false;
	
	// todo: removeChildren() here ?
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
	
	if( hasParentShape && parentShape) parentShape->onShapeEdited();
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
	float absX(x);
	float absY(y);
	if(hasParentShape && parentShape && isPositionRelative){
		absX += parentShape->getPositionUnaltered()->x;
		absY += parentShape->getPositionUnaltered()->y;
	}
	// mouse over is defined by the distance from center (circle)
	return ofDist(absX, absY, _x, _y) <= KM_BASIC_POINT_SIZE;
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
	if(!isEditable) return false;
	
	// is the zone hovered ?
	if(e.button==0 && isMouseOver()){
		focus();
		
		// track mouse clicks
		isMouseHold=true;
		bindMouseEvents();
		return true;
	}
	
	return false;
}

void basicPoint::_mouseDragged(ofMouseEventArgs &e){
	
	if( isEditable && e.button==0 && isMouseHold){
		if(hasParentShape && parentShape && isPositionRelative){
			setPos(
				   e.x - parentShape->getPositionUnaltered()->x,
				   e.y - parentShape->getPositionUnaltered()->y
			);
		}
		else setPos(e.x, e.y);
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