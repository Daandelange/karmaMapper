//
//  karmaFboLayer.h
//  karmaMapper
//
//  Created by Daan de Lange on 25/02/2016.
//
//	Render layer ready for ping-ponging & more. :)
//

#pragma once

#include "ofMain.h"
#include "basicEffect.h"

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
		
		for(int i = 0; i < 2; i++){
			frameBuffers[i].allocate(_width,_height, _internalformat );
		}
		
		clear();
		
		// Set everything to 0
		switched = false;
		dst = &frameBuffers[1];
		src = &frameBuffers[0];
	}
	
	void swap(){
		src = &frameBuffers[(switched?0:1)];
		dst = &frameBuffers[(switched?1:0)];
		switched = !switched;
	}
	
	void set(const string& _name, int _layerIndex){
		layerName = _name;
		layerIndex = _layerIndex;
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
		return frameBuffers[0].isAllocated() && frameBuffers[1].isAllocated();
	}
	
	void clear(int _alpha=255){
		for(int i = 0; i < 2; i++){
			frameBuffers[i].begin();
			ofClear(0,_alpha);
			frameBuffers[i].end();
		}
	}
	
	ofFbo& operator[]( int n ){
		return frameBuffers[n];
	}
	
	// Ping-pong Fbos access
	ofFbo *src;
	ofFbo *dst;
	
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
	ofFbo frameBuffers[2];
	bool switched;
	string layerName;
	int layerIndex;
};

//bool operator<(const karmaFboLayer::fboWithEffects& a, const karmaFboLayer::fboWithEffects& b) {
//	return a.first.getIndex() < b.first.getIndex();
//}

//inline bool orderLayersByIndex (const karmaFboLayer::fboWithEffects& a, const karmaFboLayer::fboWithEffects& b) {
//	return a.first.getIndex() < b.first.getIndex();
//}
