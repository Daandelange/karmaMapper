//
//  imageGrainEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

#include "imageGrainEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

imageGrainEffect::imageGrainEffect(){
	
	reset();
	
	// effect type must match with class
	effectType = "imageGrainEffect";
}

imageGrainEffect::~imageGrainEffect(){
	
}

// - - - - - - -
// GLOBAL EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
// This function is called before it starts rendering.
// Rendering without having called this function should not be possible.
// isReady() should return true after this is done. (can take a long time)
bool imageGrainEffect::initialise(){
	isLoading = false;
	
	hasError = img.load("effects/imageGrainEffect/black_mamba.jpg");
	
	basicEffect::initialise();
}

// todo: update -(handled by)-> animation
// returns true if rendering succeeded
bool imageGrainEffect::render(){
	if( !isReady() ) return false;
	
	// draw bounding box
	//ofSetColor( 255,0,0 );
	//ofNoFill();
	//if(overallBoundingBox.width > 0) ofDrawRectangle( overallBoundingBox );
	
	/*for(int i=0; i<shapes.size(); i++){
		shapes[i]->render();
	}*/
	
	
	
	
	
	for(int i=0; i<items.size(); i++){
		items[i].render( img.getPixels() );
	}
	musicEffect::renderVariables();
	
	return true;
}

// like ofApp::update()
void imageGrainEffect::update(){
	basicEffect::update();
	
	//if( shapes.size()==0 ) return;
	
	if( items.size() < 20 && ofRandom(0,5)>3 ) items.push_back(imageGrainItem(*shapes[round(ofRandom(0, shapes.size()-.51f))]->getPositionPtr()));
	
	for(int i=items.size()-1; i>=0; i--){
		if(items[i].isDead){
			items.erase(items.begin() + 1);
		}
		
		else items[i].update();
	}
}

// resets all values
void imageGrainEffect::reset(){
	basicEffect::reset();
	
	
	items.resize(0);
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool imageGrainEffect::randomizePresets(){
	return true;
}



// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// imageDistrosionItem
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
imageGrainItem::imageGrainItem(){
	imageGrainItem( ofPoint(0,0) );
}

imageGrainItem::imageGrainItem(ofPoint _position){
	position = _position;
	isDead = false;
}

imageGrainItem::~imageGrainItem(){
	
}

void imageGrainItem::render( const ofPixels& _pix){
	int radius = karmaSoundAnalyser::getInstance().getZeroCrossings()/200;
	
	if(_pix.size()<1) return;
	
	for(int x=position.x-radius; x<position.x+radius; x+=round(ofRandom(1,radius/10)) ){
		if( x < 0 || x > _pix.getWidth() ) continue;
		
		for(int y=position.y-radius; y<position.y+radius; y+=round(ofRandom(1,radius/10))){
			if( y < 0 || y > _pix.getHeight() ) continue;
			
			int offset = y*_pix.getWidth() + x;
			offset /= _pix.getNumChannels()*2;
			ofSetColor( (ofColor) _pix[offset] );
			ofDrawCircle(x, y, 1);
		}
	}
	
}

void imageGrainItem::update(){
	position += ofVec2f( karmaSoundAnalyser::getInstance().getBalance(true)*10.f, karmaSoundAnalyser::getInstance().getSpectrumVariation()/1000.f )*karmaSoundAnalyser::getInstance().getVolumeMono()*2;
	
	if( position.x < 0 || position.x > ofGetWidth() || position.y < 0 || position.y > ofGetHeight() ) isDead = true;
}
