//
//  lineDrawEffectLine.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 14/12/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "lineDrawEffectLine.h"

lineDrawEffectLine::lineDrawEffectLine(basicPoint* _from, basicPoint* _to) {
	bAlive = true;
	startTime = ofGetElapsedTimef();
	
	//color.setHsb(ofRandom(255), 200, 200);
	color.set(255);
	
//	posFrom = _from;
//	posTo = _to;
}

lineDrawEffectLine::lineDrawEffectLine( basicShape* targetShape ){
	rememberShape = targetShape;
//	posFrom = rememberShape->getPositionPtr();
//	posTo = rememberShape->getPositionPtr();
}

lineDrawEffectLine::lineDrawEffectLine( vertexShape* _targetShape, float _lifeTime ){
	bAlive = true;
	lifeTime = _lifeTime;
	if( lifeTime < 0.05f ) lifeTime = 1.0f;
	startTime = ofGetElapsedTimef();
	points.shape = _targetShape;
	
	points.points.push_back( _targetShape->getRandomVertexPtr() );
	points.points.push_back( _targetShape->getRandomVertexPtr() );
	points.points.push_back( _targetShape->getRandomVertexPtr() );
	points.points.push_back( _targetShape->getRandomVertexPtr() );
//	posFrom = _targetShape->getRandomVertexPtr();
//	posCenter =
//	posTo = _targetShape->getRandomVertexPtr();
}

lineDrawEffectLine::~lineDrawEffectLine() {
	
	// no need to delete posFrom, rememberShape and posTo because they are references
}

void lineDrawEffectLine::render(){
	if(!bAlive) return;
	
	float aliveSince = ofGetElapsedTimef()-startTime;
	
	if(aliveSince>1.f) bAlive = false;
	
	// tmp
	else render( aliveSince/lifeTime );
}

void lineDrawEffectLine::render(float state) {
	if(!bAlive) return;
	
	if( points.shape == nullptr || !points.shape->isReady() ) return;
	
	//float opacity = abs( (state*2)-1 );
	float opacity = 1-(state);
		
	ofPushStyle();
	ofPushMatrix();
	ofTranslate( points.shape->getPositionPtr()->x, points.shape->getPositionPtr()->y );
	
	ofSetColor(color, opacity*255 );
	ofNoFill();
	//ofSetLineWidth(5);
	
	switch( points.points.size() ){
		case 2:
			ofDrawLine(points.points[0]->x, points.points[0]->y, points.points[1]->x, points.points[1]->y);
			break;
			
		case 3:
			ofDrawLine(points.points[0]->x, points.points[0]->y, ofLerp( points.points[1]->x, points.points[2]->x, state), ofLerp(points.points[1]->y, points.points[2]->y, state) );
			break;
			
		case 4:
			ofDrawLine(ofLerp( points.points[0]->x, points.points[3]->x, state), ofLerp(points.points[0]->y, points.points[3]->y, state), ofLerp( points.points[1]->x, points.points[2]->x, state), ofLerp(points.points[1]->y, points.points[2]->y, state) );
			break;
			
		default:
			break;
	}
	
	//ofFill();
	//ofCircle(posFrom->x, posFrom->y, 5);
	//ofCircle(posTo->x, posTo->y, 5);
	
	ofPopMatrix();
	ofPopStyle();
	
	// dead ?
	if(state > 1) bAlive = false;
}

bool lineDrawEffectLine::isAlive() const{
	return bAlive;
}