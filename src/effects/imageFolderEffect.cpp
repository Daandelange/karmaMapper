//
//  imageFolderEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

#include "imageFolderEffect.h"

imageFolderEffect::imageFolderEffect(){
	basicEffect::basicEffect(); // tocheck: can be stripped out?
	
	// effect type mutch match with class
	effectType = "imageFolderEffect";
	
	// init variables
	reset();
}

imageFolderEffect::~imageFolderEffect(){
	//basicEffect::~basicEffect();
}

bool imageFolderEffect::initialise(){
	bool success = basicEffect::initialise();
	
	if(success){
		isLoading = true;
		
		// set folder and load images
		hasError = setFolder("ressources/images/collection1");
		
		// setup shader
		if(ofIsGLProgrammableRenderer()) hasError *= imageShader.load("effects/imageFolderEffect/imageShaderGL3");
		else hasError *= imageShader.load("effects/imageFolderEffect/imageShader");
		
		if(hasError) isInitialised=false;
		
		isLoading = false;
	}
	
	return isInitialised;
}

// update --> animation
// overrule this function with your own
bool imageFolderEffect::render(){
	if( shapes.size()==0 ) return;
	
	/*/ image to display ?
	if( currentImage >=0 && currentImage < cachedImages.size() && cachedImages[currentImage].isUsingTexture() ){
		cachedImages[currentImage].draw( shapes[0]->getBoundingBox() );
	}// */
	
	// show image on all available zones
	for (vector<basicShape*>::iterator it=shapes.begin(); it != shapes.end(); it++ ) {
		if( (*it)->isReady() && imageShader.isLoaded() ){
			//return true;
			//cachedImages[currentImage].draw( it[0]->getBoundingBox() );
			//it[0]->hasError?ofSetColor(255):ofSetColor(255,0,0);
			//ofDisableArbTex();
			//ofEnableTextureEdgeHack();
			
			//ofTexture &tex0 = cachedImages[currentImage].getTexture();
			
			imageShader.begin();
			
			hasError?ofSetColor(255):ofSetColor(255,0,0);
			ofFill();
			
			//tex0.setTextureWrap(GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
			
			//imageShader.setUniform2f("tex0Resolution", tex0.getWidth(), tex0.getHeight());
			
			//cachedImages[currentImage].getTexture().disableTextureMatrix();
			
			imageShader.setUniform1i("fitMode", 1);
			imageShader.setUniform2f("tex0Resolution", cachedImages[currentImage].getWidth(), cachedImages[currentImage].getHeight());
			// todo: use position pointer unstead of boundingBox ?
			ofRectangle bbox=(*it)->getBoundingBox();
			imageShader.setUniform4f("shapeBoundingBox", bbox.x, bbox.y, bbox.getWidth(), bbox.getHeight());
			//bbox=
			//imageShader.setUniform4f("shapesBoundingBox", bbox.x, bbox.y, bbox.width, bbox.height);
			imageShader.setUniform2f("fboCanvas", ofGetWidth(),ofGetHeight());
			
			//imageShader.setUniformTexture("tex0", cachedImages[currentImage].getTexture(), 1);
			
			//cachedImages[currentImage].getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
			cachedImages[currentImage].getTexture().setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
			//cachedImages[currentImage].getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
			cachedImages[currentImage].getTexture().bind();
			(*it)->sendToGPU();
			cachedImages[currentImage].getTexture().unbind();
			
			imageShader.end();
		}
	}
	
}

void imageFolderEffect::update(){
	// you can chose to do the default behaviour too or not by commenting this out
	basicEffect::update();
	
	// do your stuff here
	
	// advance slide ?
	if(cachedImages.size()>0 ){
		
		currentImage = floor( (aliveSince%( timePerSlide *cachedImages.size()))/ timePerSlide );
		
		// increment
		//currentImage++;
		
		// rewind ?
		if( currentImage >= cachedImages.size() ) currentImage=0;
	}
}

// resets all values
// overrule this function with your own.
void imageFolderEffect::reset(){
	basicEffect::reset();
	
	folder = "";
	currentImage = -1;
	imageFiles.resize(0);
	imageFiles.clear();
	timePerSlide = 500; // time in ms
}


bool imageFolderEffect::setFolder(string _dir){
	// already set ?
	if(_dir==folder) return true;
	
	ofDirectory dir(_dir);
	
	if( !dir.exists() || !dir.canRead() ){
		ofLogNotice("imageFolderEffect") << "Unable to load " << _dir << endl;
		return false;
	}
	else{
		// default image path
		//if(folder=="") folder="./imageFolderEffect/images/";
		
		// scan for images
		dir.allowExt("jpg");
		dir.allowExt("png");
		dir.allowExt("jpeg");
		dir.allowExt("gif");
		dir.listDir(_dir);
		dir.sort();
		
		// todo: change to logverbose
		if(dir.size()==0){
			if(!dir.exists()){
				ofLogNotice("imageFolderEffect") << "Folder «" << _dir << "» doesn't exist." << endl;
			}
			else if(!dir.canRead()){
				ofLogNotice("imageFolderEffect") << "Folder «" << _dir << "» is not readable." << endl;
			}
			else if(!dir.isDirectory()){
				ofLogNotice("imageFolderEffect") << "Folder «" << _dir << "» is not a directory." << endl;
			}
			else ofLogNotice("imageFolderEffect") << "Folder «" << _dir << "» contains no images to load. Make sure that there are .mov or .mp4 files." << endl;
		}
		else ofLogVerbose("imageFolderEffect") << "Scanning «" << _dir << "» ... Found " << ofToString(dir.size()) << " file(s) (.gif | .jpg | .jpeg | .png) [readable=" << ofToString(dir.canRead()) << "] [isDirectory=" << ofToString(dir.isDirectory()) << "]" << endl;
		
		// cache images
		imageFiles.resize(0);
		cachedImages.resize(0);
		for(int i=0; i<dir.size(); i++){
			// test images before adding them
			ofImage tmp;
			if(tmp.load( dir.getPath(i) )){
				imageFiles.push_back( dir.getName(i) );
				cachedImages.push_back( tmp );
			}
			else {
				ofLogNotice("imageFolderEffect") << "Could not add " << dir.getPath(i);
			}
		}
		
		// resize cache size
		//cachedImages.clear();
		//cachedImages.resize( imageFiles.size() );
		
		folder = _dir;
		
		loadRandomImage();
	}
	
	return true;
}

bool imageFolderEffect::loadRandomImage(){
	return loadImage( round(-.49+ofRandomf()*(imageFiles.size()-.011f)) );
}

bool imageFolderEffect::loadImage(int imageID){
	if(imageID < 0 || imageID >= imageFiles.size() ) return false;
	
	// already cached or cache it ?
	string tmp = ofToString(folder+"/"+imageFiles[imageID]);
	if( cachedImages[imageID].isUsingTexture() || cachedImages[imageID].load( tmp )){
		currentImage = imageID;
		return true;
	}
	// fail ?
	else{
		ofLogNotice("imageFolderEffect") << "Could not load "+ tmp << endl;;
		hasError = true;
		return false;
	}
}

bool imageFolderEffect::loadImage(string imageName){
	int found = -1;
	
	// find imageID by string
	for(int i=0; i<imageFiles.size(); i++){
		if(imageFiles[i]==imageName){
			found=i;
			break;
		}
	}
	
	return (found<0)?false:loadImage(found);
}

bool imageFolderEffect::grabSomeShapes(){
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
	
}
