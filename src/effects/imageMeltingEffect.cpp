//
//  imageMeltingEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

#include "imageMeltingEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

imageMeltingEffect::imageMeltingEffect(){
	
	reset();
	
	// effect type must match with class
	effectType = "imageMeltingEffect";
}

imageMeltingEffect::~imageMeltingEffect(){
	
}

// - - - - - - -
// GLOBAL EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
// This function is called before it starts rendering.
// Rendering without having called this function should not be possible.
// isReady() should return true after this is done. (can take a long time)
bool imageMeltingEffect::initialise(){
	isLoading = false;
	
	hasError = sourceImg.load("effects/imageGrainEffect/black_mamba.jpg");
	
	// should be handled by 
	
	basicEffect::initialise();
	
	if(hasError) isLoading=false;
}

// returns true if rendering succeeded
bool imageMeltingEffect::render(){
	if( !isReady() || shapeTextures.size()<1 ) return false;
	
	// draw bounding box
	//ofSetColor( 255,0,0 );
	//ofNoFill();
	//if(overallBoundingBox.width > 0) ofDrawRectangle( overallBoundingBox );
	
	/*for(int i=0; i<shapes.size(); i++){
		shapes[i]->render();
	}*/
	
	for (map<int, imageMeltingPoint>::reverse_iterator it = flyingTextures.rbegin(); it != flyingTextures.rend(); it++) {
		
		if( shapeTextures.find( it->first )==shapeTextures.end() ) continue;
		
		it->second.render( shapeTextures[ it->first ] );
	}
	
	return true;
}

// like ofApp::update()
void imageMeltingEffect::update(){
	basicEffect::update();
	
	if(shapes.size()<1) return;
	
	// analyse rms volume
	float volume = karmaSoundAnalyser::getInstance().getVolumeMono(true);
	// add item if volume peak detected
	if( volume > lastVolumePeakAmount*1.6f ){
		lastVolumePeakAmount = volume;
		int randomShape = round(ofRandom(0, shapes.size()-0.51f));
		
		// add a texture for the shape ?
		if( shapeTextures.find(randomShape) == shapeTextures.end() ){
			ofImage mask;
			ofRectangle rect = shapes[randomShape]->getBoundingBox();
			mask.allocate( rect.width, rect.height, OF_IMAGE_COLOR_ALPHA );
			mask.cropFrom(sourceImg, rect.x, rect.y, rect.width, rect.height);
			// todo: apply alpha to texture
			
			shapeTextures[randomShape] = mask.getTexture();
		}
		
		//ofVec2f* _pos = shapes[randomShape]->getPositionPtr();
		ofVec2f _pos = shapes[randomShape]->getBoundingBox().getTopLeft();

		basicPoint* shapePos = shapes[randomShape]->getPositionPtr();
		ofVec2f dir = ofVec2f(shapePos->x, shapePos->y) - ofVec2f(ofGetWidth()/2.0f, ofGetHeight()/10.0f);
		dir.normalize();
		ofVec2f velocity = dir*karmaSoundAnalyser::getInstance().getSpectrumVariation()/200.f;
		for(int i=0; i<karmaSoundAnalyser::getInstance().getSpectrumVariation()/100.f; i++){
			flyingTextures.insert( std::make_pair(randomShape, imageMeltingPoint(_pos, velocity )));
			velocity *= 1.1f;
		}
	}
	// decrease peak
	lastVolumePeakAmount *= 0.97f;//*(ofGetFrameRate()/60.f);
	
	// update elements
	//int i = flyingTextures.size()-1;
	for (map<int, imageMeltingPoint>::iterator it = flyingTextures.begin(); it != flyingTextures.end(); ++it) {
		
		if(shapeTextures.find(it->first)==shapeTextures.end()) continue;
		
		//it->second.render( shapeTextures[it->first] );
		
		if( it->second.isDead ){
			//map<int, imageMeltingPoint>::iterator _tmp = flyingTextures.begin();
			//std::advance(_tmp, i );
			//flyingTextures.erase( _tmp );
			//it++;
			//map<int, imageMeltingPoint>::reverse_iterator tmpit = flyingTextures.erase( it.base() );
			//flyingTextures::reverse_iterator()
			flyingTextures.erase( it );
			//it--;
		}
		else it->second.update();
		
		//i--;
	}
}

// resets all values
void imageMeltingEffect::reset(){
	basicEffect::reset();
	
	lastVolumePeakAmount = 0;
	//flyingTextures.resize(0);
	//shapeTextures.resize(0);
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool imageMeltingEffect::randomizePresets(){
	return true;
}



// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// imageDistrosionItem
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
imageMeltingPoint::imageMeltingPoint(){
	imageMeltingPoint( ofPoint(0,0), ofPoint(0,0) );
}

imageMeltingPoint::imageMeltingPoint(ofVec2f _position, ofVec2f _velocity){
	position = _position;
	velocity = _velocity;
	isDead = false;
}

imageMeltingPoint::~imageMeltingPoint(){
	
}

void imageMeltingPoint::render( const ofTexture& _tex){
	if( !_tex.isAllocated() ) return;// false;
	
	int radius = karmaSoundAnalyser::getInstance().getZeroCrossings()/400;
	
	_tex.draw(position);
	/*for(int x=position.x-radius; x<position.x+radius; x+=round(ofRandom(1,radius/10)) ){
		if( x < 0 || x > _tex.getWidth() ) continue;
		
		for(int y=position.y-radius; y<position.y+radius; y+=round(ofRandom(1,radius/10))){
			if( y < 0 || y > _tex.getHeight() ) continue;
			
			int offset = y*_tex.getWidth() + x;
			offset /= _tex.getNumChannels()*2;
			ofSetColor( (ofColor) _tex[offset] );
			ofDrawCircle(x, y, 1);
		}
	}*/
	
}

void imageMeltingPoint::update(){
	if(isDead) return;
	//position += ofVec2f( karmaSoundAnalyser::getInstance().getBalance(true)*10.f, karmaSoundAnalyser::getInstance().getSpectrumVariation()/1000.f )*karmaSoundAnalyser::getInstance().getVolumeMono()*2;
	position += velocity;
	
	velocity*=0.98f;
	velocity += karmaSoundAnalyser::getInstance().getBalance(false);
	
	if(velocity.length()<0.5f) isDead=true;
	
	else if( position.x < 0 || position.x > ofGetWidth() || position.y < 0 || position.y > ofGetHeight() ) isDead = true;
}
