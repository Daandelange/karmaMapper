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
	
	lock();
	player.stop();
	player.closeMovie();
	unlock();
	
	//stopThread();
	waitForThread(true);
	
	
	
	
	
//	ofRemoveListener(dir.events.serverAnnounced, this, &videoShader::syphonServerAnnounced);
//	// not yet implemented
//	//ofRemoveListener(dir.events.serverUpdated, this, &ofApp::serverUpdated);
//	ofRemoveListener(dir.events.serverRetired, this, &videoShader::syphonServerRetired);
	
	
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

bool videoShader::render(karmaFboLayer& renderLayer, const animationParams &params){
	if( !shaderEffect::render(renderLayer, params) ) return false;
	
//	if(textures.size()>0 ){
//		textures[0].draw(0,0);
//	}
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
void videoShader::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	// do basic Effect function
	shaderEffect::update( renderLayer, params );
	
	if( textures.size()>0 ){
		if(videoMode==VIDEO_MODE_FILE ){
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
					player.update();
					if( player.isLoaded() && (player.isFrameNew() || player.getPosition()<0) ){
						player.setUseTexture(true);
						textures[0] = player.getTexture();
					}
				}
			}
		}
		
		else if(videoMode==VIDEO_MODE_UVC_WEBCAM ){
			//shaderToyArgs.iChannelTime[0]=player.getPosition();
			
			if( UVCWebcam.isReady() ){
				
				UVCWebcam.update();
				
				if(UVCWebcam.isFrameNew()){
					webcamFPSCounter.begin();
					if(ofGetFrameNum()%4==0) webcamFPSCounter.tick("newframe");
					webcamFPSCounter.end();
					
					//tex.loadData(vidGrabber.getPixels());
					//UVCWebcam.getPixels();
					textures[0].loadData( UVCWebcam.getPixels());
				}
				for(int i=0; i<WEBCAM_FPS_HISTORY_SIZE-1; ++i){
					webcamFPSHistory[i]=webcamFPSHistory[i+1];
				}
				webcamFPSHistory[WEBCAM_FPS_HISTORY_SIZE-1] = webcamFPSCounter.getFps();
				
			}
		}
		
#ifdef KM_ENABLE_SYPHON
		else if(videoMode==VIDEO_MODE_SYPHON){
			
			// todo: set REAL channel time
			shaderToyArgs.iChannelTime[0]=ofGetElapsedTimef();
			
			// todo (should not be done here)
			shaderToyArgs.iChannelResolution[0*3+0] = syphonSource.getWidth();
			shaderToyArgs.iChannelResolution[0*3+1] = syphonSource.getHeight();
			shaderToyArgs.iChannelResolution[0*3+2] = syphonSource.getWidth() / syphonSource.getHeight();
			
			if (syphonSource.isSetup()) {
				// need to bind it so it updates....
				syphonSource.bind();// .draw(0,0);
				syphonSource.unbind();
				textures[0] = syphonSource.getTexture();
			}
		}
#endif
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
	setUsePingPong(false);
	
	playBackSpeed = 1.f;
	videoMode = VIDEO_MODE_FILE;
	if(lock()){
		bUseThreadedFileDecoding = true;
		player.closeMovie();
		unlock();
	}
	loadShader( effectFolder("videoShader.vert"), effectFolder("videoShader.frag") );
	
	activeCamera = "";
	UVCWebcam.close();
	UVCWebcam.setUseAudio(false);
	webcamSettings.targetFPS = 30;
	webcamSettings.width = 1280;
	webcamSettings.height = 720;

#ifdef KARMAMAPPER_DEBUG
	UVCWebcam.setVerbose(false);
