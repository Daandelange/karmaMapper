//
//  imageMapper.h
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 12/3/14.
//
//

#ifndef __illuminatiVisualiser__imageMapper__
#define __illuminatiVisualiser__imageMapper__

//#include <iostream>
#include "basicEffect.h"
#include "ofMain.h"

class imageMapper : public basicEffect {
	
public:
	imageMapper();
	~imageMapper();
	
	// inherited by basicEffect
	void spawn();
	void animate();
	void reset();
	void pointsUpdated();
	
	void loadFolder( string _folder );
	
	float animator; // todo --> variable that can be externally animated
	int hue;
	float animSpeed;
	
private:
	vector<ofImage> images;
	vector<ofImage> textures;
	ofMesh faces[3]; // the 4 points make 3 ≠ faces
	
	int numImages;
	string currentFolder;
	bool imagesLoaded;
};

#endif /* defined(__illuminatiVisualiser__imageMapper__) */