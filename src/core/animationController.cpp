//
//  animationController.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/03/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "animationController.h"
#include "ofxVLCRemote.h"
#include "durationRC.h"

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
animationController::animationController( shapesDB& _scene ): scene(_scene){
	
	ofSetLoggerChannel( karmaConsoleChannel::getLogger() );
	
	bEnabled = false;
	bShowGui = true;
	
	bShowMouse = true;
	bIsFullScreen = false;
	bGuiShowAnimParams = false;
	bGuiShowPlugins = false;
	loadedConfiguration = "";
	bGuiShowConsole = false;
	bGuiShowModules = false;
	
	effects.clear();
	effects.resize(0);
	
	ofAddListener( ofEvents().draw , this, &animationController::draw, OF_EVENT_ORDER_APP );
	ofAddListener( ofEvents().update , this, &animationController::update, OF_EVENT_ORDER_APP );
	
	ofAddListener( ofEvents().keyPressed, this, &animationController::_keyPressed, OF_EVENT_ORDER_APP );
	
	// build GUI
	gui.setup();
	ImGui::GetIO().MouseDrawCursor = false;
}

animationController::~animationController(){
	ofRemoveListener( ofEvents().draw , this, &animationController::draw );
	ofRemoveListener( ofEvents().update , this, &animationController::update );
	
	ofRemoveListener( ofEvents().keyPressed , this, &animationController::_keyPressed );
	
	// rm GUI stuff correctly
	//guiLoadShapesSceneBtn.removeListener(this, &animationController::showShapeLoadMenu);
	bShowGui = false;

}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
bool animationController::start(){
	
	// all black! xD
	ofSetBackgroundAuto(false);
	ofClear(0,0,0,255);
	
	// syphon
#ifdef TARGET_OS_MAC
//	syphonTexture.setName("Texture Output");
#endif
	
	// play music
	//sound.loadSound("TEST MIX V0.1.wav");
	//music.load("music.wav");
	//music.setLoop(true);
	//music.play();
	
	// Setup video recording
	//recorder.startRecording("", 850, 700);
	
//	{
//		// play music with VLC
//		ofxVLCRemote vlc;
//		vlc.setup("/Users/daan/vlc.sock"); // client-specific... :(
//		vlc.run("repeat on");
//		vlc.run("normal"); // playback speed
//		vlc.run("volume 256"); // 0-1024 (256=normal)
//		
//		//vlc.run("adev 7"); // tell VLC to use device 7 (soundflower 2ch)
//		vlc.run("play");
//		vlc.run("prev");
//		//vlc.run("got0 0");
//		
//	}
//	
//	{	// SYNC DURATION
//		durationRC rc;
//		rc.setupOSC();
//		rc.rewindAndPlay();
//	}
	
	
	// setup the OSC Router
	oscRouter.start();
	
	// enable mirOSCReceiver
	mirOSCReceiver.enable(true);
	mirOSCReceiver.start();
	oscRouter.addNode( &mirOSCReceiver );
	
	// enable durationOSCReceiver
	//durationOSCReceiver.start();
	//oscRouter.addNode( &durationOSCReceiver );
	
	// get shape instance
	//string type="basicEffect";
	//e = stringToShapeClass( type );
	
	// re-enable when uncommenting 2 lines above
	/*if(e==NULL){
	 ofLogError("--> Could not create an effect of type "+type);
	 return;
	 }*/
	
	/*
	if(false){	// VIDEO EFFECT
		basicEffect* e;
		e = new videoEffect();
		e->initialise();
		e->bindWithShape( scene.getRandomShapeByGroup(5) ); // 3 = bg
		effects.push_back(e);
	}
	
	//
	if(true){	// IMAGE EFFECT
		basicEffect* e;
		e = new imageEffect();
		e->initialise();
		((imageEffect*) e)->loadImageFromFile("vendome_full_ok.jpg");
		((imageEffect*) e)->listenToOscIntensity("bgVendomeOpacity");
		e->bindWithShape( scene.getRandomShapeByGroup(5) ); // 3 = blue windows
		
		effects.push_back(e);
	}
	
	// no mutexes --> unstable
	if(true){	// IMAGE BEAT EFFECT
		basicEffect* e;
		e = new imageBeatEffect();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(3) ); // windows
		e->enable();
		effects.push_back(e);
	}
	
	if(false){	// BASIC EFFECT
		basicEffect* e;
		e = new basicEffect();
		e->initialise();
		e->bindWithShapes( scene.getAllShapes() );
		e->enable();
		effects.push_back(e);
	}
	
	if(false){	// LINE EFFECT
		basicEffect* e;
		e = new lineEffect();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(3) ); // windows
		e->bindWithShapes( scene.getShapesByGroup(0) ); // columns
		e->enable();
		effects.push_back(e);
	}
	
	/ *{
	basicEffect* e;
	e = new imageMeltingEffect();
	e->initialise();
	e->bindWithShapes( scene.getShapesByGroup(3) );
	e->bindWithShapes( scene.getShapesByGroup(1) );
	effects.push_back(e);
	}// */
	
	/*{
	basicEffect* e;
	e = new imageGrainEffect();
	e->initialise();
	e->bindWithShapes( scene.getShapesByGroup(1) );
	effects.push_back(e);
	} // */
	
	/*{ // MESH 3D
		basicEffect* e;
		e = new meshRenderer();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(1) );
		effects.push_back(e);
	} // */
	
	/*{ // GIF effect
		basicEffect* e;
		e = new gifEffect();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(1) );
		effects.push_back(e);
	}// */
	
//	{
//		basicEffect* e;
//		e = new distortEffect();
//		e->initialise(animationParams.params);
//		e->bindWithShapes( scene.getShapesRef() );
//		effects.push_back(e);
//	}
	
//	{
//		//ofSetLogLevel(OF_LOG_VERBOSE);
//		shaderEffect* e;
//		e = new shaderEffect();
//		e->initialise(animationParams.params);
//		e->bindWithShapes( scene.getShapesRef() );
//		//e->loadShader("",  "../../src/effects/shaderEffect/menger_journey.frag");
//		e->loadShader("../../src/effects/shaderEffect/defaultShader.vert", "../../src/effects/shaderEffect/defaultShader.frag" );
//		effects.push_back(e);
//	}
	
//	{
//		basicEffect* e;
//		e = new lineEffect();
//		e->initialise(animationParams.params);
//		e->bindWithShapes( scene.getShapesRef() );
//		effects.push_back(e);
//	}
	
	bEnabled = true;
	bShowGui = true;
	
	return isEnabled()==true;
}