#endif
	
	{ // add presets for some cameras
		// most presets come from ofxUVC: https://github.com/atduskgreg/ofxUVC/blob/master/example-ofxUVC/bin/data/camera_settings.yml
		selectedUVCControlCamera = nullptr;
		UVCUsbPresets.clear();
		
		ofxUVCCameraSetting preset;
		preset.name = "HD Pro Webcam C920"; // has to match the USB device name
		preset.vendorId = 0x046d;
		preset.productId = 0x82d;
		preset.interfaceNum = 0x00;
		//preset.interfaceNum = 0x03;
		//# 1280x720 (this is 1/2 res)
		// width: 1280 - height: 720
		UVCUsbPresets.push_back(preset);
		
		preset.name = "Microsoft LifeCam HD-3000";
		preset.vendorId = 0x45e;
		preset.productId = 0x779;
		preset.interfaceNum = 0x00;
		// # 1280x720 (this is 1/2 res)
		// width: 640 - height: 360
		UVCUsbPresets.push_back(preset);
		
		preset.name = "Encore Electronics ENUCM-013";
		preset.vendorId = 0x1e4e;
		preset.productId = 0x103;
		preset.interfaceNum = 0x00;
		// width: 640 - height: 480
		UVCUsbPresets.push_back(preset);
		
		preset.name = "Rosewill";
		preset.vendorId = 0x603;
		preset.productId = 0x8b08;
		preset.interfaceNum = 0x00;
		// width: 640 - height: 480
		UVCUsbPresets.push_back(preset);
		
		preset.name = "Built-in iSight";
		preset.vendorId = 0x5ac;
		preset.productId = 0x8507;
		preset.interfaceNum = 0x00;
		// width: 640 - height: 480
		UVCUsbPresets.push_back(preset);
		
		preset.name = "Logitech c910";
		preset.vendorId = 0x046d;
		preset.productId = 0x821;
		preset.interfaceNum = 0x02;
		//# 1280x720 (this is 1/2 res)
		// width: 640 - height: 360
		UVCUsbPresets.push_back(preset);
		
		preset.name = "Logitech c6260";
		preset.vendorId = 0x046d;
		preset.productId = 0x81a;
		preset.interfaceNum = 0x00;
		// width: 640 - height: 480
		UVCUsbPresets.push_back(preset);
	}
	
#ifdef KM_ENABLE_SYPHON
	syphonAddr.appName = "Simple Server";
	syphonAddr.serverName = "";
