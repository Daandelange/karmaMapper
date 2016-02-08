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

// tmp disabled
#ifdef KM_PLEASESKIPME

#include "videoEffect.h"

videoEffect::videoEffect(){
	
	// effect type mutch match with class
	effectType = "videoEffect";
}

videoEffect::~videoEffect(){
	
	stream.close();
}

bool videoEffect::initialise(){
	bool success = basicEffect::initialise();
	
	if(success){
		effectMutex.lock();
		isLoading = true;
		
		// load movie
		hasError = stream.load("vendome_daan_v1.0/Montage_Export_v1_SD.mov");
		stream.play();
		//cachedImage.allocate(1,1, GL_RGB);
		
		// setup shader
		if(ofIsGLProgrammableRenderer()) hasError *= imageShader.load("effects/imageEffect/imageShaderGL3");
		else hasError *= imageShader.load("effects/imageEffect/imageShader");
		
		if(hasError) isInitialised=false;
		
		isLoading = false;
		
		effectMutex.unlock();
	}
	
	return isInitialised;
}



bool videoEffect::render(){
	effectMutex.lock();
	if( shapes.size()==0 ){
		effectMutex.unlock();
		return true;
	}
	
	/*/ image to display ?
	 if( currentImage >=0 && currentImage < cachedImages.size() && cachedImage.isUsingTexture() ){
		cachedImage.draw( shapes[0]->getBoundingBox() );
	 }// */
	
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
			
			hasError?ofSetColor(255):ofSetColor(255,0,0);
			ofFill();
			
			//tex0.setTextureWrap(GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
			
			//imageShader.setUniform2f("tex0Resolution", tex0.getWidth(), tex0.getHeight());
			
			//cachedImage.getTexture().disableTextureMatrix();
			
			imageShader.setUniform1i("fitMode", 1);
			imageShader.setUniform2f("tex0Resolution", stream.getTexture().getWidth(), stream.getTexture().getHeight());
			// todo: use position pointer unstead of boundingBox ?
			ofRectangle bbox=(*it)->getBoundingBox();
			imageShader.setUniform4f("shapeBoundingBox", bbox.x, bbox.y, bbox.getWidth(), bbox.getHeight());
			//bbox=
			//imageShader.setUniform4f("shapesBoundingBox", bbox.x, bbox.y, bbox.width, bbox.height);
			imageShader.setUniform2f("fboCanvas", ofGetWidth(),ofGetHeight());
			
			//imageShader.setUniformTexture("tex0", cachedImage.getTexture(), 1);
			
			//cachedImage.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
			stream.getTexture().setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
			//cachedImage.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
			stream.getTexture().bind();
			(*it)->sendToGPU();
			stream.getTexture().unbind();
			
			imageShader.end();
		}
	}
	effectMutex.unlock();
}//*/

void videoEffect::update(){
	imageEffect::update();
	
	stream.update();
	cachedImage = stream.getTexture();
}

// resets all values
// overrule this function with your own.
void videoEffect::reset(){
	basicEffect::reset();
	
	
	stream.setLoopState(OF_LOOP_NORMAL);

}

/*
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
}*/

bool videoEffect::loadVideo(string _videoName){
	if( _videoName.compare("") == 0 ) return false;
	
	if( stream.load( _videoName ) ){
		stream.play();
		stream.setVolume(0);
		
		effectMutex.lock();
		hasError = false;
		effectMutex.unlock();
		
		ofLogVerbose("videoEffect") << "Successfully loaded movie: " << _videoName << endl;
		return true;
	}
	else{
		
		ofLogNotice("videoEffect") << "Could not load video: "+ _videoName << endl;
		
		effectMutex.lock();
		hasError = true;
		effectMutex.unlock();
		
		return false;
	}
}
// END KM_PLEASESKIPME
#endif