bool animationController::stop(){
	
	// disable mirOSCReceiver
	mirOSCReceiver.stop();
	oscRouter.removeNode( &mirOSCReceiver );
	
	// disable durationOSCReceiver
	//durationOSCReceiver.stop();
	//oscRouter.removeNode( &durationOSCReceiver );
	
	oscRouter.stop();
	
	bShowGui = false;
	
//	{
//		// play music with VLC
//		ofxVLCRemote vlc;
//		vlc.setup("/Users/daan/vlc.sock"); // client-specific... :(
//		vlc.run("stop");
//	}
	
	{	// SYNC DURATION
//		durationRC rc;
//		rc.setupOSC();
//		rc.stop();
	}
	
	unloadAllEffects();
	
	// save recorded file ?
	//recorder.stopRecording();
	
	bEnabled = false;
	return !isEnabled();
}

bool animationController::isEnabled() const {
	return bEnabled;
}

// warning: if you use it, make sure to update your iterator after this if using one
bool animationController::removeEffect(basicEffect *_e){
	if( _e == nullptr) return false;
	
	auto it=std::find(effects.cbegin(), effects.cend(), _e);
	if( it != effects.cend() ){
		_e->disable();
		//delete _e;
		effects.erase( it );
	}
	
	return true;
}

// - - - - - - - -
// LOAD & SAVE
// - - - - - - - -
bool animationController::loadConfiguration(const string& _file){
	
	if( _file.empty() ){
		ofLogError("animationController::loadConfiguration") << "Not loading, no file name specified";
		return false;
	}
	
	bool success = false;
	ofxXmlSettings configXML;
	
	if( configXML.loadFile( ofToDataPath( KM_CONFIG_FOLDER +_file ) ) ){
		
		stop();
		
		// load shapes scene
		if( configXML.pushTag("sceneSettings") ){
			if( scene.loadScene( configXML.getValue("shapesFile", "")) ){
				loadedConfiguration = _file;
				start();
				success = true;
			}
			else {
				scene.unloadShapes();
				
				// todo: make this a GUI warning modal ?
				ofLogWarning("animationController::loadConfiguration") << "The config file is loading but it's associated shapes scene file was not found. ( continuing... )";
			}
			configXML.popTag(); // pop sceneSettings
		}
		else {
			scene.unloadShapes();
			ofLogNotice("animationController::loadConfiguration") << "The config file has no shapes scene file. Loading effects without shapes.";
		}
		
		// load effects
		if(	configXML.pushTag("effects") ){
			
			// loop for each effect
			int numEffects = configXML.getNumTags("effect");
			vector<int> failedEffects;
			failedEffects.clear();
			
			for(int e=0; e<numEffects; e++){
				if( configXML.pushTag("effect", e) ){
				
					// Some code comes from:
					// --> http://stackoverflow.com/questions/8269465/how-can-i-instantiate-an-object-knowing-only-its-name
					string effectType = configXML.getValue("effectType", "basicEffect");
					basicEffect* effect = effect::create(effectType);
					if( effect != nullptr ){
						effects.push_back( effect );
						effects.back()->initialise(animationParams.params);
						effects.back()->loadFromXML( configXML );
						
						if( configXML.pushTag("boundShapes") ){
						
							// bind with previous shapes
							int numShapes = configXML.getNumTags("shape");
							vector<string> failedShapes;
							failedShapes.clear();
							
							for(int s=0; s<numShapes; s++){
								
								if( configXML.tagExists("shape", s) ){
									
									// get shape instance
									string tmpName = configXML.getAttribute("shape", "name", "", s);
									basicShape* tmpShape = scene.getShapeByName( tmpName );
									
									if(tmpShape != nullptr){
										if( !effects.back()->bindWithShape(tmpShape) ){
											tmpName += "(failed binding with ";
											tmpName += configXML.getAttribute("shape", "type", "undefined type", s);
											tmpName += ")";
											failedShapes.push_back( (tmpName) );
										}
									}
									else {
										tmpName += "(";
										tmpName += configXML.getAttribute("shape", "type", "undefined type", s);
										tmpName += " = not found)";
										failedShapes.push_back( (tmpName) );
									}
									
									//configXML.popTag();
								}
								
								// todo: (important) adapt this structure in save process
							}
							
							// todo: make this GUI message and show details
							if(failedShapes.size() > 0) ofLogWarning("animationController::loadConfiguration") << " Effect '" << effectType << "' loaded but failed to bind with " << failedShapes.size() << " out of " << numShapes << " shapes... (ignoring, but re-saving the configuration will erase this information).";
							
							configXML.popTag();
						}
					}
					else {
						failedEffects.push_back(e);
						// unknow effect type
						ofLogError("effect::create") << "Effect type not found: " << effectType;
					}
					
					configXML.popTag(); // pop effect
				}
			}
			
			ofLogNotice("animationController::loadConfiguration") << "Loaded configuration from " << _file << " [" << numEffects << " effects] ( " << failedEffects.size() << "failed )";
			
			success = true;
			
			configXML.popTag(); // pop effects
		}
		else {
			if( !success ){
				// todo: make this a GUI message
				ofLogError("animationController::loadConfiguration") << "The config file has effects file. Loading effects without shapes.";
			}
			else {
				ofLogNotice("animationController::loadConfiguration") << "The config file has no effects file but it's shapes have been loaded.";
			}
		}
	}
	else {
		// todo: make this a GUI message
		ofLogError("animationController::loadConfiguration") << "404 - The config file `"<< _file << "` does not exist or is not readable.";
	}
	
	if(success){
		loadedConfiguration = _file;
		configXML.clear();
		configXML.setValue("lastLoadedConfiguration", _file );
		if(!configXML.saveFile(ofToDataPath(KM_LAST_CONFIG_FILE)) ){
			ofLogWarning("animationController::loadConfiguration") << "Failed saving lastLoadedConfiguration... (continuing...)";
		}
	}
	else{
		//ofLogError("animationController::loadConfiguration", "Loading from `"+_file+"` failed!");
	}
	
	return success;
}

