//
//  videoEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

#include "videoEffect.h"

videoEffect::videoEffect(){
	basicEffect::basicEffect();
	
	directory = "";
	videoFiles.resize(0);
	stream.setLoopState(OF_LOOP_NORMAL);
	//shader.load("./videoEffect/videoShader");
	shader.load("./videoEffect/videoShader");
}

videoEffect::~videoEffect(){
	basicEffect::~basicEffect();
}

// spawns the effect @ the scene so it starts rendering (setup)
// overrule this function with your own.
void videoEffect::spawn(){
	basicEffect::spawn();
}

// update --> animation
// overrule this function with your own.
void videoEffect::render(){
	if( pShape==NULL || !stream.isLoaded() ) return;
	
	// load shader
	shader.begin();
	shader.setUniformTexture("tex0", stream.getTextureReference(), 0);//stream.getTextureReference().texData.textureID );
	shader.setUniform2f("resolution", pShape->boundingBox.width, pShape->boundingBox.height);
	shader.setUniform2f("textureResolution", stream.getTextureReference().getWidth() , stream.getTextureReference().getHeight() );
	shader.setUniform2f("shapeCenterOffset", pShape->getCenterOffsetFromBoundingBox().x, pShape->getCenterOffsetFromBoundingBox().y);
	shader.setUniform1f("textureScale", 2);
	
	// draw shape so GPU gets their vertex data
	pShape->render();
	
	
	// flush the pipeline! :D
	shader.end();
	
	//stream.getTextureReference().draw(0,0);
}

void videoEffect::update(){
	basicEffect::update();
	
	stream.update();
	
	// tmp
	if( ofGetKeyPressed('r') ){
		shader.load("./videoEffect/videoShader");
		cout << "Shader reloaded"<<endl;
	}
}

// resets all values
// overrule this function with your own.
void videoEffect::reset(){
	basicEffect::reset();
}

// called just before removal
void videoEffect::destroy(){
	basicEffect::destroy();
}

bool videoEffect::setDirectory(string _dir){
	// already set ?
	if(_dir==directory) return true;
	
	ofDirectory dir(_dir);
	
	
	if( !dir.exists() || !dir.canRead() ){
		ofLogNotice("videoEffect") << "Unable to load " << _dir << endl;
		return false;
	}
	else{
		// default video path
		//if(directory=="") directory="./videoEffect/videos/";
		
		// scan for video files
		//ofDirectory dir(_dir);
		//dir.open(_dir);
		//dir.listDir( _dir );
		dir.allowExt("mp4");
		dir.allowExt("mov");
		dir.sort();
		dir.listDir(_dir);
		
		
		// todo: change to logverbose
		if(dir.size()==0){
			if(!dir.exists()){
				ofLogNotice("videoEffect") << "Folder «" << _dir << "» doesn't exist." << endl;
			}
			else if(!dir.canRead()){
				ofLogNotice("videoEffect") << "Folder «" << _dir << "» is not readable." << endl;
			}
			else if(!dir.isDirectory()){
				ofLogNotice("videoEffect") << "Folder «" << _dir << "» is not a directory." << endl;
			}
			else ofLogNotice("videoEffect") << "Folder «" << _dir << "» contains no movies to load. Make sure that there are .mov or .mp4 files." << endl;
		}
		else ofLogVerbose("videoEffect") << "Scanning «" << _dir << "» ... Found " << ofToString(dir.size()) << " file(s) (.mp4 or .mov) [readable=" << dir.canRead() << "] [isDirectory=" << ofToString(dir.isDirectory()) << "]" << endl;
		
		// get videos
		videoFiles.resize(0);
		for(int i=0; i<dir.size(); i++){
			// test videos before adding
			if(stream.loadMovie( dir.getPath(i) )){
				videoFiles.push_back( dir.getName(i) );
				//stream.closeMovie(); // dont remain cached
				// fucks up app launch
			}
			else {
				ofLogNotice("videoEffect") << "Could not add " << dir.getPath(i);
			}
		}
		
		directory = _dir;
		
		loadRandomVideo();
	}
	
	return true;
}

bool videoEffect::loadRandomVideo(){
	return loadVideo( (int)ofRandom(-.49, videoFiles.size()-.51) );
}

bool videoEffect::loadVideo(int videoID){
	if(videoID < 0 || videoID >= videoFiles.size() ) return false;
	string tmp = ofToString(directory+"/"+videoFiles[videoID]);
	if( stream.loadMovie( tmp ) ){
		stream.play();
		stream.setVolume(0);
		pShape->hasError = false;
		ofLogVerbose("videoEffect") << "Successfully loaded movie: " << videoFiles[videoID] << endl;
		return true;
	}
	else{
		ofLogNotice("videoEffect") << "Could not load "+ directory+"/"+videoFiles[videoID] << endl;;
		pShape->hasError = true;
		return false;
	}
}

// not: string must exist in videoFiles
bool videoEffect::loadVideo(string videoName){
	int found = -1;
	
	for(int i=0; i<videoFiles.size(); i++){
		if(videoFiles[i]==videoName){
			found=i;
			break;
		}
	}
	
	return (found<0)?false:loadVideo(found);
}
