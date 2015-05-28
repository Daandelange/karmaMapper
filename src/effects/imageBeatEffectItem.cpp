//
//  imageBeatEffectItem.cpp
//  workshopMappingCyril
//
//  Created by Daan de Lange on 14/12/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "imageBeatEffectItem.h"

imageBeatEffectItem::imageBeatEffectItem(basicShape* _shape) {
	if(_shape==NULL) return;
	
	vertexShape* shape =((vertexShape*)_shape);
	
	bAlive = true;
	startTime = ofGetElapsedTimef();
	center = shape->getCenterPtr();
	position = *center - ofVec2f( shape->getBoundingBox().x, _shape->getBoundingBox().y );
	
	// get image from shape
	ofRectangle size = _shape->getBoundingBox();
	if( size.width ==0 && size.height ==0 ){
		bAlive=false;
		return;
	}
	image.allocate(size.width, size.height, GL_RGBA, 1);
	image.loadScreenData(size.x, size.y, size.width, size.height);
	/* ofPixels tmp; // tmp
	//image.readToPixels(tmp);
	
	// draw shape in an fbo and draw shape mask
	ofFbo buffer;
	buffer.allocate(size.width, size.height, GL_RGBA, 1);
	buffer.begin();
	ofClear(0,1);
	ofClearAlpha();
	
	// draw virtual mask
	//ofTranslate( *shape->getPositionPtr() * ofVec2f(-1.f,-1.f) +size.width/2); // because shape->render() translates it back
	ofTranslate(-1*ofVec2f( shape->getBoundingBox().x, _shape->getBoundingBox().y ));
	shape->render();
	ofTranslate(ofVec2f( shape->getBoundingBox().x, _shape->getBoundingBox().y ));
	
	buffer.end();
	
	// draw texture with alpha mask
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_SUBTRACT);
	//ofSetColor(255,255);
	image.draw(-position.x,-position.y);
	ofDisableAlphaBlending();
	
	buffer.end();
	
	//buffer.readToPixels(tmp);
	
	//ofSaveImage(tmp, "lol.png");
	
	// get mask pixels
	ofPixels maskPixels;
	buffer.readToPixels( maskPixels );
	image.loadData(maskPixels);
	//image.
	//*/
	//ofPixels tmp;
	//image.readToPixels(tmp);
	
	//ofSaveImage(tmp, "lol.png");
	
	// apply mask opacity to image (3=alpha channel)
	//for( int i=3; i<maskPixels.size(); i += maskPixels.getNumChannels() ) {
		//image.getPixelsRef()[i] = maskPixels[i];
	//}
	
	// tmp to see the image mask for debugging
	//image.setFromPixels(maskPixels);
	
	// finally update our image's pixels
	//image.update();
	
	//image.
	
	
}

imageBeatEffectItem::~imageBeatEffectItem() {
		
}

void imageBeatEffectItem::render(){
	
	//ofScopedLock lock( effectMutex );
	if(!bAlive) return;
	
	float aliveSince = ofGetElapsedTimef()-startTime;
	
	if(aliveSince>LEL_LIFE_SPAN) bAlive=false;
	
	else render( aliveSince/LEL_LIFE_SPAN );
	
	return true;
}

void imageBeatEffectItem::render(float state) {
	if(!bAlive) return;
	
	//float opacity = abs( (state*2)-1 );
	float opacity = 1-abs( (state-0.5f)*2 );
	
	ofPushStyle();
	
	float scale = 1-state;
	ofVec2f xy = *center - position*scale;
	ofVec2f wh = ofVec2f( image.getWidth(), image.getHeight() )*scale;
	image.draw( xy.x, xy.y, wh.x, wh.y );
	
	ofPopStyle();
}

bool imageBeatEffectItem::isAlive() const{
	return bAlive;
}
