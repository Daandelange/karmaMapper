//
//  lineEffectLine.h
//  karmaMapper
//
//  Created by Daan de Lange on 14/12/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "shapes.h"

#define LEL_LIFE_SPAN 1

// todo: implement (animation) modes or a random preset generator

class lineEffectLine {

public:
	lineEffectLine( ofVec2f* _from, ofVec2f* _to);
	lineEffectLine( basicShape* targetShape );
	lineEffectLine( vertexShape* targetShape );
	~lineEffectLine();
	
	void render();
	void render(float state);
	bool isAlive() const;
	
protected:
	ofVec2f* posFrom;
	ofVec2f* posTo;
	ofColor color;
	float startTime;
	bool bAlive;

private:
	basicShape* rememberShape = NULL;
};
