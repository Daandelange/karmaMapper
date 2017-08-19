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

videoShader::videoShader() : bUseThreadedFileDecoding(false, "bUseThreadedFileDecoding"), videoMedia(karmaVideoMediaInformationStruct(), "videoShaderMedia") {
	videoShader::reset();
	
	//threadedPlayer.setPlayer(ofPtr<ofQTKitPlayer>(new ofQTKitPlayer()));
}

videoShader::~videoShader(){
	
	player.stop();
	player.closeMovie();
	//player.ofBaseVideo::close();
	
	newImagesFromThread.close();
	
	//stopThread();
	if(isThreadRunning()){
		waitForThread(true);
	}
	
	//threadedPlayer.stop();
	//threadedPlayer.closeMovie();
	
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
	shaderEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	videoShader::reset();
	
	bIsLoading = false;
	bInitialised = true;
	
	return bInitialised;
}

bool videoShader::render(karmaFboLayer& renderLayer, const animationParams &params){
	
	if( !shaderEffect::render(renderLayer, params) ){
		return false;
	}
	
//	renderLayer.begin();
//	if(textures.size()>0 ){
//		textures[0].draw(0,0);
//	}
//	renderLayer.end();
	
	return true;
}

// updates shape data
void videoShader::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	if(!bEnabled) return;
	
	// do basic Effect function
	shaderEffect::update( renderLayer, params );
	
	if( textures.size()>0 ){
		if(videoMode==VIDEO_MODE_FILE ){
			
			// sync threadedVars
			bUseThreadedFileDecoding.syncFromMainThread();
			if( videoMedia.syncFromMainThread() ){
				// received a change from thread
				
			}
			
			static kmNewVideoFrameStruct newFrame;//(pix);
			bool hasNewPix = false;
			// while to get the last sent one (and pop() all others)
			while( newImagesFromThread.tryReceive(newFrame)){
				hasNewPix = true;
			}
			
			if( bUseThreadedFileDecoding ){
				
				if( hasNewPix ){
					if(newFrame.newPixels.isAllocated()){
						
						videoPlayerFPSCounter.begin();
						videoPlayerFPSCounter.tick("newframe");
						videoPlayerFPSCounter.end();
						
						for(int i=0; i<WEBCAM_FPS_HISTORY_SIZE-1; ++i){
							videoPlayerFPSHistory[i]=videoPlayerFPSHistory[i+1];
						}
						videoPlayerFPSHistory[WEBCAM_FPS_HISTORY_SIZE-1] = videoPlayerFPSCounter.getFps();
						
						videoMedia->playBackSettings.seekerPosition = newFrame.position;
						shaderToyArgs.iChannelTime[0]=newFrame.position;
						textures[0].loadData(newFrame.newPixels);
						//textures[0].loadData(newFrame.newPixels.getData(), newFrame.newPixels.getWidth(), newFrame.newPixels.getHeight(), ofGetGLFormatFromPixelFormat(newFrame.newPixels.getPixelFormat()) );
						//ofSaveImage(newFrame.newPixels, "myTmpImage.png");
					}
				}
			}
			// no threaded file decoding
			else {
				player.setUseTexture(true);
				player.update();
				
				shaderToyArgs.iChannelTime[0]=player.getPosition();
				
				if( player.isLoaded() && (player.isFrameNew() || player.getPosition()<0) ){
					videoPlayerFPSCounter.begin();
					videoPlayerFPSCounter.tick("newframe");
					videoPlayerFPSCounter.end();
					
					for(int i=0; i<WEBCAM_FPS_HISTORY_SIZE-1; ++i){
						videoPlayerFPSHistory[i]=videoPlayerFPSHistory[i+1];
					}
					videoPlayerFPSHistory[WEBCAM_FPS_HISTORY_SIZE-1] = videoPlayerFPSCounter.getFps();
					
					
					textures[0] = player.getTexture();
				}
			}
		}
		
		else if(videoMode==VIDEO_MODE_UVC_WEBCAM ){
			//shaderToyArgs.iChannelTime[0]=player.getPosition();
			
#if defined( OF_VIDEO_CAPTURE_QTKIT )
			if( UVCWebcam.isReady() ){
				
				UVCWebcam.update();
				
				if(UVCWebcam.isFrameNew()){
					webcamFPSCounter.begin();
					webcamFPSCounter.tick("newframe");
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
#endif
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
	
	videoMode = VIDEO_MODE_FILE;
	bUseThreadedFileDecoding = true;
	
	videoMedia = karmaVideoMediaInformationStruct("");
	videoMedia.triggerUpdate();
	
	player.stop();
	player.closeMovie();
	//player.load(""); // starts player object
	
	loadShader( effectFolder("videoShader.vert"), effectFolder("videoShader.frag") );
	
	activeCamera = "";
 #if defined( OF_VIDEO_CAPTURE_QTKIT )
	UVCWebcam.close();
	UVCWebcam.setUseAudio(false);
 #endif
	webcamSettings.targetFPS = 30;
	webcamSettings.width = 1280;
	webcamSettings.height = 720;
	
	for(int i=0; i<WEBCAM_FPS_HISTORY_SIZE; ++i){
		webcamFPSHistory[i]=0;
		videoPlayerFPSHistory[i]=0;
	}

#ifdef KARMAMAPPER_DEBUG
#if defined( OF_VIDEO_CAPTURE_QTKIT )
    UVCWebcam.setVerbose(false);
#endif
#endif

#ifdef TARGET_OSX
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
		
		preset.name = "Caméra FaceTime HD (intégrée)"; // frenchies!!!
		preset.vendorId = 0x5ac;
		preset.productId = 0x850b;
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
#endif
	
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
		ImguiShowTextureMode();
		
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
            ImGui::LabelText("Video File Directory", "%s", videoMedia.get().path.c_str() );
            ImGui::LabelText("Video File Name", "%s", videoMedia.get().fileName.c_str() );
			
			if( ImGui::Button("Choose File...") ){
				ofFileDialogResult d = ofSystemLoadDialog("Choose a video file...");
				if(d.bSuccess){
					karmaVideoMediaInformationStruct mediaInfo( d.getPath() );
					loadVideoFile( mediaInfo );
				}
			}
			ImGui::SameLine();
			if( ImGui::Button("Reload") ){
				loadVideoFile( videoMedia );
			}
			bool tmpThreading = bUseThreadedFileDecoding;
			if(ImGui::Checkbox("Use threaded video decoding (experimental)", &tmpThreading)) {
				setUseThread(tmpThreading);
			}
			
			if( ImGui::DragFloat("playBackSpeed", &(videoMedia->playBackSettings.playbackSpeed), 0.05, 0.1) ){
				setVideoMediaSpeed(videoMedia->playBackSettings.playbackSpeed);
			}
			
			ImGui::Text("Video FPS: %f", videoPlayerFPSCounter.getFps() );
			ImGui::PlotHistogram("Histogram", videoPlayerFPSHistory, WEBCAM_FPS_HISTORY_SIZE, 0, NULL, 0.f, 60.f, ImVec2(0,80));
			
			float tmpSeekerPos = videoMedia->playBackSettings.seekerPosition;
			if( ImGui::SliderFloat("seekerPosition", &tmpSeekerPos, 0, 1) ){
				seekVideoMedia( tmpSeekerPos );
			}
			if( ImGui::Button("Stop") ){
				stopVideoMedia();
			}
			ImGui::SameLine();
			if( ImGui::Button("Play") ){
				startVideoMedia();
			}
			ImGui::SameLine();
			if( ImGui::Button("Pause") ){
				pauseVideoMedia(true);
			}
			
			if(ImGui::SliderFloat("Video volume", &videoMedia->playBackSettings.volume, 0.f, 1.f)){
				setVideoMediaVolume(videoMedia->playBackSettings.volume);
			}
			
		}
		
		else if(videoMode==VIDEO_MODE_UVC_WEBCAM){
			ImGui::TextWrapped("This mode reads a video stream from a UVC webcam.");
			ImGui::Text("Webcam source : %s", activeCamera.c_str());
			
			if(ImGui::TreeNode("Webcam Selection")){
#ifdef OF_VIDEO_CAPTURE_QTKIT
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
#else
                // No qtkit...
                ImGui::TextWrapped("[Not available, no Qtkit in this build]");
#endif
				ImGui::TreePop();
            } // end ImGui::TreeNode("Webcam Selection")
			
#ifdef OF_VIDEO_CAPTURE_QTKIT
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
				
				ImGui::Separator();
				ImGui::TextWrapped("Video Codecs:");
				ImGui::Indent();
				ImGui::TextWrapped("Sets the video codec used. Check status in console with KARMAMAPPER_DEBUG on.");
				ImGui::Separator();
				static auto codecs = UVCWebcam.listVideoCodecs();
				for(auto it=codecs.begin(); it!=codecs.end(); ++it){
					//UVCWebcam.
					if(ImGui::Selectable(it->c_str(), false, ImGuiSelectableFlags_DontClosePopups )){
						UVCWebcam.setVideoCodec(*it);
						selectUVCWebcam();
					}
				}
				ImGui::Unindent();
				
				ImGui::TreePop();
			}
// endif QT KIT
#endif

#ifdef TARGET_OSX
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
						//bool autoExposure = UVCController.getAutoExposure();
						if(ImGui::Checkbox("Auto Exposure", &webcamSettings.UVCAutoExposure)){
							UVCController.setAutoExposure(webcamSettings.UVCAutoExposure);
						}
						//float UVCExposure = UVCController.getExposure();
						if(ImGui::SliderFloat("Exposure", &webcamSettings.UVCExposure, 0, 1)){
							UVCController.setExposure(webcamSettings.UVCExposure);
						}
						//bool autoFocus = UVCController.getAutoFocus();
						if(ImGui::Checkbox("Auto Focus", &webcamSettings.UVCAutoFocus)){
							UVCController.setAutoFocus(webcamSettings.UVCAutoFocus);
						}
						//float UVCFocus = UVCController.getAbsoluteFocus();
						if(ImGui::SliderFloat("Focus", &webcamSettings.UVCFocus, 0, 1)){
							UVCController.setAbsoluteFocus(webcamSettings.UVCFocus);
						}
						//bool autoWB = UVCController.getAutoWhiteBalance();
						if(ImGui::Checkbox("Auto White Balance", &webcamSettings.UVCAutoWhiteBalance)){
							UVCController.setAutoWhiteBalance(webcamSettings.UVCAutoWhiteBalance);
						}
						//float UVCWhiteBalance = UVCController.getWhiteBalance();
						if(ImGui::SliderFloat("White Balance", &webcamSettings.UVCWhiteBalance, 0, 1)){
							UVCController.setWhiteBalance(webcamSettings.UVCWhiteBalance);
						}
						
						ImGui::Separator();
						ImGui::Separator();
						
						//float UVCContrast = UVCController.getContrast();
						if(ImGui::SliderFloat("Contrast", &webcamSettings.UVCContrast, 0, 1)){
							UVCController.setContrast(webcamSettings.UVCContrast);
						}
						//float UVCSaturation = UVCController.getSaturation();
						if(ImGui::SliderFloat("Saturation", &webcamSettings.UVCSaturation, 0, 1)){
							UVCController.setSaturation(webcamSettings.UVCSaturation);
						}
						//float UVCSharpness = UVCController.getSharpness();
						if(ImGui::SliderFloat("Sharpness", &webcamSettings.UVCSharpness, 0, 1)){
							UVCController.setSharpness(webcamSettings.UVCSharpness);
						}
						//float UVCGain = UVCController.getGain();
						if(ImGui::SliderFloat("Gain", &webcamSettings.UVCGain, 0, 1)){
							UVCController.setGain(webcamSettings.UVCGain);
						}
						//float UVCBrightness = UVCController.getBrightness();
						if(ImGui::SliderFloat("Brightness", &webcamSettings.UVCBrightness, 0, 1)){
							UVCController.setBrightness(webcamSettings.UVCBrightness);
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
// endif TARGET_OSX
#endif

        } // endif VIDEO_MODE_UVC_WEBCAM
		
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
    } // endif imgui videoshader panel
	ImGui::Separator();
	
	shaderEffect::printCustomEffectGui();
	
	return true;
}

void videoShader::ImguiShowTextureMode(){
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
		if(ImGui::Selectable("Multiple shapes", textureMode==4)){
			setTextureMode(4);
		}
		ImGui::ListBoxFooter();
	}
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool videoShader::saveToXML(ofxXmlSettings& xml) const {
	bool ret = shaderEffect::saveToXML(xml);
	
	xml.addValue("videoMode", static_cast<int>(videoMode) );

	xml.addValue("playBackSpeed", videoMedia.get().playBackSettings.playbackSpeed);
	xml.addValue("videoFile", videoMedia.get().getFullPath() );
	xml.addValue("videoIsPlaying", player.isPlaying());
	xml.addValue("videoPosition", player.getPosition());
	xml.addValue("videoIsPaused", player.isPaused());
	xml.addValue("videoVolume", videoMedia.get().playBackSettings.volume);
	xml.addValue("bUseThreadedFileDecoding", bUseThreadedFileDecoding.get() );

#ifdef KM_ENABLE_SYPHON
	xml.addValue("syphonServer", syphonAddr.serverName);
	xml.addValue("syphonApp", syphonAddr.appName);
#endif

	xml.addValue("activeCamera", activeCamera);
	xml.addValue("activeCameraTargetFPS", webcamSettings.targetFPS);
	xml.addValue("activeCameraWidth", webcamSettings.width);
	xml.addValue("activeCameraHeight", webcamSettings.height);
	
	
	xml.addValue("UVCAutoFocus", webcamSettings.UVCAutoFocus );
	xml.addValue("UVCFocus", webcamSettings.UVCFocus);
	
	xml.addValue("UVCAutoWhiteBalance", webcamSettings.UVCAutoWhiteBalance );
	xml.addValue("UVCWhiteBalance", webcamSettings.UVCWhiteBalance);
	
	xml.addValue("UVCAutoExposure", webcamSettings.UVCAutoExposure );
	xml.addValue("UVCExposure", webcamSettings.UVCExposure);
	
	xml.addValue("UVCContrast", webcamSettings.UVCContrast);
	xml.addValue("UVCSaturation", webcamSettings.UVCSaturation);
	xml.addValue("UVCSharpness", webcamSettings.UVCSharpness);
	xml.addValue("UVCGain", webcamSettings.UVCGain);
	xml.addValue("UVCBrightness", webcamSettings.UVCBrightness);
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool videoShader::loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene){
	bool ret = shaderEffect::loadFromXML(xml, _scene);
	
	ret *= loadShader( effectFolder("videoShader.vert", "videoShader"), effectFolder("videoShader.frag", "videoShader") );
	
	bUseShadertoyVariables = true;
	bUseTextures = true;
	
    setVideoMode( static_cast<enum videoModeEnum>(xml.getValue("videoMode", VIDEO_MODE_FILE )) );
    
//    if(!bUseThreadedFileDecoding){
//        setVideoFilePlaybackInformation(player, videoMedia->playBackSettings );
//    }
//    else {
//        videoMedia.triggerUpdate();
//    }
	
	webcamSettings.targetFPS = xml.getValue("activeCameraTargetFPS", webcamSettings.targetFPS);
	webcamSettings.width = xml.getValue("activeCameraWidth", webcamSettings.width);
	webcamSettings.height = xml.getValue("activeCameraHeight", webcamSettings.height);
	selectUVCWebcam(xml.getValue("activeCamera", "none"));
	
	webcamSettings.UVCAutoFocus = xml.getValue("UVCAutoFocus", webcamSettings.UVCAutoFocus );
	webcamSettings.UVCFocus = xml.getValue("UVCFocus", webcamSettings.UVCFocus);
	webcamSettings.UVCAutoWhiteBalance = xml.getValue("UVCAutoWhiteBalance", webcamSettings.UVCAutoWhiteBalance );
	webcamSettings.UVCWhiteBalance = xml.getValue("UVCWhiteBalance", webcamSettings.UVCWhiteBalance);
	webcamSettings.UVCAutoExposure = xml.getValue("UVCAutoExposure", webcamSettings.UVCAutoExposure );
	webcamSettings.UVCExposure = xml.getValue("UVCExposure", webcamSettings.UVCExposure);
	
	webcamSettings.UVCContrast = xml.getValue("UVCContrast", webcamSettings.UVCContrast);
	webcamSettings.UVCSaturation = xml.getValue("UVCSaturation", webcamSettings.UVCSaturation);
	webcamSettings.UVCSharpness = xml.getValue("UVCSharpness", webcamSettings.UVCSharpness);
	webcamSettings.UVCGain = xml.getValue("UVCGain", webcamSettings.UVCGain);
	webcamSettings.UVCBrightness = xml.getValue("UVCBrightness", webcamSettings.UVCBrightness);
	
#ifdef TARGET_OSX
	// set webcam settings
	if(selectedUVCControlCamera!=nullptr){
		UVCController.setAutoExposure(webcamSettings.UVCAutoExposure);
		if(!webcamSettings.UVCAutoExposure){
			UVCController.setExposure(webcamSettings.UVCExposure);
		}
		UVCController.setAutoFocus(webcamSettings.UVCAutoFocus);
		if(!webcamSettings.UVCAutoFocus){
			UVCController.setAutoFocus(webcamSettings.UVCFocus);
		}
		UVCController.setAutoWhiteBalance(webcamSettings.UVCAutoWhiteBalance);
		if(!webcamSettings.UVCAutoWhiteBalance){
			UVCController.setWhiteBalance(webcamSettings.UVCWhiteBalance);
		}
		UVCController.setContrast(webcamSettings.UVCContrast);
		UVCController.setSaturation(webcamSettings.UVCSaturation);
		UVCController.setSharpness(webcamSettings.UVCSharpness);
		UVCController.setGain(webcamSettings.UVCGain);
		UVCController.setBrightness(webcamSettings.UVCBrightness);
	}
#endif
	
#ifdef KM_ENABLE_SYPHON
	connectToSyphonServer( ofxSyphonServerDescription(
		xml.getValue("syphonServer", syphonAddr.serverName),
		xml.getValue("syphonApp", syphonAddr.appName)
	));
#endif
	
	setUseThread( xml.getValue("bUseThreadedFileDecoding", true) );
	videoMedia = karmaVideoMediaInformationStruct( xml.getValue("videoFile", videoMedia->getFullPath() ) );
	videoMedia->playBackSettings.volume = xml.getValue("videoVolume", videoMedia->playBackSettings.volume);
	videoMedia->playBackSettings.playbackSpeed = xml.getValue("playBackSpeed", videoMedia->playBackSettings.playbackSpeed);
	videoMedia->playBackSettings.seekerPosition = xml.getValue("videoPosition", videoMedia->playBackSettings.seekerPosition);
	
	videoMedia->playBackSettings.isPlaying = xml.getValue("videoIsPlaying", videoMedia->playBackSettings.isPlaying);
	videoMedia->playBackSettings.isPaused = xml.getValue("videoIsPaused", videoMedia->playBackSettings.isPlaying);
	
	loadVideoFile( videoMedia, true );
	
	bIsLoading = false;
	
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
void videoShader::setVideoMode(const enum videoModeEnum& mode){
	
    if (mode == VIDEO_MODE_FILE) {
		videoMode = mode;
		
		if( !player.isLoaded() ) setError(true, "Please select a source video.");
	}
	
    else if (mode == VIDEO_MODE_UVC_WEBCAM ) {
		videoMode = mode;
		
		if( !selectUVCWebcam() ) setError(true, "Please select a source video.");
	}
	
#ifdef KM_ENABLE_SYPHON
    else if (mode == VIDEO_MODE_SYPHON) {
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

bool videoShader::loadVideoFile(const karmaVideoMediaInformationStruct &_media, bool forceRemember) {
	
	// remember information for anytime
	if( forceRemember ){
		videoMedia = _media;
	}
    
	if (videoMode==VIDEO_MODE_FILE) {
		
		ofFile file( _media.getFullPath() );
		if( file && file.exists() ){
			
			//videoMedia->playBackSettings.volume = fVideoVolume;
			//videoMedia->playBackSettings.loopState = OF_LOOP_NORMAL;
			//videoMedia->playBackSettings.playbackSpeed = playBackSpeed;
			
			// todo : finish this part
			if( bUseThreadedFileDecoding ){
				player.stop();
				
				if(!isThreadRunning()){
					startThread();
				}
			}
			
			// no threaded file decoding
			else {
				//player.setUseTexture( true );
				//player.ofBaseVideoPlayer::loadAsync(string name)
				if(player.load( _media.getFullPath() ) /*&& player.isInitialized()*/ ){
                    
                    videoMedia = _media; // also triggers update on thread
					
					videoMedia->mediaDimensions[0] = player.getWidth();
					videoMedia->mediaDimensions[1] = player.getHeight();
					videoMedia->isVideoFile = true;
					
					setVideoFilePlaybackInformation(player, videoMedia->playBackSettings);
					
					//player.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
					//player.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
					shaderToyArgs.iChannelResolution[0] = videoMedia->mediaDimensions[0];
					shaderToyArgs.iChannelResolution[1] = videoMedia->mediaDimensions[1];
					shaderToyArgs.iChannelResolution[2] = shaderToyArgs.iChannelResolution[0] / shaderToyArgs.iChannelResolution[1];
                    
                    //player.setPosition(videoMedia->playBackSettings.seekerPosition);
					//player.play();
                    setVideoFilePlaybackInformation(player, videoMedia->playBackSettings);
					
					// sync video Media (makes sure to stop threaded playback)
					videoMedia.triggerUpdate();
				}
				
			}
			
			textures.clear();
			textures.push_back( ofTexture() );
			textures.back().allocate(videoMedia->mediaDimensions[0], videoMedia->mediaDimensions[1], GL_RGB);
			shaderToyArgs.iChannelTime[0]=0.f;
			
			// tmp, this could possibly reset another error then the video file error... // todo
			if(bHasError){
				setError(false);
			}
			
			ofLogNotice("videoShader::loadVideoFile") << "Loaded "<< videoMedia->getFullPath() << ".";
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

#if !defined( OF_VIDEO_CAPTURE_QTKIT )
    return false;
#else

	int camNum = -1;
	vector<string> cams = UVCWebcam.listVideoDevices();
	
#ifdef TARGET_OSX
	selectedUVCControlCamera = nullptr;
#endif
	
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
			UVCWebcam.initRecording();
			//UVCWebcam.setup(-1,-1); // native dimensions
			UVCWebcam.setup(webcamSettings.width,webcamSettings.height);
			UVCWebcam.setDesiredFrameRate(webcamSettings.targetFPS);
			
			
			if(UVCWebcam.isInitialized()){
				shaderToyArgs.iChannelResolution[0*3+0] = UVCWebcam.getWidth();
				shaderToyArgs.iChannelResolution[0*3+1] = UVCWebcam.getHeight();
				shaderToyArgs.iChannelResolution[0*3+2] = UVCWebcam.getWidth() / UVCWebcam.getHeight();
				shaderToyArgs.iChannelTime[0]=0.f;
				
				textures.clear();
				textures.push_back( ofTexture() );
				textures.back().allocate(player.getWidth(), player.getHeight(), GL_RGB);
#ifdef TARGET_OSX
				// check if UVC controls are available ?
				for(auto it=UVCUsbPresets.begin(); it!=UVCUsbPresets.end(); ++it){
					// on mac devices sometimes get renamed to `device name #2`
					string cleanName = activeCamera;
					if(activeCamera.find(" #")!=activeCamera.npos){
						cleanName = cleanName.replace(activeCamera.find(" #"), activeCamera.npos, "");
					}

					if(it->name.compare( cleanName ) == 0){
						UVCController.useCamera(it->vendorId, it->productId, it->interfaceNum);
						selectedUVCControlCamera = &(*it);
						#ifdef KARMAMAPPER_DEBUG
						ofLogVerbose("videoShader::selectUVCWebcam") << "Found matching UVC control presets for camera " << selectedUVCControlCamera->name;
						#endif
					}
				}
#endif
				
				setError(false);
			}
			else{
				setError(true, "UVC Camera Error...");
			}
		}
	}
	
	return true;
// endif QT KIT
#endif

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
	
	bUseThreadedFileDecoding = _useThread;
	
	if (videoMode == VIDEO_MODE_FILE) {
		
		if (!_useThread) {
			if(isThreadRunning()){
				waitForThread(true);
			}
		}
		else if (!isThreadRunning()) {
			startThread();
		}
	}
	else {
		if(!_useThread && isThreadRunning()){
			waitForThread(true);
			
			if(videoMedia->playBackSettings.isPlaying){
				player.play();
				player.setPaused( videoMedia->playBackSettings.isPaused );
			}
			else {
				player.stop();
			}
		}
		else if(_useThread && !isThreadRunning()){
			player.stop();
			//startThread();
		}
	}
}

void videoShader::setVideoFilePlaybackInformation(ofVideoPlayer& _player, karmaVideoPlaybackSettingsStruct _settings) {
	
	if( !_player.isInitialized() ){
		return;
	}
	
	_player.setVolume(_settings.volume);
	_player.setSpeed(_settings.playbackSpeed);
	_player.setLoopState(_settings.loopState);
	_player.setPosition(_settings.seekerPosition);
	
	if( _settings.isPlaying ){
		if(!_player.isPlaying()){
			_player.play();
		}
		
		if(_settings.isPaused){
			if(!_player.isPaused()){
				_player.setPaused(true);
			}
		}
		else {
			if(_player.isPaused()){
				_player.setPaused(false);
			}
		}
	}
	else {
		if(_player.isPlaying()){
			_player.stop();
		}
		
	}
}

void videoShader::startVideoMedia(){
	videoMedia->playBackSettings.isPlaying = true;
	
	if(!bUseThreadedFileDecoding){
		if(videoMedia->playBackSettings.isPlaying){
			player.play();
		}
		else {
			player.stop();
		}
	}
	else {
		videoMedia.triggerUpdate();
	}
}

void videoShader::stopVideoMedia(){
	
	videoMedia->playBackSettings.isPlaying = false;
	
	if(!bUseThreadedFileDecoding){
		player.stop();
	}
	else {
		videoMedia.triggerUpdate();
	}
}

void videoShader::pauseVideoMedia(const bool &paused){
	videoMedia->playBackSettings.isPaused = paused;
	
	if(!bUseThreadedFileDecoding){
		player.setPaused( videoMedia->playBackSettings.isPaused );
	}
	else {
		videoMedia.triggerUpdate();
	}
}

void videoShader::seekVideoMedia(const float& _pos){
	videoMedia->playBackSettings.seekerPosition = _pos;
	
	if(!bUseThreadedFileDecoding){
		player.setPosition( videoMedia->playBackSettings.seekerPosition );
	}
	else {
		videoMedia.triggerUpdate();
	}
}

void videoShader::setVideoMediaVolume(const float &_vol){
	videoMedia->playBackSettings.volume = _vol;
	
	if(!bUseThreadedFileDecoding){
		player.setVolume( videoMedia->playBackSettings.volume );
	}
	else {
		videoMedia.triggerUpdate();
	}
}

void videoShader::setVideoMediaSpeed(const float &_speed){
	videoMedia->playBackSettings.playbackSpeed = _speed;
	
	if(!bUseThreadedFileDecoding){
		player.setSpeed( videoMedia->playBackSettings.playbackSpeed );
	}
	else {
		videoMedia.triggerUpdate();
	}
}

void videoShader::threadedFunction(){

#ifdef KARMAMAPPER_DEBUG
	//ofThread::setThreadName("videoShader::threadedFunction() " + ofToString(ofThread::getThreadId()) );
#endif
	
	while (isThreadRunning()) {
		
		// sync playback variables
		if( bUseThreadedFileDecoding.syncFromThread() ){
			
			if(!bUseThreadedFileDecoding.getFromThread()){
				threadedPlayer.close();
				// stop thread execution
				break;
			}
			else {
				threadedPlayer.setUseTexture(false);
			}
		}
		
		if( videoMedia.syncFromThread() ){
			// reload movie ?
			if( threadedPlayer.getMoviePath().compare(videoMedia.getFromThread().getFullPath() )!=0 ){
				threadedPlayer.setUseTexture(false);
				
				threadedPlayer.closeMovie();
				if( threadedPlayer.load( videoMedia.getFromThread().getFullPath() ) ){
					
					videoMedia->mediaDimensions[0] = threadedPlayer.getWidth();
					videoMedia->mediaDimensions[1] = threadedPlayer.getHeight();
					videoMedia->isVideoFile = true;
				}
			}
			
			// resync some other settings
			//threadedPlayer.setSpeed( videoMedia->playBackSettings.playbackSpeed );
			//threadedPlayer.setVolume( videoMedia->playBackSettings.volume );
			
//			if(videoMedia->playBackSettings.isPlaying){
//				threadedPlayer.setPosition( videoMedia->playBackSettings.seekerPosition );
//				
//				if(!threadedPlayer.isPlaying()){
//					threadedPlayer.play();
//				}
//				
//				threadedPlayer.setPaused( videoMedia->playBackSettings.volume );
//			}
//			else if(!videoMedia->playBackSettings.isPlaying) {
//				threadedPlayer.stop();
//				//threadedPlayer.setPosition( videoMedia->playBackSettings.seekerPosition );
//			}
			
			
			// resync playback information
			setVideoFilePlaybackInformation(threadedPlayer, videoMedia->playBackSettings);
			
		}
		
		if( bUseThreadedFileDecoding ){
					
			if (threadedPlayer.isLoaded() && threadedPlayer.isInitialized() ){
				
				threadedPlayer.update();
				videoMedia->playBackSettings.seekerPosition = threadedPlayer.getPosition();
	
				if( (threadedPlayer.isFrameNew() /*|| player.getPosition()<0*/) && newImagesFromThread.empty() ){
					kmNewVideoFrameStruct newFrame(threadedPlayer.getPixels());
					
					if(newFrame.newPixels.isAllocated()){
						newFrame.position = videoMedia->playBackSettings.seekerPosition;
						newImagesFromThread.send( std::move(newFrame) );
					}
				}
			}
			unlock();
			//sleep(16); // limits CPU usage, todo: could be revised #dirty
		}
		
		else {
			break;
		}
	}
	//stopThread();
	threadedPlayer.stop();
	threadedPlayer.closeMovie();
}

// register effect type
EFFECT_REGISTER( videoShader , "videoShader" );
