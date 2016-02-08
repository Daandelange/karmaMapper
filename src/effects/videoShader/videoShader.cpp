//
//  videoShader.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 10/10/15.
//
//  - - - -
//
//  Displays a texture on shape

#include "videoShader.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

videoShader::videoShader(){
	videoShader::reset();
}

videoShader::~videoShader(){
	stopThread();
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool videoShader::initialise(const animationParams& params){
	// init values
	basicEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	videoShader::reset();
	
	return bInitialised;
}

bool videoShader::render(const animationParams &params){
	if( !shaderEffect::render(params) ) return false;
	
//	if(videoMode==VIDEO_MODE_FILE){
//		// tmp
//		if (lock()) {
//			if ( player.isLoaded() && textures.size()>0 ){
//				textures[0].draw(0,0);
//			}
//			unlock();
//		}
//	}
	
	return true;
}

// updates shape data
void videoShader::update(const animationParams& params){
	
	// do basic Effect function
	shaderEffect::update( params );
	
	if(videoMode==VIDEO_MODE_FILE && textures.size()>0 ){
		if ( lock() ){
			shaderToyArgs.iChannelTime[0]=player.getPosition();
			
			if( bUseThreadedFileDecoding){
				unlock();
				ofPixels pix;
				if( images_to_update.tryReceive(pix)){
					textures[0].loadData(pix.getData(), pix.getWidth(), pix.getHeight(), GL_RGB);
				}
			}
			else {
				unlock(); // no threading = no locking
				if( player.isLoaded() && textures.size()>0){
					player.setUseTexture(true);
					player.update();
					textures[0] = player.getTexture();
				}
			}
		}
	}
}

// resets all values
void videoShader::reset(){
	shaderEffect::reset();
	
	// effect type must match with class
	effectType = "videoShader";
	
	// over-ride shader's reset
	bUseShadertoyVariables = true;
	bUseTextures = true;
	
	playBackSpeed = 1.f;
	videoMode = VIDEO_MODE_FILE;
	if(lock()){
		bUseThreadedFileDecoding = true;
		unlock();
		player.closeMovie();
	}
	loadShader( effectFolder("videoShader.vert"), effectFolder("videoShader.frag") );
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
	
	// set error (no video file)
	bHasError = true;
	
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool videoShader::printCustomEffectGui(){
	
	shaderEffect::printCustomEffectGui();
	
	if( ImGui::CollapsingHeader( GUIvideoShaderPanel, "GUIvideoShaderPanel", true, true ) ){
		
		ImGui::TextWrapped("Draws video streams on shapes.");
		
		ImGui::Separator();
		if( ImGui::ListBoxHeader("Texture Mode...", 2) ){
			if(ImGui::Selectable("Fill Shape", textureMode==0)){
				setTextureMode(0);
			}
			if(ImGui::Selectable("Cover", textureMode==1)){
				setTextureMode(1);
			}
			if(ImGui::Selectable("Fit (clamped)", textureMode==2)){
				setTextureMode(2);
			}
			if(ImGui::Selectable("Fit (repeat)", textureMode==3)){
				setTextureMode(3);
			}
			ImGui::ListBoxFooter();
		}
		
		
		
		if (ImGui::ListBoxHeader("Video Mode", 2)){
			
			// manually add new modes here
			if ( ImGui::Selectable("VIDEO_MODE_FILE", videoMode==VIDEO_MODE_FILE)) {
				setVideoMode(VIDEO_MODE_FILE);
			}
			if ( ImGui::Selectable("VIDEO_MODE_SYPHON", videoMode==VIDEO_MODE_SYPHON)) {
				setVideoMode(VIDEO_MODE_SYPHON);
			}
			ImGui::ListBoxFooter();
		}
		
		ImGui::Separator();
		
		if(videoMode==VIDEO_MODE_FILE){
			ImGui::TextWrapped("This mode reads a video from a given file.");
			ImGui::LabelText("Video File Path", "%s", videoFile.c_str() );
			
			if( ImGui::Button("Choose File...") ){
				ofFileDialogResult d = ofSystemLoadDialog("Choose a video file...");
				if(d.bSuccess){
					loadVideoFile( d.getPath() );
				}
			}
			ImGui::SameLine();
			if( ImGui::Button("Reload") ){
				loadVideoFile( videoFile );
			}
			if (lock()) {
				if(ImGui::Checkbox("Use threaded video decoding", &bUseThreadedFileDecoding)) {
					bool tmp = bUseThreadedFileDecoding;
					unlock();
					setUseThread(tmp);
				}
				else {
					unlock();
				}
			}
			
			if( ImGui::DragFloat("playBackSpeed", &playBackSpeed, 0.05, 0.1) ){
				if (lock()){
					player.setSpeed(playBackSpeed);
					unlock();
				}
			}
			if( ImGui::SliderFloat("seekerPosition", &shaderToyArgs.iChannelTime[0], 0, 1) ){
				if (lock()){
					player.setPosition(shaderToyArgs.iChannelTime[0]);
					unlock();
				}
			}
			if( ImGui::Button("Stop") ){
				if( lock() ){
					player.stop();
					unlock();
				}
			}
			ImGui::SameLine();
			if( ImGui::Button("Play") ){
				if( lock() ){
					player.play();
					unlock();
				}
			}
			ImGui::SameLine();
			if( ImGui::Button("Pause") ){
				if( lock() ){
					player.setPaused( !player.isPaused() );
					unlock();
				}
			}
			
		}
		else if(videoMode==VIDEO_MODE_SYPHON){
			ImGui::TextWrapped("This mode reads a video from a syphon server.");
		}
		
		ImGui::Separator();
	}
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool videoShader::saveToXML(ofxXmlSettings& xml) const{
	bool ret = shaderEffect::saveToXML(xml);
	
//	xml.addTag("linesColor");
//	if(xml.pushTag("linesColor")){
//		xml.addValue("r", linesColor[0] );
//		xml.addValue("g", linesColor[1] );
//		xml.addValue("b", linesColor[2] );
//		xml.addValue("a", linesColor[3] );
//		xml.popTag();
//	}
	xml.addValue("playBackSpeed", playBackSpeed);
	xml.addValue("videoMode", static_cast<int>(videoMode) );
	xml.addValue("videoFile", videoFile );
	
	//if( lock() ){
		xml.addValue("bUseThreadedFileDecoding", bUseThreadedFileDecoding);
		//unlock();
	//}
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool videoShader::loadFromXML(ofxXmlSettings& xml){
	bool ret = shaderEffect::loadFromXML(xml);
	
	loadShader( effectFolder("videoShader.vert"), effectFolder("videoShader.frag") );
	bUseShadertoyVariables = true;
	bUseTextures = true;
	playBackSpeed = xml.getValue("playBackSpeed", 1);
	videoMode = static_cast<enum videoMode>(xml.getValue("videoMode", VIDEO_MODE_FILE ));
	loadVideoFile( xml.getValue("videoFile", "") );
	setUseThread( xml.getValue("bUseThreadedFileDecoding", true) );
	
	
	return ret;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool videoShader::randomizePresets(){
	if(!shaderEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}

// - - - - - - -
// videoEffect FUNCTIONS
// - - - - - - -
void videoShader::setVideoMode(const enum videoMode& mode){
	videoMode = mode;
	
	if (videoMode == VIDEO_MODE_FILE) {
		bHasError = true;
		shortStatus = "Please select a source video.";
	}
	
	else if (videoMode == VIDEO_MODE_SYPHON) {
		bHasError = true;
		shortStatus = "Please select a source video.";
	}
	else {
		bHasError = true;
		shortStatus = "Unsupported video mode in videoShader.";
		ofLogWarning("videoShader::setVideoMode") << "UnSupported video mode: " << mode;
	}
}

bool videoShader::loadVideoFile(const string &_path) {
	ofFile file( _path );
	if( file.exists() && file ){
		videoFile = file.getAbsolutePath();
		
		// start playback
		if( lock() ){
			player.setUseTexture( !bUseThreadedFileDecoding );
			player.load(videoFile);
			player.setVolume(0);
			player.setSpeed(playBackSpeed);
			player.setLoopState(OF_LOOP_NORMAL);
			//player.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
			//player.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
			shaderToyArgs.iChannelResolution[0][0] = player.getWidth();
			shaderToyArgs.iChannelResolution[0][1] = player.getHeight();
			shaderToyArgs.iChannelResolution[0][2] = player.getWidth() / player.getHeight();
			player.play();
			unlock();
		}
		if (bUseThreadedFileDecoding && !isThreadRunning()){
			startThread();
		}
		
		textures.clear();
		textures.push_back( ofTexture() );
		textures.back().allocate(player.getWidth(), player.getHeight(), GL_RGB);
		shaderToyArgs.iChannelTime[0]=0.f;
		//texturesTime.push_back(0.f);
		
		ofLogNotice("videoShader::loadVideoFile") << "Loaded "<< videoFile << ".";
	}
	else {
		ofLogNotice("videoShader::loadVideoFile") << "Invalid movie file. Not loading...";
	}
}

void videoShader::setUseThread(const bool& _useThread){
	if(lock()){
		bUseThreadedFileDecoding = _useThread;
		if (!bUseThreadedFileDecoding) {
			if(isThreadRunning()) stopThread();
		}
		else if (!isThreadRunning() ) {
			startThread();
		}
		unlock();
	}
}

void videoShader::threadedFunction(){

#ifdef KARMAMAPPER_DEBUG
	thread.setName("videoShader::threadedFunction() " + thread.name());
#endif
	
	while (isThreadRunning()) {
		if(lock()){
			if (!bUseThreadedFileDecoding){
				unlock();
				return;
			}
			
			if ( images_to_update.empty()  && videoMode==VIDEO_MODE_FILE) {
				
				if (player.isLoaded() ){
					player.setUseTexture(false);
					player.update();
		
					if( player.isFrameNew() ){
						//ofPixels pix = player.getPixels();
						images_to_update.send (std::move (player.getPixels()));
					}
				}
			}
			unlock();
			sleep(14); // limits CPU usage, todo: could be revised
		}
	}
	stopThread();
}

// register effect type
EFFECT_REGISTER( videoShader , "videoShader" );