bool animationController::loadLastConfiguration(){
	
	
	ofxXmlSettings prevSettings;
	bool success = false;
	
	// what are we doing here?!?!
	if( prevSettings.loadFile( ofToDataPath(KM_LAST_CONFIG_FILE) ) ){
		
		// load created shapes
		success = loadConfiguration( prevSettings.getValue("lastLoadedConfiguration", KM_CONFIG_DEFAULT) );
		
		prevSettings.clear();
	}
	else {
		ofLogNotice("animationController::")<< __FUNCTION__ << "Previous configuration file not found. Not loading any scene.";
	}
	
	return success;
}

bool animationController::saveConfiguration( const string& _fileName ){
	bool success = false;
	
	string fullPath;
	// no save file ?
	if( _fileName.empty() ){
		if( loadedConfiguration.empty() ){
			ofSystemAlertDialog("Please provide a name for the save file!");
			return false;
		}
		else fullPath = KM_CONFIG_FOLDER+loadedConfiguration;
	}
	else fullPath = KM_CONFIG_FOLDER+_fileName;
	
	ofxXmlSettings sceneXML;
	sceneXML.addTag("sceneSettings");
	sceneXML.pushTag("sceneSettings");
	sceneXML.setValue("shapesFile", scene.getLoadedScene() );
	sceneXML.popTag();
	
	sceneXML.addTag("effects");
	sceneXML.pushTag("effects");
	
	// save all shapes data
	int e=0;
	vector<basicEffect*> failed;
	failed.clear();
	for(auto it = effects.begin(); it != effects.end(); it++, e++){
		
		sceneXML.addTag("effect");
		sceneXML.pushTag("effect", e);
		
		if(!(*it)->saveToXML(sceneXML)) failed.push_back(*it);
		
		sceneXML.popTag(); //pop shape
	}
	
	// write down settings to disk
	if( sceneXML.saveFile(fullPath) ){
		loadedConfiguration = ofFilePath::getFileName(fullPath);
		success = true;
		if(failed.size() == 0 ) ofLogNotice("animationController::saveConfiguration()", "Saved current configuration to `"+fullPath+"`");
		else ofSystemAlertDialog("The configuration has been saved but "+ ofToString(failed.size()) +" out of "+ ofToString(effects.size()) +" shapes failed to save.");
	}
	else ofSystemAlertDialog("Could not save the current configuration... :(\nSave File: "+fullPath);
	
	return success;
}

