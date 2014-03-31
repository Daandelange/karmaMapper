//
//  vectorShape.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"

class vectorShape : public basicShape {
	
public:
	vectorShape();
	~vectorShape();
	
	// inherited by basicShape
	void spawn();
	//void reset();
	void destroy();
	void drawWireframe();
	void editModeChanged(bool editMode);
	void pointsUpdated();
	
	bool isSelectableItem(int _i);
	
private:
	//bool hovered, selected;
	unsigned int numPoints;
	
};