#endif
	
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
	
	if( ImGui::CollapsingHeader( GUIvideoShaderPanel, "GUIvideoShaderPanel", true, true ) ){
		
		ImGui::TextWrapped("Draws video streams on shapes.");
		
		ImGui::Separator();
		ImGui::SliderFloat("Opacity", &mainColor[3], 0, 1);
		
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
			if ( ImGui::Selectable("VIDEO_MODE_UVC_WEBCAM", videoMode==VIDEO_MODE_UVC_WEBCAM)) {
				setVideoMode(VIDEO_MODE_UVC_WEBCAM);
			}
#ifdef KM_ENABLE_SYPHON
			if ( ImGui::Selectable("VIDEO_MODE_SYPHON", videoMode==VIDEO_MODE_SYPHON)) {
				setVideoMode(VIDEO_MODE_SYPHON);
			}
#endif
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
			static bool tmpThreading = bUseThreadedFileDecoding;
			if(ImGui::Checkbox("Use threaded video decoding", &tmpThreading)) {
				setUseThread(tmpThreading);
				tmpThreading = bUseThreadedFileDecoding;
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
		
		else if(videoMode==VIDEO_MODE_UVC_WEBCAM){
			ImGui::TextWrapped("This mode reads a video stream from a UVC webcam.");
			ImGui::Text("Webcam source : %s", activeCamera.c_str());
			
			if(ImGui::TreeNode("Webcam Selection")){
				vector<string> availableCams = UVCWebcam.listVideoDevices();
				if(availableCams.size()==0){
					ImGui::TextWrapped("[None available...]");
				}
				else {
					for(auto it=availableCams.begin(); it!=availableCams.end(); it++){
						if( ImGui::Selectable( it->c_str(), (*it).compare(activeCamera)==0 ) ){
							selectUVCWebcam(*it);
						}
					}
				}
				ImGui::TreePop();
			}
			
			if(ImGui::TreeNode("Webcam Setup")){
				//UVCWebcam.setDesiredFrameRate();
				ImGui::TextWrapped("Device ID: %d", UVCWebcam.getDeviceID() );
				
				ImGui::Separator();
				ImGui::Text("Resolution:");
				ImGui::SameLine();
				if(ImGui::Button("Choose preset...")){
					ImGui::OpenPopup("camResPresets");
				}
				if(ImGui::BeginPopup("camResPresets")){
					if(ImGui::Selectable("1920 x 1080")){
						webcamSettings.width = 1920;
						webcamSettings.height = 1080;
						selectUVCWebcam();
					}
					if(ImGui::Selectable("1280 x 720")){
						webcamSettings.width = 1280;
						webcamSettings.height = 720;
						selectUVCWebcam();
					}
					if(ImGui::Selectable("640 x 360")){
						webcamSettings.width = 640;
						webcamSettings.height = 360;
						selectUVCWebcam();
					}
					if(ImGui::Selectable("360 x 240")){
						webcamSettings.width = 320;
						webcamSettings.height = 240;
						selectUVCWebcam();
					}
					
					ImGui::EndPopup();
				}
				ImGui::Indent();
				
				static bool lockRatio = true;
				if(ImGui::InputInt("Width:", &webcamSettings.width, 1, 10, ImGuiInputTextFlags_EnterReturnsTrue)){
					if(lockRatio){
						webcamSettings.height = (webcamSettings.height*1.f) * ((webcamSettings.width*1.f)/UVCWebcam.getWidth());
					}
					// reload camera settings
					selectUVCWebcam();
				}
				if(ImGui::InputInt("Height:", &webcamSettings.height, 1, 10, ImGuiInputTextFlags_EnterReturnsTrue)){
					if(lockRatio){
						webcamSettings.width = (webcamSettings.width*1.f) * ((webcamSettings.height*1.f)/UVCWebcam.getHeight());
					}
					// reload camera settings
					selectUVCWebcam();
				}
				ImGui::Checkbox("Lock Ratio", &lockRatio);
				ImGui::Unindent();
				
				ImGui::Separator();
				if(ImGui::InputInt("Target FPS", &webcamSettings.targetFPS, 1, 1, ImGuiInputTextFlags_EnterReturnsTrue)){
					UVCWebcam.setDesiredFrameRate(webcamSettings.targetFPS);
				}
				ImGui::Text("Real FPS: %f", webcamFPSCounter.getFps() );
				ImGui::PlotHistogram("Histogram", webcamFPSHistory, WEBCAM_FPS_HISTORY_SIZE, 0, NULL, 0.f, 60.f, ImVec2(0,80));
				
				ImGui::TreePop();
			}
			
			if(ImGui::TreeNode("Webcam UVC Setup")){
				
				ImGui::TextWrapped("This widget gives extended control options to some webcams using ofxUVC.");
				
				if( ImGui::Button("Show compatible webcams") ){
					ImGui::OpenPopup("UVC Webcam Presets");
					
				}
				
				ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
				if(ImGui::BeginPopupContextItem("UVC Webcam Presets")){

					//ImGui::SetWindowSize(ImVec2(300,200));
					//ImGui::setwin
					ImGui::Text("You may add your own presets in videoShader.cpp");
					
					ImGui::Separator();
					ImGui::PushItemWidth(200);
					for(auto it=UVCUsbPresets.begin();it!=UVCUsbPresets.end(); ++it){
						ImGui::TextWrapped("%s", it->name.c_str());
					}
					ImGui::EndPopup();
				}
				
				ImGui::Separator();
				if(selectedUVCControlCamera!=nullptr){
					ImGui::TextWrapped("Connected with: %s", selectedUVCControlCamera->name.c_str());
					ImGui::Indent();
					ImGui::TextWrapped("vendorId: %x", selectedUVCControlCamera->vendorId);
					ImGui::TextWrapped("productId: %x", selectedUVCControlCamera->productId);
					ImGui::TextWrapped("interfaceNum: %x", selectedUVCControlCamera->interfaceNum);
					ImGui::Unindent();
				}
				else {
					ImGui::TextWrapped("The selected webcam has no available UVC control presets.");
				}
				
				ImGui::TreePop();
			}
			
			if(ImGui::TreeNode("Webcam UVC Controls")){
				if(selectedUVCControlCamera!=nullptr){
					ImGui::Separator();
					if(UVCController.getCameraControls().size()>0){
						bool autoExposure = UVCController.getAutoExposure();
						if(ImGui::Checkbox("Auto Exposure", &autoExposure)){
							UVCController.setAutoExposure(autoExposure);
						}
						float UVCExposure = UVCController.getExposure();
						if(ImGui::SliderFloat("Exposure", &UVCExposure, 0, 1)){
							UVCController.setExposure(UVCExposure);
						}
						bool autoFocus = UVCController.getAutoFocus();
						if(ImGui::Checkbox("Auto Focus", &autoFocus)){
							UVCController.setAutoFocus(autoFocus);
						}
						float UVCFocus = UVCController.getAbsoluteFocus();
						if(ImGui::SliderFloat("Focus", &UVCFocus, 0, 1)){
							UVCController.setAbsoluteFocus(UVCFocus);
						}
						bool autoWB = UVCController.getAutoWhiteBalance();
						if(ImGui::Checkbox("Auto White Balance", &autoWB)){
							UVCController.setAutoWhiteBalance(autoWB);
						}
						float UVCWhiteBalance = UVCController.getWhiteBalance();
						if(ImGui::SliderFloat("White Balance", &UVCWhiteBalance, 0, 1)){
							UVCController.setWhiteBalance(UVCWhiteBalance);
						}
						
						ImGui::Separator();
						ImGui::Separator();
						
						float UVCContrast = UVCController.getContrast();
						if(ImGui::SliderFloat("Contrast", &UVCContrast, 0, 1)){
							UVCController.setWhiteBalance(UVCContrast);
						}
						float UVCSaturation = UVCController.getSaturation();
						if(ImGui::SliderFloat("Saturation", &UVCSaturation, 0, 1)){
							UVCController.setSaturation(UVCSaturation);
						}
						float UVCSharpness = UVCController.getSharpness();
						if(ImGui::SliderFloat("Sharpness", &UVCSharpness, 0, 1)){
							UVCController.setSharpness(UVCSharpness);
						}
						float UVCGain = UVCController.getGain();
						if(ImGui::SliderFloat("Gain", &UVCGain, 0, 1)){
							UVCController.setGain(UVCGain);
						}
						float UVCBrightness = UVCController.getBrightness();
						if(ImGui::SliderFloat("Brightness", &UVCBrightness, 0, 1)){
							UVCController.setBrightness(UVCBrightness);
						}
						
						if(ImGui::TreeNode("Detailed UVC information")){
							ImGui::Columns(4);
							ImGui::SetColumnOffset(0, 0);
							ImGui::Text("Param Name");
							ImGui::NextColumn();
							ImGui::SetColumnOffset(1, 180);
							ImGui::Text("Status Code");
							ImGui::NextColumn();
							ImGui::SetColumnOffset(2, 220);
							ImGui::Text("r/w");
							ImGui::NextColumn();
							ImGui::SetColumnOffset(3, 260);
							ImGui::Text("Auto-controlled");
							ImGui::NextColumn();
							ImGui::Separator();
							auto params = UVCController.getCameraControls();
							for(auto it=params.begin(); it!=params.end(); it++){
								ImGui::Text("%s", it->name.c_str());
								ImGui::NextColumn();
								ImGui::Text("%ld", it->status);
								ImGui::NextColumn();
								ImGui::Text("%d/%d", it->supportsGet(), it->supportsSet());
								ImGui::NextColumn();
								ImGui::Text("%d", it->underAutomaticControl());
								ImGui::NextColumn();
							}
							
							ImGui::Columns(1);
							ImGui::TreePop();
						}
						
					}
				}
				else {
					ImGui::TextWrapped("Unavailable...");
				}
				
				ImGui::TreePop(); // pop UVC controls
			}
		}
		
#ifdef KM_ENABLE_SYPHON
		else if(videoMode==VIDEO_MODE_SYPHON){
			ImGui::TextWrapped("This mode reads a video from a syphon server.");
			ImGui::LabelText("Syphon Running & Connected:", "%i", syphonSource.isSetup());
			ImGui::LabelText("Syphon Source: ", "%s - %s", syphonSource.getServerName().c_str(), syphonSource.getApplicationName().c_str() );
			ImGui::LabelText("Dimensions: ", "%fpx x %fpx", syphonSource.getWidth(), syphonSource.getHeight() );
			
			if( ImGui::Button("Connect to...") ){
				ImGui::OpenPopup("Syphon Server Menu");
			}
			ImGui::SameLine();
			if(ImGui::BeginPopup("Syphon Server Menu")){
				if( dir.size()==0 ){
					ImGui::Text("It seems like you have no syphon server running...");
				}
				else for( ofxSyphonServerDescription& server : dir.getServerList() ){
					if( ImGui::Selectable( (server.serverName + " - " + server.appName ).c_str() ) ){
						connectToSyphonServer(server);
					}
				}
				ImGui::EndPopup();
			}
			
		}
#endif
		
		ImGui::Separator();
	}
	
	shaderEffect::printCustomEffectGui();
	
	return true;
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool videoShader::saveToXML(ofxXmlSettings& xml) const{
	bool ret = shaderEffect::saveToXML(xml);
	
	xml.addValue("videoMode", static_cast<int>(videoMode) );
	
	xml.addValue("playBackSpeed", playBackSpeed);
	xml.addValue("videoFile", videoFile );
	xml.addValue("videoIsPlaying", player.isPlaying());
	xml.addValue("videoPosition", player.getPosition());
	xml.addValue("videoIsPaused", player.isPaused());
	//if( lock() ){
	xml.addValue("bUseThreadedFileDecoding", bUseThreadedFileDecoding);
	//unlock();
	//}

#ifdef KM_ENABLE_SYPHON
	xml.addValue("syphonServer", syphonAddr.serverName);
	xml.addValue("syphonApp", syphonAddr.appName);
#endif

	xml.addValue("activeCamera", activeCamera);
	xml.addValue("activeCameraTargetFPS", webcamSettings.targetFPS);
	xml.addValue("activeCameraWidth", webcamSettings.width);
	xml.addValue("activeCameraHeight", webcamSettings.height);
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool videoShader::loadFromXML(ofxXmlSettings& xml){
	bool ret = shaderEffect::loadFromXML(xml);
	
	ret *= loadShader( effectFolder("videoShader.vert"), effectFolder("videoShader.frag") );
	
	bUseShadertoyVariables = true;
	bUseTextures = true;
	playBackSpeed = xml.getValue("playBackSpeed", 1);
	setVideoMode( static_cast<enum videoMode>(xml.getValue("videoMode", VIDEO_MODE_FILE )) );
	loadVideoFile( xml.getValue("videoFile", "") );
	setUseThread( xml.getValue("bUseThreadedFileDecoding", true) );
	
	if( player.isLoaded() && xml.getValue("videoIsPlaying", false) ){
		
		player.play();
		
		// todo: seems not to work properly...
		player.setPosition( xml.getValue("videoPosition", 0.f) );
		
		if(xml.getValue("videoIsPaused", false)){
			player.setPaused(true);
		}
	}
	else {
		player.stop();
	}
	
	webcamSettings.targetFPS = xml.getValue("activeCameraTargetFPS", webcamSettings.targetFPS);
	webcamSettings.width = xml.getValue("activeCameraWidth", webcamSettings.width);
	webcamSettings.height = xml.getValue("activeCameraHeight", webcamSettings.height);
	selectUVCWebcam(xml.getValue("activeCamera", "none"));
	
#ifdef KM_ENABLE_SYPHON
	connectToSyphonServer( ofxSyphonServerDescription(
		xml.getValue("syphonServer", syphonAddr.serverName),
		xml.getValue("syphonApp", syphonAddr.appName)
	));
#endif
	
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
	
	if (videoMode == VIDEO_MODE_FILE) {
		videoMode = mode;
		
		if( !player.isLoaded() ) setError(true, "Please select a source video.");
	}
	
	else if (videoMode == VIDEO_MODE_UVC_WEBCAM ) {
		videoMode = mode;
		
		if( !selectUVCWebcam() ) setError(true, "Please select a source video.");
	}
	
#ifdef KM_ENABLE_SYPHON
	else if (videoMode == VIDEO_MODE_SYPHON) {
		videoMode = mode;
		
		// start Syphon server tracker
		dir.setup();

		connectToSyphonServer( syphonAddr );
	}
#endif
	else {
		setError( true, "Unsupported video mode in videoShader.");
		ofLogWarning("videoShader::setVideoMode") << "UnSupported video mode: " << mode;
	}
}

bool videoShader::loadVideoFile(const string &_path) {
	ofFile file( _path );
	if( file.exists() && file ){
		videoFile = file.getAbsolutePath();
		
		if (videoMode==VIDEO_MODE_FILE) {
			
			// start playback
			if( lock() ){
				player.setUseTexture( !bUseThreadedFileDecoding );
				player.load(videoFile);
				player.setVolume(0);
				player.setSpeed(playBackSpeed);
				player.setLoopState(OF_LOOP_NORMAL);
				//player.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
				//player.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
				shaderToyArgs.iChannelResolution[0*3+0] = player.getWidth();
				shaderToyArgs.iChannelResolution[0*3+1] = player.getHeight();
				shaderToyArgs.iChannelResolution[0*3+2] = player.getWidth() / player.getHeight();
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
			
			// tmp, this could possibly reset another error then the video file error... // todo
			if(bHasError){
				setError(false);
			}
			
			ofLogNotice("videoShader::loadVideoFile") << "Loaded "<< videoFile << ".";
		}
		else {
			// not in video mode, simply remember file
			
		}
		return true;
	}
	else {
		ofLogNotice("videoShader::loadVideoFile") << "Invalid movie file. Not loading...";
	}
	return false;
}

bool videoShader::selectUVCWebcam(string _cam){
	int camNum = -1;
	vector<string> cams = UVCWebcam.listVideoDevices();
	
	selectedUVCControlCamera = nullptr;
	
	if(cams.size()<1){
		
		if(videoMode==VIDEO_MODE_UVC_WEBCAM){
			setError(true, "No webcams available!");
		}
		
		return false;
	}
	
	// no cam?
	if( _cam.compare("")==0 ){
		// use prev selected one ?
		if(activeCamera.compare("")!=0){
			_cam = activeCamera;
		}
		
		// select first one ?
		camNum = 0;
		_cam = cams[camNum];
	}
	
	// check if camera exists...
	for(int i=0;i!=cams.size();++i){
		if( cams[i].compare(_cam) == 0 ){
			camNum = i;
		}
	}
	
	if(camNum == -1 && videoMode==VIDEO_MODE_UVC_WEBCAM) {
		setError(true, "Please select a webcam!");
		activeCamera = -1;
		return false;
	}
	else {
		// not in UVC mode, simply remember the camera
		activeCamera = _cam;
		
		// setup cam ?
		if (videoMode==VIDEO_MODE_UVC_WEBCAM) {
			//UVCWebcam.setVideoDeviceID(camNum);
			UVCWebcam.setDeviceID(camNum);
			//UVCWebcam.setup(-1,-1); // native dimensions
			UVCWebcam.setup(webcamSettings.width,webcamSettings.height);
			UVCWebcam.setDesiredFrameRate(webcamSettings.targetFPS);
			//UVCWebcam.initRecording();
			
			if(UVCWebcam.isInitialized()){
				shaderToyArgs.iChannelResolution[0*3+0] = UVCWebcam.getWidth();
				shaderToyArgs.iChannelResolution[0*3+1] = UVCWebcam.getHeight();
				shaderToyArgs.iChannelResolution[0*3+2] = UVCWebcam.getWidth() / UVCWebcam.getHeight();
				shaderToyArgs.iChannelTime[0]=0.f;
				
				textures.clear();
				textures.push_back( ofTexture() );
				textures.back().allocate(player.getWidth(), player.getHeight(), GL_RGB);
				
				// check if UVC controls are available ?
				for(auto it=UVCUsbPresets.begin(); it!=UVCUsbPresets.end(); ++it){
					// on mac devices sometimes get renamed to `device name #2`
					string cleanName = activeCamera;
					if(activeCamera.find(" #")!=activeCamera.npos){
						cleanName = cleanName.replace(activeCamera.find(" #"), activeCamera.npos, "");
					}
					cout << cleanName << endl;
					if(it->name.compare( cleanName ) == 0){
						UVCController.useCamera(it->vendorId, it->productId, it->interfaceNum);
						selectedUVCControlCamera = &(*it);
						#ifdef KARMAMAPPER_DEBUG
						ofLogVerbose("videoShader::selectUVCWebcam") << "Found matching UVC control presets for camera " << selectedUVCControlCamera->name;
						#endif
					}
				}
				
				setError(false);
			}
			else{
				setError(true, "UVC Camera Error...");
			}
		}
	}
	
	return true;
}

#ifdef KM_ENABLE_SYPHON
// returns true if server is set (doesn't ensure it's connected)
bool videoShader::connectToSyphonServer( const ofxSyphonServerDescription& _addr ){
	
	if( videoMode == VIDEO_MODE_SYPHON ){
	
		if (!syphonSource.isSetup()) syphonSource.setup();
		syphonSource.set(_addr.serverName,_addr.appName);
		
		bHasError = !syphonSource.isSetup();
		if (bHasError){
			shortStatus = "Please select a Syphon source.";
			return false;
		}
		else {
			if(textures.size()==0) textures.push_back( syphonSource.getTexture() );
			
			shaderToyArgs.iChannelResolution[0*3+0] = syphonSource.getWidth();
			shaderToyArgs.iChannelResolution[0*3+1] = syphonSource.getHeight();
			shaderToyArgs.iChannelResolution[0*3+2] = syphonSource.getWidth() / syphonSource.getHeight();
			return true;
		}
	}
	else {
		// not in syphon mode, simply remember the address for later
		syphonAddr = _addr;
		return true;
	}
	return false;
}
#endif

void videoShader::setUseThread(const bool& _useThread){
	if (videoMode == VIDEO_MODE_FILE) {
		if (lock()) {
			bUseThreadedFileDecoding = _useThread;
			unlock();
			if (!_useThread) {
				if(isThreadRunning()) stopThread();
			}
			else if (!isThreadRunning()) {
				startThread();
			}
			
		}
	}
	else {
		stopThread();
		bUseThreadedFileDecoding = _useThread;
	}
}

void videoShader::threadedFunction(){

#ifdef KARMAMAPPER_DEBUG
	//ofThread::setThreadName("videoShader::threadedFunction() " + ofToString(ofThread::getThreadId()) );
#endif
	
	while (isThreadRunning()) {
		if(lock()){
			if(!player.isLoaded()){
				unlock();
				stopThread();
				return;
			}
			
			if (!bUseThreadedFileDecoding){
				unlock();
				stopThread();
			}
			
			if ( images_to_update.empty()  && videoMode==VIDEO_MODE_FILE) {
				
				if (player.isLoaded() ){
					player.setUseTexture(false);
					//try {
					player.update();
		
					if( player.isFrameNew() || player.getPosition()<0 || images_to_update.empty() ){
						//images_to_update.empty()
						//ofPixels pix = player.getPixels();
						images_to_update.send (std::move (player.getPixels()));
					}
				}
			}
			unlock();
			sleep(14); // limits CPU usage, todo: could be revised #dirty
		}
	}
	stopThread();
}

// register effect type
EFFECT_REGISTER( videoShader , "videoShader" );
