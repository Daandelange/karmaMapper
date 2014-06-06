//
//  gifEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 06/06/2014.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

#include "gifEffect.h"

gifEffect::gifEffect(){
	basicEffect::basicEffect();
	
	directory = "";
	gifFiles.resize(0);
	shader.load("./gifEffect/videoShader");
	timePerFrame = 0.05;
	loopCount = 0;
	loopStartTime = ofGetElapsedTimef();
	cachedGif = -1;
	cachedFiles.resize(2);
	
	numLoopsPerGif = 4;
	for(int i=0;i<cachedFiles.size();i++) cachedFiles[i].setBackgroundColor( ofColor(0,0) );
}

gifEffect::~gifEffect(){
	basicEffect::~basicEffect();
}

// spawns the effect @ the scene so it starts rendering (setup)
// overrule this function with your own.
void gifEffect::spawn(){
	basicEffect::spawn();
}

// update --> animation
void gifEffect::render(){
	// todo: what if gif contains only 1 frame ?
	if( pShape==NULL || cachedGif<0 || cachedGif >= cachedFiles.size() || cachedFiles[cachedGif].getNumFrames() < 1 ) return;
	
	//int currentFrame;
	
	// looped ?
	if(loopStartTime + timePerFrame*numLoopsPerGif*cachedFiles[cachedGif].getNumFrames() < ofGetElapsedTimef() ){
		//cachedGif++;
		//cachedGif = cachedGif%cachedFiles.size();
		
		if( loadGif( (currentGif+1)%gifFiles.size() ) ){
			loopStartTime = ofGetElapsedTimef();
		}
		// todo: skip gif if stuck/fail
		else {
			
		}
	}
	
	// calc current frame Nb
	//timePerFrame = cachedFiles[cachedGif].getDuration() / cachedFiles[cachedGif].getNumFrames();
	int currentFrame = (int) floor( (ofGetElapsedTimef() - loopStartTime) /(timePerFrame) ) % (int) cachedFiles[cachedGif].getNumFrames();
	
	
	// load shader
	shader.begin();
	shader.setUniformTexture("tex0", *cachedFiles[cachedGif].getFrameAt(currentFrame)->getRawTexture() , 0);
	shader.setUniform2f("resolution", pShape->boundingBox.width, pShape->boundingBox.height);
	shader.setUniform2f("textureResolution", cachedFiles[cachedGif].getWidth(), cachedFiles[cachedGif].getHeight() );
	shader.setUniform2f("shapeCenterOffset", pShape->getCenterOffsetFromBoundingBox().x, pShape->getCenterOffsetFromBoundingBox().y);
	shader.setUniform1f("textureScale", 1);
	
	// draw shape so GPU gets their vertex data
	pShape->render();
	
	// flush the pipeline! :D
	shader.end();
	
}

void gifEffect::update(){
	basicEffect::update();
	
	//stream.update();
	
	// tmp
	if( ofGetKeyPressed('r') ){
		shader.load("./gifEffect/videoShader");
		cout << "Shader reloaded"<<endl;
	}
}

// resets all values
// overrule this function with your own.
void gifEffect::reset(){
	basicEffect::reset();
}

// called just before removal
void gifEffect::destroy(){
	basicEffect::destroy();
}

bool gifEffect::setDirectory(string _dir){
	// already set ?
	if(_dir==directory) return true;
	
	ofDirectory dir(_dir);
	
	
	if( !dir.exists() || !dir.canRead() ){
		ofLogNotice("gifEffect") << "Unable to load " << _dir << endl;
		return false;
	}
	else{
		// default video path
		if(directory=="") directory="./gifEffect/gifs/";
		
		dir.allowExt("gif");
		dir.sort();
		dir.listDir(_dir);
		
		
		if(dir.size()==0){
			if(!dir.exists()){
				ofLogNotice("gifEffect") << "Folder «" << _dir << "» doesn't exist." << endl;
			}
			else if(!dir.canRead()){
				ofLogNotice("gifEffect") << "Folder «" << _dir << "» is not readable." << endl;
			}
			else if(!dir.isDirectory()){
				ofLogNotice("gifEffect") << "Folder «" << _dir << "» is not a directory." << endl;
			}
			else ofLogNotice("gifEffect") << "Folder «" << _dir << "» gif files." << endl;
		}
		else ofLogVerbose("gifEffect") << "Scanning «" << _dir << "» ... Found " << ofToString(dir.size()) << " file(s) (.gif) [readable=" << dir.canRead() << "] [isDirectory=" << ofToString(dir.isDirectory()) << "]" << endl;
		
		// get videos
		gifFiles.resize(0);
		for(int i=0; i<dir.size(); i++){
			// test videos before adding... HEAVY (remove this check for performence?)
			//if(decoder.decode( dir.getPath(i) )){
				gifFiles.push_back( dir.getName(i) );
				//decoder.reset();
			//}
			//else {
			//	ofLogNotice("gifEffect") << "Could not add " << dir.getPath(i);
			//}
		}
		
		directory = _dir;
		
		//loadRandomGif();
		loadGif(0);
	}
	
	return true;
}

bool gifEffect::loadRandomGif(){
	return loadGif( (int)ofRandom(-.49, gifFiles.size()-.51) );
}

bool gifEffect::loadGif(int gifID){
	if(gifID < 0 || gifID >= gifFiles.size() ) return false;
	
	string tmp = ofToString(directory+"/"+gifFiles[gifID]);
	if( decoder.decode( tmp ) ){
		cachedGif = (cachedGif+1)%cachedFiles.size();
		cachedFiles[ cachedGif ] = decoder.getFile();
		pShape->hasError = false;
		ofLogVerbose("gifEffect") << "Successfully loaded movie: " << gifFiles[gifID] << endl;
		currentGif = gifID;
		
		return true;
	}
	else{
		ofLogNotice("gifEffect") << "Could not load "+ directory+"/"+gifFiles[gifID] << endl;;
		pShape->hasError = true;
		cachedGif = -1;
		return false;
	}
}

// not: string must exist in gifFiles
bool gifEffect::loadGif(string videoName){
	int found = -1;
	
	for(int i=0; i<gifFiles.size(); i++){
		if(gifFiles[i]==videoName){
			found=i;
			break;
		}
	}
	
	return (found<0)?false:loadGif(found);
}
