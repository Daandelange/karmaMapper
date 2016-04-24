//
//  karmaFboLayer.h
//  karmaMapper
//
//  Created by Daan de Lange on 25/02/2016.
//
//	Render layer ready for ping-ponging & more. :)
//
//	Freely inspired from code from
//	https://github.com/openframeworks/openFrameworks/blob/master/apps/devApps/fboTester/src/demo4.h

#pragma once

#include "ofMain.h"
#include "basicEffect.h"
#include "animationControllerEvents.h"

class karmaFboLayer;
struct karmaFboLayerDrawEventArgs {
	
	karmaFboLayerDrawEventArgs(karmaFboLayer& _fbo) : fbo(_fbo){
		
	}
	
	karmaControllerDrawEventStage stage;
	karmaFboLayer& fbo;
};

class karmaFboLayer {
public:
	
//	karmaFboLayer(){
//		layerName = "Untitled Layer";
//		layerIndex = -1;
//		allocate( ofGetWidth(), ofGetHeight(), GL_RGBA );
//	}
	
	karmaFboLayer(int _w, int _h){
		layerName = "Untitled Layer";
		layerIndex = -1;
		allocate( _w, _h, GL_RGBA );
#ifdef KM_LOG_INSTANCIATIONS
		cout << "karmaFboLayer() " << ofToString(&*this) << endl;
#endif
	}
	
	~karmaFboLayer(){
	// todo: prevent having duplicates of this...
#ifdef KM_LOG_INSTANCIATIONS
		cout << "~karmaFboLayer() " << ofToString(&*this) << endl;
#endif
	}
	
	void allocate( int _width, int _height, int _internalformat = GL_RGBA){
		
		ofFbo::Settings s;
		s.width				= _width;
		s.height			= _height;
		s.numColorbuffers	= 2;// gets us 2 textures for ping-pong
		s.numSamples		= 0;// ? ofFbo::maxSamples() : 0;
		s.internalformat	= _internalformat;
		
		fbo.allocate(s);
		
		maskFbo.allocate(_width, _height, GL_LUMINANCE); //or GL_RED if you are using the programmable renderer
		
//		for(int i = 0; i < 2; i++){
//			frameBuffers[i].allocate(s);
//			//frameBuffers[i].allocate(_width,_height, _internalformat );
//		}
		
		width = _width;
		height = _height;
		
		clear();
		
		// Set everything to 0
		switched = false;
		//dst = &frameBuffers[1];
		//src = &frameBuffers[0];
		
		MSAA = s.numSamples;
	}
	
	//_bUseBufferFbo lets you draw to the pre-allocated other texture.
	void begin(bool _bUseBufferFbo=false) {
		
		fbo.begin();
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + ((switched?(0+_bUseBufferFbo):(1-_bUseBufferFbo))));	// write to this texture
        
        // alternatve method, but doesnt work on all GPUs
		//fbo.setActiveDrawBuffer(switched?0:1);
		
		// tmp disabled
		if(false && getMSAA()>0){
			fbo.end();
			fbo.updateTexture(switched?(0+_bUseBufferFbo):(1-_bUseBufferFbo));
			fbo.begin();
		}
		//cout << "drawing to fbo.texture: "<<(switched?0:1)<<" // " << fbo.getIdDrawBuffer()<<" // " << fbo.getId()<<endl;
	}
	
	void end(const bool& displayOutput=true){
		fbo.end();
		
		if(displayOutput){
			draw();
		}
	}
	
	void draw(){
		glColor3f(1, 1, 1);
		fbo.draw(0,0);
	}
	
	void swap(){
		//src = &frameBuffers[(switched?0:1)];
		//dst = &frameBuffers[(switched?1:0)];
		switched = !switched;
		//cout << "Switched: "<< switched << endl;
		
		// clear new dest buffer
		fbo.begin();
		fbo.setActiveDrawBuffer(switched?0:1);
		//glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + (switched?0:1));	// write to this texture
		ofClear(0,0);
		fbo.end();
	}
	
	void resetSwap(){
		switched = false;
		//cout << "reset" << endl;
	}
	
	void set(const string& _name, int _layerIndex){
		layerName = _name;
		layerIndex = _layerIndex;
	}
	
	// tmp for debugging
	ofFbo& getFBO(){
		return fbo;
	}
	
	const int& getMSAA(){
		return MSAA;
	}
	
	const string& getName() const {
		return layerName;
	}
	
	const int& getIndex() const {
		return layerIndex;
	}
	
	void setIndex(const int& _index) {
		layerIndex = _index;
	}
	
	bool isAllocated() const {
		return fbo.isAllocated();
	}
	
	ofTexture& getSrcTexture() {
		return (fbo.getTexture(switched?0:1));
	}
	
	ofTexture& getDstTexture() {
		//cout << "Dst = " << (switched?1:0) << endl;
		return (fbo.getTexture(switched?1:0));
	}
	
	ofTexture& getSrcTextureIndex(int i) {
		return (fbo.getTexture(i));
	}
	
	int getHeight() const {
		return height;
	}
	
	int getWidth() const {
		return width;
	}
	
	void clear(int _alpha=255){
//		for(int i = 0; i < 2; i++){
//			frameBuffers[i].begin();
//			ofClear(0,_alpha);
//			frameBuffers[i].end();
//		}
		fbo.begin();
		
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + 1);	// write to this texture
		ofClear(0,_alpha);
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + 0);	// write to this texture
		ofClear(0,_alpha);
		
		fbo.end();
		
		maskFbo.begin();
		ofClear(0,_alpha);
		maskFbo.end();
	}
	
	static ofEvent<karmaFboLayerDrawEventArgs> karmaFboLayerBeforeDraw;
	static ofEvent<karmaFboLayerDrawEventArgs> karmaFboLayerAfterDraw;
	
//	ofFbo& operator[]( int n ){
//		return frameBuffers[n];
//	}
	
	// Ping-pong Fbos access
	//ofFbo *src;
	//ofFbo *dst;
	
	// provide sorting functions
	typedef std::pair<karmaFboLayer, list<basicEffect*> > fboWithEffects;
	
	// used with std::sort()
	struct orderByIndexFunctor {
		bool operator()(const karmaFboLayer* a, const karmaFboLayer* b) const {
			return a->getIndex() < b->getIndex();
		};
		
		//karmaFboLayer::
		bool operator()(const karmaFboLayer::fboWithEffects& a, const karmaFboLayer::fboWithEffects& b) const{
			return a.first.getIndex() < b.first.getIndex();
		};
	};
	static struct orderByIndexFunctor orderByIndex;
	
private:
	// ensured to be deleted on destruction
	//ofFbo frameBuffers[2];
	ofFbo fbo;
	ofFbo maskFbo;
	bool switched;
	string layerName;
	int layerIndex;
	int height, width;
	int MSAA;
};


//bool operator<(const karmaFboLayer::fboWithEffects& a, const karmaFboLayer::fboWithEffects& b) {
//	return a.first.getIndex() < b.first.getIndex();
//}

//inline bool orderLayersByIndex (const karmaFboLayer::fboWithEffects& a, const karmaFboLayer::fboWithEffects& b) {
//	return a.first.getIndex() < b.first.getIndex();
//}
