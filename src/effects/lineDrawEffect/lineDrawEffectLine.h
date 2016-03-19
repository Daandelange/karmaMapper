//
//  lineDrawEffectLine.h
//  karmaMapper
//
//  Created by Daan de Lange on 14/12/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "mirEvents.h"

// todo: implement (animation) modes or a random preset generator

struct linePoints {
	linePoints(){
		shape = nullptr;
		points.clear();
	}
	
	basicShape* shape;
	vector< basicPoint* > points;
};

class lineDrawEffectLine {

public:
	lineDrawEffectLine( basicPoint* _from, basicPoint* _to);
	lineDrawEffectLine( basicShape* targetShape );
	lineDrawEffectLine( vertexShape* targetShape, float _lifeTime, ofColor _color );
	~lineDrawEffectLine();
	
	void render();
	void render(float state);
	bool isAlive() const;
	
protected:
	linePoints points;
	ofColor color;
	float startTime;
	float lifeTime = 1.0f;
	bool bAlive;

private:
	basicShape* rememberShape = NULL;
};
