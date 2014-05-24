//
//  vertexShape.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"

class vertexShape : public basicShape {
	
public:
	vertexShape();
	~vertexShape();
	
	// inherited by basicShape
	void spawn();
	//void reset();
	void destroy();
	void drawWireframe();
	void editModeChanged(bool editMode);
	void pointsUpdated();
	
	void selectNextHandle();
	void selectHandle(int _i);
	void translateActiveHandle(ofPoint _offset);
	bool isSelectableItem(int _i);
	
	// custom functions
	void _mousePressed( ofMouseEventArgs& args );
	void _mouseDragged( ofMouseEventArgs& args );
	void _mouseReleased( ofMouseEventArgs& args );
	
private:
	//bool hovered, selected;
	//unsigned int numPoints;
	
	
	
};
