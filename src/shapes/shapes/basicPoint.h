//
//  basicPoint.h
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 24/2/14.
//
//

#pragma once

#include "ofMain.h"
#include "KMSettings.h"

class basicPoint {
	
public:
	basicPoint();
	basicPoint(float _f);
	basicPoint(float _x, float _y);
	~basicPoint();
	
	float x;
	float y;
	
	inline bool operator==( const basicPoint& p ) const;
	inline bool operator!=( const basicPoint& p ) const;
	
	basicPoint operator+( const basicPoint& p ) const;
	basicPoint operator-( const basicPoint& p ) const;
	basicPoint operator*( const basicPoint& p ) const;
	basicPoint operator/( const basicPoint& p ) const;

	
	basicPoint& operator+=( const basicPoint& p );
	basicPoint& operator+=( const float& f );
	basicPoint& operator-=( const basicPoint& p );
	basicPoint& operator-=( const float& f );
	basicPoint& operator*=( const basicPoint& p );
	basicPoint& operator*=( const float& f );
	basicPoint& operator/=( const basicPoint& p );
	basicPoint& operator/=( const float& f );
	
#ifdef KM_EDITOR_APP
	void draw();
	
	void makeParent( list<basicPoint>& _children );
	void removeChildren();
	
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
	list<basicPoint>* children;

	bool isMouseHold;
	ofColor color;
	bool interceptMouseClick(ofMouseEventArgs &e);
	void _mouseDragged(ofMouseEventArgs &e);
	void _mouseReleased(ofMouseEventArgs &e);
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

// operations
basicPoint basicPoint::operator+( const basicPoint& p ) const {
	return basicPoint( x+p.x, y+p.y);
}

basicPoint basicPoint::operator-( const basicPoint& p ) const {
	return basicPoint( x+p.x, y+p.y);
}

basicPoint basicPoint::operator*( const basicPoint& p ) const {
	return basicPoint( x*p.x, y*p.y);
}

inline basicPoint basicPoint::operator/( const basicPoint& p ) const {
	return basicPoint( x/p.x, y/p.y);
}

// overloading
basicPoint& basicPoint::operator+=( const basicPoint& p ) {
	x += p.x;
	y += p.y;
	return *this;
}

inline basicPoint& basicPoint::operator+=( const float& f ) {
	x += f;
	y += f;
	return *this;
}

inline basicPoint& basicPoint::operator-=( const basicPoint& p ) {
	x -= p.x;
	y -= p.y;
	return *this;
}

inline basicPoint& basicPoint::operator-=( const float& f ) {
	x -= f;
	y -= f;
	return *this;
}

basicPoint& basicPoint::operator*=( const basicPoint& p ) {
	x *= p.x;
	y *= p.y;
	return *this;
}

inline basicPoint& basicPoint::operator*=( const float& f ) {
	x *= f;
	y *= f;
	return *this;
}

inline basicPoint& basicPoint::operator/=( const basicPoint& p ) {
	x /= p.x;
	y /= p.y;
	return *this;
}
inline basicPoint& basicPoint::operator/=( const float& f ) {
	x /= f;
	y /= f;
	return *this;
}