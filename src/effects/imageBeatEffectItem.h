//
//  imageBeatEffectItem.h
//  workshopMappingCyril
//
//  Created by Daan de Lange on 14/12/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "vertexShape.h"

#define LEL_LIFE_SPAN 1

class imageBeatEffectItem {

public:
	imageBeatEffectItem(basicShape* _shape);
	~imageBeatEffectItem();
	
	void render();
	void render(float state);
	bool isAlive() const;
	
protected:
	ofVec2f* center;
	ofVec2f position;
	ofTexture image;
	
	float startTime;
	bool bAlive = false;

private:
	
};
