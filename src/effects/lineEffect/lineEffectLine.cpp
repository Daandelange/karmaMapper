//
//  lineEffectLine.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 14/12/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "lineEffectLine.h"

lineEffectLine::lineEffectLine(basicPoint* _from, basicPoint* _to) {
	bAlive = true;
	startTime = ofGetElapsedTimef();
	
	//color.setHsb(ofRandom(255), 200, 200);
	color.set(255);
	
	posFrom = _from;
	posTo = _to;
}

lineEffectLine::lineEffectLine( basicShape* targetShape ){
	rememberShape = targetShape;
	posFrom = rememberShape->getPositionPtr();
	posTo = rememberShape->getPositionPtr();
}

lineEffectLine::lineEffectLine( vertexShape* _targetShape ){
	rememberShape = _targetShape;
	posFrom = _targetShape->getRandomVertexPtr();
	posTo = _targetShape->getRandomVertexPtr();
}

lineEffectLine::~lineEffectLine() {
	
	// no need to delete posFrom, rememberShape and posTo because they are references
}

void lineEffectLine::render(){
	if(!bAlive) return;
	
	float aliveSince = ofGetElapsedTimef()-startTime;
	
	if(aliveSince>LEL_LIFE_SPAN) bAlive = false;
	
	// tmp
	else render( aliveSince/LEL_LIFE_SPAN );
}

void lineEffectLine::render(float state) {
	if(!bAlive) return;
	
	//float opacity = abs( (state*2)-1 );
	float opacity = 1-abs( (state-0.5f)*2 );
		
	ofPushStyle();
	
	ofSetColor(color, opacity*255 );
	ofNoFill();
	//ofSetLineWidth(5);
	ofDrawLine(posFrom->x, posFrom->y, posTo->x, posTo->y);
	
	//ofFill();
	//ofCircle(posFrom->x, posFrom->y, 5);
	//ofCircle(posTo->x, posTo->y, 5);
	
	ofPopStyle();
}

bool lineEffectLine::isAlive() const{
	return bAlive;
}