//
//  basicPoint.h
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 24/2/14.
//
//

#pragma once

#include "KMSettings.h"
#include "ofMain.h"


// needed forward declaration
class basicShape;

enum basicShapePointType {
	POINT_POSITION_RELATIVE,
	POINT_POSITION_RELATIVE_UNALTERED,
	POINT_POSITION_ABSOLUTE
};

// todo: rename basicPoint so shapePoint

class basicPoint {
	
public:
	basicPoint();
	basicPoint(float _f);
	basicPoint(float _x, float _y);
	basicPoint(const ofPoint& p);
	basicPoint(const ofVec2f& p);
	~basicPoint();
	
	// #########
	// global variables
	static basicPoint nullPoint;
	
	float x;
	float y;
	
	inline bool operator ==( const basicPoint& p ) const;
	inline bool operator !=( const basicPoint& p ) const;
	
	// conversion
	basicPoint operator =( const ofVec2f& p ) const;
	basicPoint operator =( const ofPoint& p ) const;
	basicPoint operator =( const glm::vec2& p ) const;
	basicPoint operator =( const glm::vec3& p ) const;
	operator ofVec2f() const { return ofVec2f(x,y); }
	operator ofPoint() const { return ofPoint(x,y,0); }
	operator glm::vec2() const { return glm::vec2(x,y); }
	operator glm::vec3() const { return glm::vec3(x,y,0); }
	
	
	basicPoint operator + ( const basicPoint& p ) const;
	basicPoint operator - ( const basicPoint& p ) const;
	basicPoint operator * ( const basicPoint& p ) const;
	basicPoint operator / ( const basicPoint& p ) const;

	
	basicPoint& operator += ( const basicPoint& p );
	basicPoint& operator += ( const float& f );
	basicPoint& operator -= ( const basicPoint& p );
	basicPoint& operator -= ( const float& f );
	basicPoint& operator *= ( const basicPoint& p );
	basicPoint& operator *= ( const float& f );
	basicPoint& operator /= ( const basicPoint& p );
	basicPoint& operator /= ( const float& f );
	
#ifdef KM_EDITOR_APP
	void initEditorVariables();
	void draw();
	
	// todo: rm these functionality ?
	void makeParent( list<basicPoint>& _children );
	void removeChildren();
	
	void setShape( basicShape& _shape, bool _isPositionRelative = false );
	void unbindShape();
	
	void focus();
	void blur();
	
	void setEditable(bool status);
	ofVec2f getPos() const;
	void setPos(const basicPoint& _pos);
	void setPos(float _x, float _y);
	void translate(const basicPoint& _offset);
	void translate(const float _xOffset, const float _yOffset);
	bool isMouseOver() const;
	bool isMouseOver( const float _x, const float _y ) const;
	bool isFocused() const;
	
	// listeners
	bool interceptMouseClick(ofMouseEventArgs &e);
	void _mouseDragged(ofMouseEventArgs &e);
	void _mouseReleased(ofMouseEventArgs &e);
	
	static unsigned int pointSize;
	
private:
	// utility
	void drawToolTip(const string _tip);
	
	// mouse events
	void bindMouseEvents();
	void unbindMouseEvents();
	bool isEventBound;
	
	bool isEditable;
	bool isActive;
	bool isParentOfOthers;
	bool hasParentShape;
	bool isMouseHold;
	
	list<basicPoint>* children; // rm this ?
	basicShape* parentShape;
	bool isPositionRelative;
	
	ofColor color;
#endif
};


// - - - - - - -
// OPERATORS

// check equality
inline bool basicPoint::operator==( const basicPoint& p ) const {
	return (x == p.x) && (y == p.y);
}

inline bool basicPoint::operator!=( const basicPoint& p ) const {
	return (x != p.x) || (y != p.y);
}

// conversion
inline basicPoint basicPoint::operator =( const ofVec2f& p ) const{
	return basicPoint(x-p.x, y-p.y); // todo: is the minus correct???
}
inline basicPoint basicPoint::operator =( const ofPoint& p ) const {
	return basicPoint(x-p.x, y-p.y); // todo: is the minus correct???
}
inline basicPoint basicPoint::operator =( const glm::vec2& p ) const{
	return basicPoint(p.x, p.y);
}
inline basicPoint basicPoint::operator =( const glm::vec3& p ) const{
	return basicPoint(p.x, p.y);
}

// operations
inline basicPoint basicPoint::operator +( const basicPoint& p ) const {
	return basicPoint( x+p.x, y+p.y);
}

inline basicPoint basicPoint::operator -( const basicPoint& p ) const {
	return basicPoint( x+p.x, y+p.y);
}

inline basicPoint basicPoint::operator *( const basicPoint& p ) const {
	return basicPoint( x*p.x, y*p.y);
}

inline basicPoint basicPoint::operator /( const basicPoint& p ) const {
	return basicPoint( x/p.x, y/p.y);
}

// overloading
inline basicPoint& basicPoint::operator +=( const basicPoint& p ) {
	x += p.x;
	y += p.y;
	return *this;
}

inline basicPoint& basicPoint::operator +=( const float& f ) {
	x += f;
	y += f;
	return *this;
}

inline basicPoint& basicPoint::operator -=( const basicPoint& p ) {
	x -= p.x;
	y -= p.y;
	return *this;
}

inline basicPoint& basicPoint::operator -=( const float& f ) {
	x -= f;
	y -= f;
	return *this;
}

inline basicPoint& basicPoint::operator *=( const basicPoint& p ) {
	x *= p.x;
	y *= p.y;
	return *this;
}

inline basicPoint& basicPoint::operator *=( const float& f ) {
	x *= f;
	y *= f;
	return *this;
}

inline basicPoint& basicPoint::operator /=( const basicPoint& p ) {
	x /= p.x;
	y /= p.y;
	return *this;
}
inline basicPoint& basicPoint::operator /=( const float& f ) {
	x /= f;
	y /= f;
	return *this;
}
