//
//  imageEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

// tmp disabled
#ifdef KM_PLEASESKIPME

#include "imageEffect.h"

imageEffect::imageEffect(){
	basicEffect::basicEffect(); // tocheck: can be stripped out?
	
	// effect type mutch match with class
	effectType = "imageEffect";
	
	// init variables
	reset();
}

imageEffect::~imageEffect(){
	//basicEffect::~basicEffect();
	
	ofScopedLock lock(effectMutex);
	
	imageShader.unload();
	
	
	ofRemoveListener(durationReceiver::durationFloatEvent, this, &imageEffect::floatListener);
}

bool imageEffect::initialise(){
	bool success = basicEffect::initialise();
	
	if(success){
		effectMutex.lock();
		isLoading = true;
		
		// set folder and load images
		//hasError = setFolder("ressources/images/collection1");
		//cachedImage.allocate(500,500, GL_RGBA, 0);
		//loadImageFromFile("ressources/images/collection1/Stronium-Tree-highres.jpg");
		//cachedImage.allocate(ofGetCurrentRenderer()->getViewportWidth(), ofGetCurrentRenderer()->getViewportHeight(), GL_RGB);
		hasError = false;
		
		// setup shader
		if(ofIsGLProgrammableRenderer()) hasError *= imageShader.load("effects/imageEffect/imageShaderGL3");
		else hasError *= imageShader.load("effects/imageEffect/imageShader");
		
		if(hasError) isInitialised=false;
		
		isLoading = false;
		effectMutex.unlock();
	}
	
	return isInitialised;
}

bool imageEffect::render(){
	//effectMutex.lock();
	ofScopedLock lock(effectMutex);
	
	if( shapes.size()==0 ){
		//effectMutex.unlock();
		return true;
	}
	
	if(opacity==0){
		return true;
	}
	
	// show image on all available zones
	for (vector<basicShape*>::iterator it=shapes.begin(); it != shapes.end(); it++ ) {
		if( (*it)->isReady() && imageShader.isLoaded() ){
			//return true;
			//cachedImage.draw( it[0]->getBoundingBox() );
			//it[0]->hasError?ofSetColor(255):ofSetColor(255,0,0);
			//ofDisableArbTex();
			//ofEnableTextureEdgeHack();
			
			//ofTexture &tex0 = cachedImage.getTexture();
			
			imageShader.begin();
			
			//hasError?ofSetColor(255):ofSetColor(255,0,0);
			ofSetColor(255, opacity*255);
			ofFill();
			
			//tex0.setTextureWrap(GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
			
			//imageShader.setUniform2f("tex0Resolution", tex0.getWidth(), tex0.getHeight());
			
			//cachedImage.getTexture().disableTextureMatrix();
			
			imageShader.setUniform1i("fitMode", 1);
			imageShader.setUniform2f("tex0Resolution", cachedImage.getWidth(), cachedImage.getHeight());
			// todo: use position pointer unstead of boundingBox ?
			ofRectangle bbox=(*it)->getBoundingBox();
			imageShader.setUniform4f("shapeBoundingBox", bbox.x, bbox.y, bbox.getWidth(), bbox.getHeight());
			//bbox=
			//imageShader.setUniform4f("shapesBoundingBox", bbox.x, bbox.y, bbox.width, bbox.height);
			imageShader.setUniform2f("fboCanvas", ofGetWidth(),ofGetHeight());
			imageShader.setUniform1f("opacity", opacity);
			
			//imageShader.setUniformTexture("tex0", cachedImage.getTexture(), 1);
			
			//cachedImage.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
			cachedImage.setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
			//cachedImage.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
			cachedImage.bind();
			(*it)->sendToGPU();
			cachedImage.unbind();
			
			imageShader.end();
		}
	}
	//effectMutex.unlock();
}

void imageEffect::update(){
	// you can chose to do the default behaviour too or not by commenting this out

	basicEffect::update();
	
	// do your stuff here
	
	//effectMutex.lock();
	//effectMutex.unlock();
	
	
}

// resets all values
// overrule this function with your own.
void imageEffect::reset(){
	basicEffect::reset();
}

bool imageEffect::loadImageFromFile(string _imagePath){
	if(_imagePath.compare("")==0) return false;
	
	// already cached or cache it ?
	//ofPixels img;
	if( ofLoadImage( cachedImage, _imagePath ) ){
		return true;
	}
	// fail ?
	else{
		ofLogNotice("imageEffect") << "Could not load: "+ _imagePath << endl;
		hasError = true;
	}
	
	return false;
}

bool imageEffect::listenToOscIntensity(string _OSCPath){
	if(_OSCPath.compare("")==0) return false;
	
	listenToIntensity = _OSCPath;
	
	ofAddListener(durationReceiver::durationFloatEvent, this, &imageEffect::floatListener);
	
	return true;
}

/*bool imageEffect::grabSomeShapes(){
	// clear
	shapes.clear();
	shapes.resize(0);
	
	// we only want vertex shapes
	//shapes = shapesHandler::getInstance().getShapesByType("vertexShape");
	
	// select the vertex shapes in the biggest group
	map<int, int> groups;
	groups.clear();
	for(int i=0; i<shapes.size(); i++){
		int group = shapes[i]->getGroupID();
		groups[group]++;
	}
	
	// get biggest populated group ID
	int most = -1;
	for( std::map<int,int>::iterator it=groups.begin(); it!= groups.end(); ++it ){
		if( it->second > groups[most] ) most = it->first;
	}
	
	// filter others out
	for (int i=shapes.size()-1; i>=0; i--) {
		if( shapes[i]->getGroupID() != most ) shapes.erase( shapes.begin() + i );
	}
	
	return true;
}*/

// - - - - -
// EVENT LISTENERS
// - - - - - - - - -
void imageEffect::floatListener(durationFloatEventArgs &_args){
	ofScopedLock lock(effectMutex);
	//cout <<_args.track << endl;
	if( listenToIntensity.compare("")==0) return;
	if( _args.track.compare(listenToIntensity)!=0 ) return;
	
	opacity = _args.value;
	//lines.push_back();
}

/*void imageEffect::onSetEventListener(mirOnSetEventArgs &_args){
	ofScopedLock lock(effectMutex);
	
	if(shapes.size()<=0) return;
	
	if(_args.source.compare("aubioOnSet")==0) for(int i=0; i<round(mirReceiver::mirCache.zcr*130); i++){
		//lines.push_back( getRandomLine() );
	}
	
	//else if(_args.source.compare("")==0){}
}*/

// END KM_PLEASESKIPME
#endif