bool animationController::unloadAllEffects(){
	
	for( auto it = effects.rbegin(); it != effects.rend(); ++it){
		delete (*it); // todo: does this call destructor ?
		effects.erase( std::next(it).base() );
	}
	
	effects.resize(0);
	
	// file is not loaded anymore
	loadedConfiguration = "";
	
	return true;
}

void animationController::newConfiguration(){
	unloadAllEffects();
	scene.unloadShapes();
}

// - - - - - - - -
// GETTERS // UTILITIES
// - - - - - - - -
const unsigned int animationController::getNumEffects() const {
	return effects.size();
}

vector<basicEffect*> animationController::getEffectsByType(string _type){
	vector<basicEffect*> ret;
	ret.clear();
	ret.resize(0);
	
	// loop trough effects and return the wanted ones
	for(auto it = effects.begin(); it != effects.end(); it++){
		if((*it)->getType() == _type ) ret.push_back((*it));
	}
	
	return ret;
}

map<string, vector<basicEffect*> > animationController::getAllEffectsByType() const {
	map<string, vector<basicEffect*> > ret;
	for( auto it=effects.begin(); it!=effects.end(); ++it ){
		if( ret.find( (*it)->getType() ) == ret.end() ){
			ret[ (*it)->getType() ] = vector<basicEffect*>();
		}
		ret[ (*it)->getType() ].push_back(*it);
	}
	return ret;
}

// - - - - - - - -
// EVENT LISTENERS
// - - - - - - - -
void animationController::update(ofEventArgs &event){
	if(!isEnabled()) return;
	
	// reset shapes data to original state
	// every frame, effects can alter this
	for(auto it=scene.getShapesRef().begin(); it!=scene.getShapesRef().end(); ++it){
		(*it)->resetToScene();
	}
	
	// todo:
	// update/create animation state (bunch of variables)
	
	// update effects (run mode)
	for(int i=0; i<effects.size(); i++){
		effects[i]->update(animationParams.params);
	}
	
	// update modules
	for(int i=0; i<modules.size(); i++){
		modules[i]->update( animationParams.params );
	}
}

