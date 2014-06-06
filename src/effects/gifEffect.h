//
//  gifEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 06/06/2014.
//
//

#pragma once

#include "ofMain.h"
#include "basicEffect.h"
#include "ofxGifDecoder.h"

class gifEffect : public basicEffect {
	
public:
	gifEffect();
	~gifEffect();
	
	virtual void spawn();
	virtual void render();
	virtual void update();
	virtual void reset();
	virtual void destroy();
	
	bool setDirectory(string _dir);
	bool loadRandomGif();
	bool loadGif(int videoID);
	bool loadGif(string videoName);
	
protected:
	vector<string> gifFiles;
	string directory;
	ofShader shader;
	
	unsigned int numLoopsPerGif;
	unsigned int loopCount;
	unsigned int currentGif;
	float loopStartTime;
	float timePerFrame;
	ofxGifDecoder decoder;
	vector<ofxGifFile> cachedFiles;
	int cachedGif; // cachedFiles is for precaching... this tells which one is being used
};
