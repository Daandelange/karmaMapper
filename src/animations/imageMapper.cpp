//
//  imageMapper.cpp
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 12/3/14.
//
//

#include "imageMapper.h"


// this animation loads images from a given folder and displays them on the faces using animations.

imageMapper::imageMapper(){
	
	imagesLoaded = false;
	animator=0;
	animSpeed = .1f;
	
	for(int i = 0 ; i < 3 ; i++){
		faces[i].enableTextures();
		faces[i].addVertex(ofVec3f(0,0));
		faces[i].addVertex(ofVec3f(0,0));
		faces[i].addVertex(ofVec3f(0,0));
	}
	
}

imageMapper::~imageMapper(){
	
}

void imageMapper::spawn(){
	// tmp
	//loadFolder("/animations/imageMapper/a1");
}

void imageMapper::animate(){
	// ofEnableAplhaBlending(); ??????
	
	// security
	if(!numImages || !imagesLoaded) return;
	
	// DRAW the effect
	animator+=animSpeed;
	int currentImage = (int) abs(animator) % numImages;
	
	//textures[currentImage].bind();
	//face[i].enableTexture();
	
	int H = images[currentImage].getHeight();
	int W = images[currentImage].getWidth();
	
	// draw faces 1 by 1
	for(int i=0; i<3; i++){
		ofPushStyle();
		ofSetColor(ofColor::fromHsb(hue, 180, 170));
		faces[i].clearTexCoords();
		faces[i].addTexCoord( ofPoint(0, 0) );
		faces[i].addTexCoord( ofPoint(W, 0) );
		faces[i].addTexCoord( ofPoint(W, H) );
		
		ofFloatColor c = ofFloatColor(255,133,230);
		
		//faces[i].setColor(0,c);
		//faces[i].setColor(1,c);
		//faces[i].setColor(2,c);
		//ofFill();
		//faces[i].disableNormals();
		
		
		textures[(currentImage)%numImages].bind();
		
		faces[i].draw();
		textures[(currentImage)%numImages].unbind();
		
		ofPopStyle();
		
		//ofSetColor(100,200,150);
		//ofNoFill();
		//ofLine(points[0], points[1]);
		
	}
	//textures[currentImage].unbind();
	
	
}

void imageMapper::reset(){
	
}

// celled then the triangle points have been updated
void imageMapper::pointsUpdated(){
	
	// update faces
	//for(int i=0; i<3; i++){
		faces[0].setVertex(0, points[0]);
		faces[0].setVertex(1, points[1]);
		faces[0].setVertex(2, points[3]);
		
		faces[1].setVertex(0, points[2]);
		faces[1].setVertex(1, points[1]);
		faces[1].setVertex(2, points[3]);
		
		faces[2].setVertex(0, points[3]);
		faces[2].setVertex(1, points[0]);
		faces[2].setVertex(2, points[2]);
	//}
	
}

void imageMapper::loadFolder( string _folder ){
	// dont reload the folder if already loaded
	if(currentFolder == _folder) return;
	
	// get folder contents
	ofDirectory dir;
	dir.listDir( _folder );
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
	
	//allocate the vector to have as many ofImages as files
	if( dir.size() ) images.assign( dir.size(), ofImage() );
	
	// loop trough files
	numImages = 0;
	for(int i=0; i<(int)dir.size(); i++){
		string path = dir.getPath(i);
		
		// to check: does loadImage() return false on fail ?
		if( images[numImages].loadImage( path ) ){
			numImages++;
	
		}
	}
	
	// check...
	if(numImages < 1){
		cout << "imageMapper::loadFolder() : Found no images in folder «" << _folder << "»" << endl;
		imagesLoaded = false;
	}

	else {
		imagesLoaded=true;
		
		// resize images[] here ? (if some files failed loading)
		
		// load image texture in cache (copy)
		textures.resize(numImages);
		
		for(int i=0; i<numImages; i++){
			textures[i] = ofImage();
			textures[i].clone( images[i] );
		}
	}
	
	// remember folder name
	currentFolder = _folder;
}