void animationController::draw(ofEventArgs& event){
	if(!isEnabled()) return;
	
	//ofScale(0.5, 0.5); // tmp
	
	//recorder.beginFrame();
	
	// tmp
	ofClear(0,1);
	//ofBackground(255,0,0);
	
	// draw modules
	for(int i=0; i<modules.size(); i++){
		modules[i]->draw(animationParams.params);
	}
	
	// render a scene without effects (tmp?)
	if(effects.size()==0 || ofGetKeyPressed(OF_KEY_LEFT_SHIFT)){
		ofSetColor( ofFloatColor(1.f, 1));//params.seasons.summer));
		ofFill();
		for(auto it=scene.getShapesRef().begin(); it!=scene.getShapesRef().end(); ++it){
			(*it)->sendToGPU();
		}
	}
	
	// draw effects
	else for(int i=0; i<effects.size(); i++){
		effects[i]->render(animationParams.params);
	}
	
	//recorder.endFrame(true);
	
	// syphon
#ifdef TARGET_OS_MAC
	//if(recorder.isRecording()){
//		syphonTexture.publishTexture( &recorder.getTexture() );
	//}
	//else {
//		syphonTexture.publishScreen();
	//}
#endif
	
	// draw gui stuff
	gui.begin();
	static bool ShowSaveAsModal = false;
	if( bShowGui ){
		
		ImGui::BeginMainMenuBar();
		if( ImGui::BeginMenu("karmaMapper :: Animator") ){
			// todo: put all this karmaMapper app data in a global struct
			
			if (ImGui::MenuItem("Version", KM_VERSION )) {}
			
			ImGui::MenuItem("FPS", ofToString( ofGetFrameRate() ).c_str() );
			
			ImGui::MenuItem("Application load", "#todo" );
			
			char buffer[26];
			snprintf(buffer, 26, "Resolution: %d x %i", ofGetWidth(), ofGetHeight() );
			static int int2[2] = { ofGetWidth(),ofGetHeight() };
			if(ImGui::BeginMenu( buffer )){
				
				ImGui::InputInt2("", int2);
				ImGui::SameLine();
				if( ImGui::Button("Set") ){
					ofSetWindowShape(int2[0], int2[1]);
					int2[0] = ofGetWidth();
					int2[1] = ofGetHeight();
				}
				ImGui::EndMenu();
			}
			
			static bool useVsync;
			if( ImGui::MenuItem("Enable v-sync", NULL, &useVsync) ) {
				ofSetVerticalSync(useVsync);
			}
			ImGui::SameLine();
			ImGui::Checkbox("", &useVsync);
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("File")){
			if( ImGui::MenuItem(GUINewConfiguration, ofToString(KM_CTRL_KEY_NAME).append( " + N" ).c_str()) ) {
				newConfiguration();
			}
			
			if (ImGui::BeginMenu("Load configuration..." ) ){
				
//				if (ImGui::MenuItem("File browser...", ofToString(KM_CTRL_KEY_NAME).append( " + O" ).c_str() )){
//				}
//				ImGui::Separator();
				
				ofDirectory dir;
				dir.listDir( ofToDataPath( KM_CONFIG_FOLDER ) );
				dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
				if( dir.size() <= 0 ){
					ImGui::Text("No files in %s", KM_CONFIG_FOLDER);
					ImGui::Separator();
				} else {
					for (int i = 0; i < dir.size(); i++){
						if( ImGui::MenuItem( dir.getName(i).c_str(), "", (bool)(scene.getLoadedScene() == dir.getName(i)) ) ){
							
							// todo: add error feedback here
							loadConfiguration(dir.getName(i));

						}
					}
				}
				ImGui::EndMenu();
			}
			
			if ( !loadedConfiguration.empty() && scene.getLoadedScene()!="" && ImGui::MenuItem("Save configuration", ofToString(KM_CTRL_KEY_NAME).append( " + S" ).c_str() )){
				saveConfiguration();
			}
			
			if (ImGui::MenuItem("Save as...", ofToString(KM_CTRL_KEY_NAME).append( " + SHIFT + S" ).c_str() )){
				// ImGui::OpenPopup("Save config as...");
				// tmp workaround: https://github.com/ocornut/imgui/issues/331
				ShowSaveAsModal = true;
			}
			
			ImGui::MenuItem("", "", false, false);
			
			if (ImGui::BeginMenu("Load Scene...")){
				if (ImGui::MenuItem("File browser...", ofToString(KM_CTRL_KEY_NAME).append( " + O" ).c_str() )){
					showShapeLoadMenu();
				}
				ImGui::Separator();
				
				ofDirectory dir;
				dir.listDir( ofToDataPath( KM_SCENE_SAVE_PATH ) );
				dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
				if( dir.size() <= 0 ){
					ImGui::Text("No files in %s", KM_SCENE_SAVE_PATH);
					ImGui::Separator();
				} else {
					for (int i = 0; i < dir.size(); i++){
						if( ImGui::MenuItem( dir.getName(i).c_str(), "", (bool)(scene.getLoadedScene() == dir.getName(i)) ) ){
							stop();
							if( scene.loadScene( dir.getName(i) ) ){
								start();
							}
						}
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options")){
			if (ImGui::MenuItem("Full screen", ofToString(KM_CTRL_KEY_NAME).append( " + F" ).c_str(), &bIsFullScreen )){
				setFullScreen( bIsFullScreen );
			}
			if (ImGui::MenuItem("Show Gui", ofToString(KM_CTRL_KEY_NAME).append( " + G" ).c_str(), &bShowGui )){
				//bShowGui = !bShowGui;
				//tmp_show_gui = bShowGui;
			}
			if (ImGui::MenuItem("Show Mouse Cursor", ofToString(KM_CTRL_KEY_NAME).append( " + M" ).c_str(), &bShowMouse )){
				setShowMouse(bShowMouse);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Extras")){
			if (ImGui::MenuItem(GUIEffectsViewParams, NULL, &bGuiShowAnimParams )){
				showAnimationsGui(bGuiShowAnimParams);
			}
			
			ImGui::MenuItem(GUIToggleConsole, NULL, &bGuiShowConsole);
			
			ImGui::MenuItem(GUIShowModules, NULL, &bGuiShowModules );
			
			ImGui::MenuItem(GUIShowPlugins, NULL, &bGuiShowPlugins );
			
			ImGui::EndMenu();
		}
		
//		if( ImGui::BeginMenu("") ){
//			
//			ImGui::EndMenu();
//		}
		ImGui::EndMainMenuBar();
		
		if(bGuiShowPlugins){
			ImGui::Begin("Karmamapper Plugins", &bGuiShowPlugins );
			
			ImGui::TextWrapped("karmaMapper hard-codes all its plugins on compilation time while they remain dynamically instantiable.\nThis window shows all plugins that have been compiled in this release.\n\n");
			
			if( ImGui::CollapsingHeader( GUIShapeTypesInfo, "GUIShapeTypesInfo", true, true ) ){
				for(auto it = shape::factory::getShapeRegistry().begin(); it!= shape::factory::getShapeRegistry().end(); ++it){
					
					ImGui::Text( "%s", it->first.c_str() );
				}
			}
			
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			
			if( ImGui::CollapsingHeader( GUIEffectTypesInfo, "GUIEffectTypesInfo", true, true ) ){
				for(auto it = effect::factory::getEffectRegistry().begin(); it!= effect::factory::getEffectRegistry().end(); ++it){
					
					ImGui::Text( "%s", it->first.c_str() );
				}
			}
			
			ImGui::End();
		}
		
		ImGui::Begin("karmaMapper", &bShowGui, ImVec2(400, ofGetHeight()*.8f) );
		
//		if( ImGui::Button("Hide Gui") ){
//			bShowGui = !bShowGui;
//		}
		
		ImGui::Spacing();
		if( ImGui::CollapsingHeader( GUIShapesInfo, "GUIShapesInfo", true, true ) ){
			ImGui::Separator();
			
			ImGui::Text( "Loaded scene: %s", scene.getLoadedScene().c_str() );
			
			ImGui::Separator();
			ImGui::Text( "Number of shapes: %u", scene.getNumShapes() );
			
			// todo: list specific shape data here (n vertexShapes, etc)
			if(scene.getNumShapes()>0){
				if (ImGui::TreeNode("All Shapes")){
					static ImGuiTextFilter filter;
					filter.Draw("Filter by name");
					
					static int shapeTypeFilter = 0;
					ImGui::Text("Type: ");
					ImGui::SameLine();
					ImGui::RadioButton("All", &shapeTypeFilter, 0);
					
					auto shapesByType = scene.getAllShapesByType();
					int i = 1;
					for( auto it = shapesByType.begin(); it!=shapesByType.end(); ++it, ++i ){
						ImGui::SameLine();
						ImGui::RadioButton((it->first).c_str(), &shapeTypeFilter, i);
					}
					
					ImGui::Separator();
					
					ImGui::Columns(4);
					static bool firstTime = true;
					if( firstTime ){
						ImGui::SetColumnOffset(0, 00);
						ImGui::SetColumnOffset(1, 50);
						ImGui::SetColumnOffset(2, 230);
						ImGui::SetColumnOffset(3, 330);
						firstTime = false;
					}
					
					//ImGui::SameLine(20);
					ImGui::Text("On"); ImGui::NextColumn();
					ImGui::Text("Name"); ImGui::NextColumn();
					ImGui::Text("Type"); ImGui::NextColumn();
					ImGui::Text("Group"); ImGui::NextColumn();
					
					ImGui::Separator();
					
					for( auto it=scene.getShapesRef().begin(); it!=scene.getShapesRef().end(); ++it ){
						basicShape* s = *it;
						
						if( shapeTypeFilter > 0 ){
							auto shapeIt = shapesByType.begin();
							std::advance( shapeIt, shapeTypeFilter );
							
							if( s->getShapeType() == shapeIt->first ) continue;
						}
						
						if( filter.PassFilter( s->getName().c_str() ) ){
						
							ImVec4 statusColor( !s->isReady()*1.f, s->isReady()*1.f + (!s->isReady() && !s->hasFailed())*.5f, 0.f, 1.f );
							
							// todo: let users enable/disable shapes ?
							ImGui::Checkbox("", &s->isSelected );// Text("-");
							ImGui::NextColumn();// ImGui::SameLine(60);
							//ImGui::Text( "%s", s->getName().c_str() );
							if( ImGui::Selectable(s->getName().c_str(), &s->isSelected ) ){
								//s->isSelected = !s->isSelected;
							}
							ImGui::NextColumn();// ImGui::SameLine(180);
							ImGui::TextColored( statusColor, "%s", (*it)->getShapeType().c_str() );
							ImGui::NextColumn();// ImGui::SameLine(100);
							ImGui::Text( "%i", s->getGroupID() );
							ImGui::NextColumn(); // next line
							
							// TODO: add shape-specific information line ?
						}
					}
					ImGui::Columns(1);
					
					ImGui::TreePop();
				}
					
//				ImGui::Separator();
//				
//				if (ImGui::TreeNode("Shape Groups"))
//				{
//					static bool selected[3] = { false, false, false };
//					ImGui::Selectable("main.c", &selected[0]);    ImGui::SameLine(300); ImGui::Text(" 2,345 bytes");
//					ImGui::Selectable("Hello.cpp", &selected[1]); ImGui::SameLine(300); ImGui::Text("12,345 bytes");
//					ImGui::Selectable("Hello.h", &selected[2]);   ImGui::SameLine(300); ImGui::Text(" 2,345 bytes");
//					ImGui::TreePop();
//				}
			}
		} // end shapes section
		
		ImGui::Spacing();
		if( ImGui::CollapsingHeader( GUIEffectsTitle, "GUIEffectsPanel", true, true ) ){
			
			ImGui::Text( "Loaded configuration: %s", loadedConfiguration.c_str() );
			
			ImGui::Separator();
			ImGui::Text( "Number of effects : %u", (int) effects.size() );
			
			if( ImGui::Button("Add new...") ){
				ImGui::OpenPopup("Add new shape...");
			}
			ImGui::SameLine();
			//ImGui::Text(selected_fish == -1 ? "<None>" : names[selected_fish]);
			if (ImGui::BeginPopup("Add new shape...")){
				ImGui::Separator();
				for(auto it = effect::factory::getEffectRegistry().begin(); it!= effect::factory::getEffectRegistry().end(); ++it){
					if ( ImGui::Selectable( it->first.c_str() )){
						basicEffect* e = effect::create(it->first);
						if( e != nullptr ){
							e->initialise(animationParams.params);
							effects.push_back( e );
						}
					}
				}
				ImGui::EndPopup();
			}
			
			ImGui::Separator();
			ImGui::Separator();
			
			if(getNumEffects()>0){
				if (ImGui::TreeNode("All Effects")){
					//static ImGuiTextFilter filter;
					//filter.Draw("Filter by name");
					
					static int effectTypeFilter = 0;
					ImGui::Text("Type: ");
					ImGui::SameLine();
					ImGui::RadioButton("All", &effectTypeFilter, 0);
					
					auto effectsByType = getAllEffectsByType();
					int i = 1;
					for( auto it = effectsByType.begin(); it!=effectsByType.end(); ++it, ++i ){
						ImGui::SameLine();
						ImGui::RadioButton((it->first).c_str(), &effectTypeFilter, i);
					}
					
					ImGui::Separator();
					
					ImGui::Columns(5);
					static bool firstTime = true;
					if( firstTime ){
						ImGui::SetColumnOffset(0, 00);
						ImGui::SetColumnOffset(1, 50);
						ImGui::SetColumnOffset(2, 220);
						ImGui::SetColumnOffset(3, 300);
						ImGui::SetColumnOffset(3, 350);
						firstTime = false;
					}
					
					//ImGui::SameLine(20);
					ImGui::Text("On"); ImGui::NextColumn();
					ImGui::Text("Name"); ImGui::NextColumn();
					ImGui::Text("Type"); ImGui::NextColumn();
					ImGui::Text("Bound Shapes"); ImGui::NextColumn();
					ImGui::Text("Rm?"); ImGui::NextColumn();
					
					ImGui::Separator();
					
					for( auto it=effects.begin(); it!=effects.end(); ++it ){
						basicEffect* e = *it;
						
						// apply filters
						if( effectTypeFilter > 0 ){
							auto effectIt = effectsByType.begin();
							std::advance( effectIt, effectTypeFilter );
							
							if( e->getType() == effectIt->first ) continue;
						}
						
						ImGui::PushID(e);
						
						ImVec4 statusColor( !e->isReady()*1.f, e->isReady()*1.f + (!e->isReady() && !e->isLoading())*.5f, 0.f, 1.f );
						ImGui::Text("x");
						ImGui::NextColumn();// ImGui::SameLine(60);
						//ImGui::Text( "%s", s->getName().c_str() );
						
						if( ImGui::Selectable(e->getName().c_str(), false)){//&e->isSelected ) ){
							//s->isSelected = !s->isSelected;
							e->toggleGuiWindow();
							// todo: resolve namespace conflicts here
						}
						if (ImGui::IsItemHovered()){
							ImGui::SetTooltip( "%s", e->getShortStatus().c_str() );
						}

						ImGui::NextColumn();// ImGui::SameLine(180);
						ImGui::Text("%s", (*it)->getType().c_str() );
						ImGui::NextColumn();// ImGui::SameLine(100);
					
						ImGui::Text("%i", (*it)->getNumShapes() );
						ImGui::NextColumn();
						
						//ImGui::PushID(e);
						if(ImGui::Button("x")){
							ImGui::OpenPopup(e->getName().insert(0, "rm-").c_str());
						}
						if( ImGui::BeginPopup(e->getName().insert(0, "rm-").c_str()) ){
							ImGui::Text("Remove? ");
							ImGui::SameLine();
							if(ImGui::Button("Cancel")){
								ImGui::CloseCurrentPopup();
							} ImGui::SameLine();
							if( ImGui::Button("Ok") ){
								removeEffect(e);
								it--; // keep valid iterator
							}
							
							ImGui::EndPopup();
						}
						//ImGui::PopID();
						
						ImGui::NextColumn();
						
						ImGui::PopID();
					}
					ImGui::Columns(1);
					
					ImGui::Spacing();
					ImGui::Spacing();
					
					ImGui::TreePop();
				}
			} // end effectsPanel
			
			// show modules GUIs
			for(int i=0; i<modules.size(); i++){
				modules[i]->drawMenu();
			}
			
			if(bGuiShowModules){
				ImGui::Begin( GUIModulesPanel, &bGuiShowModules, ImVec2(400, ofGetHeight()*.8f) );
				
				ImGui::End();
			} // end karma mapper modules gui window
			
			// show console ?
			if( bGuiShowConsole ){
				karmaConsoleChannel::getLogger()->drawImGui (GUIConsolePanel, bGuiShowConsole );
			}
			
			// show effect gui
			for(int i=0; i<effects.size(); i++){
				effects[i]->showGuiWindow( scene );
			}
			
		}  // end bShowGui
		
		// end karma mapper main gui window
		ImGui::End();
		
	}
	
	if(ShowSaveAsModal) ImGui::OpenPopup("Save config as...");
	
	if (ImGui::BeginPopupModal("Save config as...", NULL, ImGuiWindowFlags_AlwaysAutoResize) ){
		static char tmpName[32] = "";
		ImGui::TextWrapped("Please enter a file name.\n\n");
		ImGui::Separator();
		bool doSaveNow = false;
		if( ImGui::InputText("File name", tmpName, 32, ImGuiInputTextFlags_EnterReturnsTrue) ){
			doSaveNow = true;
		}
		
		if( ImGui::Button("Cancel") ){
			memset(&tmpName[0], '\0', sizeof(tmpName));
			ShowSaveAsModal = false; // tmp
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if( ImGui::Button("Save") ){
			doSaveNow = true;
		}
		
		if( doSaveNow && tmpName[0] != '\0'){
			saveConfiguration(tmpName);
			memset(&tmpName[0], '\0', sizeof(tmpName));
			ImGui::CloseCurrentPopup();
			ShowSaveAsModal = false; // tmp
		}
		ImGui::EndPopup();
	}
	
	gui.end();
}

// - - - - - - - -
// MENU EVENT LISTENERS
// - - - - - - - -
void animationController::showShapeLoadMenu(){
	//ofRectangle btn = loadButton.getShape();
	ofFileDialogResult openFileResult= ofSystemLoadDialog("Select the XML file to load...", false, ofToDataPath(KM_SCENE_SAVE_PATH));
	
	if(openFileResult.bSuccess){
		// restrict to saveFiles dir
		ofFile file( ofToDataPath(KM_SCENE_SAVE_PATH)+openFileResult.getName() );
		if(file.exists()){
			stop();
			if( scene.loadScene( openFileResult.getName() )){
				start();
			}
		}
	}
}

void animationController::showSaveDialog(){
	ofFileDialogResult openFileResult= ofSystemSaveDialog(scene.getLoadedScene(), "Where shall I save your scene?");
	
	if(openFileResult.bSuccess){
		// only keep the file name, not the path
		scene.saveScene( openFileResult.getName() );
	}
}

void animationController::setFullScreen(bool _fullScreen){
	bIsFullScreen = _fullScreen;
	ofSetFullscreen( _fullScreen );
}

void animationController::setShowMouse(bool _showMouse){
	bIsFullScreen = _showMouse;
	bShowMouse ? ofShowCursor() : ofHideCursor();
}

void animationController::showAnimationsGui( bool& _b){
	// sync var bGuiShowAnimParams
	animationParams.setShowParams(_b);
}

//

//void animationController::guiEvent(ofxUIEventArgs &e){
//	string name = e.getName();
//	
//	// scan for open dropdowns
//	// (dirty) bypass for issue https://github.com/rezaali/ofxUI/issues/68
//	//ofxUIDropDownList* addShapeDDL = (ofxUIDropDownList*) gui->getWidget("Add Shape");
//	//bool dropdownOpen = addShapeDDL->isOpen();
//	//if( selectConfigFileOpen && name != "Configuration File") return;
//	
//	
//	// interface options
//	// - - - - - - - - -
//	if(name==GUIToggleFullScreen){
//		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
//		ofSetFullscreen(toggle->getValue());
//	}
//	else if(name=="lololol"){
//		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
//		toggle->getValue()?ofHideCursor():ofShowCursor();
//	}
//	
//	#ifdef KARMAMAPPER__DEBUG
//	else {
//		ofLogNotice("animationController::guiEvent") << "Unknown GUI event: " << name << endl;
//	}
//	#endif
//}

void animationController::_keyPressed(ofKeyEventArgs &e){
	
	// ignore keypress if imGui is typing
	if( ImGui::GetIO().WantTextInput ) return;
	
	// check key combinations
	if( ofGetKeyPressed( KM_CTRL_KEY_CODE ) ) {
		
		// ignore case for shortcuts
		int keyToLower = ofToChar( ofToLower( ofToString((char) e.key )));
		
		// toggle gui ?
		if ( 'g' == keyToLower ){
			bShowGui = !bShowGui;
		}
		
		else if ( 'f' == keyToLower ){
			setFullScreen( ! bIsFullScreen );
		}
		
		else if ( 'n' == keyToLower ){
			newConfiguration();
		}
		
		else if ( 'm' == keyToLower ){
			setShowMouse( !bShowMouse );
		}
		
		else if ( 's' == keyToLower ){
			if( loadedConfiguration.empty() || ofGetKeyPressed(OF_KEY_SHIFT) ){
				gui.begin();
				ImGui::OpenPopup("Save config as...");
				gui.end();
			}
			else {
				saveConfiguration( loadedConfiguration );
			}
		}
	}
	
}

void animationController::_mousePressed( ofMouseEventArgs &e ){
	// ignore click if imGui intercepted it
	if( ImGui::GetIO().WantCaptureMouse ) return;
}
