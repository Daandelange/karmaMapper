//
//  imageFolderEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

// tmp disabled
#ifdef KM_PLEASESKIPME

#pragma once

#include "ofMain.h"
#include "basicEffect.h"
#include "ofGLProgrammableRenderer.h"

class imageFolderEffect : public basicEffect {
	
public:
	imageFolderEffect();
	~imageFolderEffect();
	
	// global effect functions
	virtual bool initialise();
	virtual bool render();
	virtual void update();
	virtual void reset();
	
	bool setFolder(string _dir);
	bool loadRandomImage();
	bool loadImage(int imageID);
	bool loadImage(string imageName);
	
	bool grabSomeShapes();
	
	
protected:
	
	// video stuff
	vector<string> imageFiles;
	vector<ofImage> cachedImages;
	ofShader imageShader;
	string folder;
	int currentImage;
	unsigned int timePerSlide;
	
private:
	
};

// END KM_PLEASESKIPME
#endif