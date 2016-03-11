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

// forward declarations are needed for event listening
ofEvent<karmaControllerDrawEventArgs> animationController::karmaControllerBeforeDraw;//(animationParams& _p);
ofEvent<karmaControllerDrawEventArgs> animationController::karmaControllerAfterDraw;

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
	bGuiShowMainWindow = true;
	
	ofAddListener( ofEvents().draw , this, &animationController::draw, OF_EVENT_ORDER_APP );
	ofAddListener( ofEvents().update , this, &animationController::update, OF_EVENT_ORDER_APP );
	
	ofAddListener( ofEvents().keyPressed, this, &animationController::_keyPressed, OF_EVENT_ORDER_APP );
	
	newConfiguration();
	
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

	unloadAllLayers();
	unloadAllModules();
	
}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
bool animationController::start(){
	
	// all black! xD
	ofSetBackgroundAuto(false);
	ofClear(0,0,0,255);
	
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
	//oscRouter.enable();
	
	// enable mirOSCReceiver
	//mirReceiver* mirOSCReceiver = new mirReceiver();
	//mirOSCReceiver->enable(true);
	//mirOSCReceiver->start();
	//OSCRouter::getInstance().clearAllNodes();
	//OSCRouter::getInstance().addNode( mirOSCReceiver );
	
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
	//mirOSCReceiver.stop();
	//oscRouter.removeNode( &mirOSCReceiver );
	
	// disable durationOSCReceiver
	//durationOSCReceiver.stop();
	//oscRouter.removeNode( &durationOSCReceiver );
	
	//oscRouter.setEnabled(false);
	
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
	
	unloadAllLayers();
	unloadAllModules();
	
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
	
	for(auto layer=layers.begin(); layer!=layers.end(); ++layer){
		
		auto it=std::find(layer->second.begin(), layer->second.end(), _e);
		if( it != layer->second.end() ){
			_e->disable();
			layer->second.erase( it );
			delete _e;
			break;
		}
	}
	
	return true;
}

bool animationController::removeLayer(  karmaFboLayer& _layer  ){
	//if( _layer == nullptr) return false;
	
	for(auto layer=layers.rbegin(); layer!=layers.rend(); ++layer){
		
		if( &layer->first == &_layer ){
			// destroy effects
			if(layer->second.size()>0){
				for(auto e=layer->second.rbegin(); e != layer->second.rend(); ++e){
					basicEffect* tmpE (*e);
					tmpE->disable();
					layer->second.erase( std::next(e).base() );
					delete tmpE;
					break;
				}
			}
			// rm layer entry
			layers.erase( std::next(layer).base() );
			break;
		}
		
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
	
	if( configXML.loadFile( _file ) ){
		
		stop();
		
		// load shapes scene
		if( configXML.pushTag("sceneSettings") ){
			if( scene.loadScene( configXML.getValue("shapesFile", "")) ){
				loadedConfiguration = _file;
				start();
				success = true;
			}
			else {
				newConfiguration();
				start(); // start anyways
				// todo: make this a GUI warning modal ?
				ofLogWarning("animationController::loadConfiguration") << "The config file is loading but it's associated shapes scene file was not found. ( continuing... )";
			}
			configXML.popTag(); // pop sceneSettings
		}
		else {
			newConfiguration();
			start();
			ofLogNotice("animationController::loadConfiguration") << "The config file has no shapes scene file. Loading effects without shapes.";
		}
		
		if( configXML.pushTag("guiSettings") ){
			bShowGui = configXML.getValue("bShowGui", bShowGui );
			bGuiShowMainWindow = configXML.getValue("bGuiShowMainWindow", bGuiShowMainWindow );
			bGuiShowAnimParams = configXML.getValue("bGuiShowAnimParams", bGuiShowAnimParams );
			bGuiShowPlugins = configXML.getValue("bGuiShowPlugins", bGuiShowPlugins );
			bGuiShowModules = configXML.getValue("bGuiShowModules", bGuiShowModules );
			bGuiShowConsole = configXML.getValue("bGuiShowConsole", bGuiShowConsole );
			configXML.popTag();
		}
		
		// fix (import (old)savefiles which don't have layers
		bool bypass = false;
		if (configXML.tagExists("effects")){
			//configXML.doc.
			//configXML.addTag("layers");
			// push directly into effect
			//if(configXML.pushTag("effects")){
			ofLogWarning("animationController::loadConfiguration") << "LOADING OLD SAVE FILE WITHOUT LAYERS... (should automatically convert by re-saving it once loaded)";
			bypass=true;
			//}
		}
		
		// load effects
		if(	bypass || configXML.pushTag("layers") ){
			layers.clear(); // should be done in resetLayers()
			int numLayers = configXML.getNumTags("layer");
			
			if(bypass || numLayers==0){
				numLayers = 1;
			}
			
			// load layers from save file
			for(int l=0; l<numLayers; ++l){
				
				if( bypass || configXML.pushTag("layer",l) ){
					
					layers.emplace_back(
						karmaFboLayer(ofGetWidth(),ofGetHeight()),
						list<basicEffect*>()
					);
					
					karmaFboLayer& curFbo = layers.back().first;
					list<basicEffect*>& curLayerEffects = layers.back().second;
					
					// setup karmaFboLayer
					curLayerEffects.clear();
					if(bypass){
						ofLogVerbose("animationController::loadConfiguration") << "Created initial layer";
						curFbo.set("Initial Layer", 0);
					}
					else {
						curFbo.set( configXML.getValue("layerName", "Layer "+ofToString(l)), l);
					}
					
					// setup it's effects
					vector<int> failedEffects;
					failedEffects.clear();
					vector<int> effectIndexes;
					effectIndexes.clear();
					int numEffects = 0;
					
					// push into effects
					if( configXML.pushTag("effects") ){
						
						// loop for each effect
						numEffects = configXML.getNumTags("effect");
						for(int e=0; e<numEffects; e++){
							if( configXML.pushTag("effect", e) ){
								
								// Some code comes from:
								// --> http://stackoverflow.com/questions/8269465/how-can-i-instantiate-an-object-knowing-only-its-name
								string effectType = configXML.getValue("effectType", "basicEffect");
								basicEffect* effect = effect::create(effectType);
								if( effect != nullptr ){
									curLayerEffects.push_back( effect );
									effect->initialise(animationParams.params);
									effect->loadFromXML( configXML );
									
									// check index
									int tmpIndex = effect->getIndex();
									if(std::find(effectIndexes.begin(), effectIndexes.end(), tmpIndex) == effectIndexes.end() ){
										// index is OK to have
										
									}
									else {
										while( std::find(effectIndexes.begin(), effectIndexes.end(), tmpIndex) != effectIndexes.end() ){
											tmpIndex++;
										}
										effect->setIndex(tmpIndex);
									}
									effectIndexes.push_back(tmpIndex);
									
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
													if( !curLayerEffects.back()->bindWithShape(tmpShape) ){
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
											
											// todo: (important) adapt this structure in the basicShape save process
										}
										
										// todo: make this GUI message and show details
										if(failedShapes.size() > 0) ofLogWarning("animationController::loadConfiguration") << " Effect '" << effectType << "' loaded but failed to bind with " << failedShapes.size() << " out of " << numShapes << " shapes... (ignoring, but re-saving the configuration will erase this information).";
										
										configXML.popTag(); // pop bound shapes
									}
									
									
								}
								else {
									failedEffects.push_back(e);
									// unknow effect type
									ofLogError("effect::create") << "Effect type not found: " << effectType;
								}
								
								configXML.popTag(); // pop effect
							}
							else {
								
								ofLogWarning("animationController::loadConfiguration") << "Weird situation... Can't push into effect #" << e;
							}
						} // end for(effects)
						ofLogVerbose("animationController::loadConfiguration") << "Successfully loaded " <<  curLayerEffects.size() << "effects on layer " << l;
						
						configXML.popTag(); // pop effects
					}
					else {
						if(bypass){
							ofLogError("animationController::loadConfiguration") << "Couldn't import OLD CONFIG FILE, sorry...";
						}
						else {
							ofLogNotice("animationController::loadConfiguration") << "No effects in layer #"<< l << ", skipping effects on this layer!";
						}
					}
					
					if (failedEffects.size()>0) {
						success = false;
						ofLogError("animationController::loadConfiguration") << "Failed loading " << failedEffects.size()<< " effects on layer #" << l;
					}
					
					ofLogVerbose("animationController::loadConfiguration") << "Loaded layer #"<< l << "/"<< numLayers <<" ("<< curFbo.getName() << ") from file [" << _file << "] with [" << numEffects << " effects] ( " << failedEffects.size() << "failed )";
					
					configXML.popTag(); // pop effects
					
					ofLogVerbose("animationController::loadConfiguration") << "Created layer "<< l <<" ("<< curFbo.getName() << ")";

//					else {
//						ofLogError("animationController::loadConfiguration") << "Layer " << l << " couldn't be loaded! Continueing with other layers...";
//					}
					
					// pop layer
					if(!bypass) configXML.popTag();
				}
				else {
					ofLogWarning("animationController::loadConfiguration") << "Can't load layer #" << l;
				}
				
				
				
			} // end for(layer)
			
			// pop "layers"
			if(!bypass) configXML.popTag();
		}
		else {
			ofLogError("animationController::loadConfiguration") << "Weird situation... Can't push into layers section.";
		}
			
		// load modules
		if(	configXML.pushTag("modules") ){
				
			// loop for each effect
			int numModules = configXML.getNumTags("module");
			vector<int> failedModules;
			failedModules.clear();
			
			for(int m=0; m<numModules; m++){
				if( configXML.pushTag("module", m) ){
					
					// Some code comes from:
					// --> http://stackoverflow.com/questions/8269465/how-can-i-instantiate-an-object-knowing-only-its-name
					string moduleType = configXML.getValue("moduleType", "karmaModule");
					
					//
					karmaModule* newModule = tryLoadModule(moduleType);
					if ( newModule != nullptr) {
						// (do nothing )
						if(! newModule->loadFromXML( configXML ) ){
							ofLogError("animationController::loadConfiguration");
							failedModules.push_back(m);
						}
						else {
							newModule->enable();
						}
					}
					else {
						failedModules.push_back(m);
						// unknow effect type
						ofLogError("module::create") << "Module type not found, or error while instantiating it: " << moduleType;
					}
					
					configXML.popTag(); // pop module
				}
			ofLogNotice("animationController::loadConfiguration") << "Loaded modules from " << _file << " [" << numModules << " modules] ( " << failedModules.size() << "failed )";
			}
			
			if (failedModules.size()>0) {
				success = false;
			}
			configXML.popTag(); // pop modules
		}
		else {
			ofLogNotice("animationController::loadConfiguration") << "No modules in file, skipping modules!";
		}
		
		if( !success ){
			ofLogError("animationController::loadConfiguration") << "There were errors loading the configuration file...";
		}
		else {
			ofLogNotice("animationController::loadConfiguration") << "The config file was loaded !";
		}
	}
	else {
		// todo: make this a GUI message
		ofLogError("animationController::loadConfiguration") << "404 - The config file `"<< _file << "` does not exist or is not readable.";
		success = false;
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
		success = loadConfiguration( prevSettings.getValue("lastLoadedConfiguration", ofToDataPath( KM_CONFIG_FOLDER ) + KM_CONFIG_DEFAULT ) );
		
		prevSettings.clear();
	}
	else {
		ofLogNotice("animationController::")<< __FUNCTION__ << "Previous configuration file not found. Not loading any scene.";
	}
	
	return success;
}

bool animationController::saveConfiguration( const string& _filePath ){
	bool success = false;
	
	string fullPath;
	// no save file ?
	if( _filePath.empty() ){
		if( loadedConfiguration.empty() ){
			ofSystemAlertDialog("Please provide a name for the save file!");
			return false;
		}
		else fullPath = loadedConfiguration;
	}
	else fullPath = ofToDataPath(_filePath);
	
	ofxXmlSettings sceneXML;
	
	sceneXML.addTag("sceneSettings");
	if( sceneXML.pushTag("sceneSettings") ){
		sceneXML.setValue("shapesFile", scene.getLoadedScene() );
		sceneXML.popTag();
	}
	
	sceneXML.addTag("guiSettings");
	if( sceneXML.pushTag("guiSettings") ){
		sceneXML.setValue("bShowGui", bShowGui );
		sceneXML.setValue("bGuiShowMainWindow", bGuiShowMainWindow );
		sceneXML.setValue("bGuiShowAnimParams", bGuiShowAnimParams );
		sceneXML.setValue("bGuiShowPlugins", bGuiShowPlugins );
		sceneXML.setValue("bGuiShowModules", bGuiShowModules );
		sceneXML.setValue("bGuiShowConsole", bGuiShowConsole );
		sceneXML.popTag();
	}
	
	// save all effect & layer data
	sceneXML.addTag("layers");
	vector<int> failedLayers;
	vector<basicEffect*> totalFailedEffects;
	if(sceneXML.pushTag("layers")){
		int l=0;
		failedLayers.clear();
		for(auto layer = layers.begin(); layer!=layers.end(); ++layer, ++l){
			
			karmaFboLayer& layerFbo = layer->first;
			list<basicEffect*>& layerEffects = layer->second;
			
			sceneXML.addTag("layer");
			if( sceneXML.pushTag("layer",l) ){
				
				// layer settings
				sceneXML.addValue("layerName", layerFbo.getName());
				sceneXML.addValue("layerIndex", layerFbo.getIndex());
				
				// layer effects
				sceneXML.addTag("effects");
				vector<basicEffect*> failedEffects;
				if (sceneXML.pushTag("effects")) {
					
					int e=0;
					failedEffects.clear();
					for(auto it = layerEffects.begin(); it != layerEffects.end(); it++, e++){
						
						sceneXML.addTag("effect");
						if( sceneXML.pushTag("effect", e) ){
							
							if(!(*it)->saveToXML(sceneXML)) failedEffects.push_back(*it);
							
							sceneXML.popTag(); //pop effect
						}
						else {
							ofLogError("animationController::saveConfiguration()") << "Layer " << l << " couldn't be saved due to an ofxXmlSettings error.";
							failedEffects.push_back(*it);
						}
					}
					sceneXML.popTag(); // pop effects
				}
				else {
					ofLogError("animationController::saveConfiguration()") << "Effects " << l << " couldn't be saved due to an ofxXmlSettings error.";
					failedLayers.push_back(l);
				}
				
				totalFailedEffects.insert(totalFailedEffects.end(), failedEffects.begin(), failedEffects.end());
				
				sceneXML.popTag(); //pop layer
			}
			else {
				ofLogError("animationController::saveConfiguration()") << "Layer " << l << " couldn't be created due to an ofxXmlSettings error.";
				failedLayers.push_back(l);
			}
		}
	}
	
	// save all modules data
	sceneXML.addTag("modules");
	vector<karmaModule*> failedModules;
	if (sceneXML.pushTag("modules")) {
	
		int m=0;
		failedModules.clear();
		for(auto it = modules.begin(); it != modules.end(); it++, m++){
			
			sceneXML.addTag("module");
			sceneXML.pushTag("module", m);
			
			if(!(*it)->saveToXML(sceneXML)) failedModules.push_back(*it);
			
			
			sceneXML.popTag(); //pop module
		}
		
		sceneXML.popTag(); // pop modules
	}
	
	// write down settings to disk
	if( sceneXML.saveFile(fullPath) ){
		//loadedConfiguration = ofFilePath::getFileName(fullPath);
		loadedConfiguration = fullPath;
		success = true;
		if(totalFailedEffects.size() == 0 && failedModules.size() == 0 && failedLayers.size() == 0 ){
			ofLogNotice("animationController::saveConfiguration()", "Saved current configuration to `"+fullPath+"`");
		}
		else{
			ofSystemAlertDialog("The configuration has been saved but "+ ofToString(totalFailedEffects.size()) +" effects and "+ ofToString(failedModules.size()) +" modules and "+ofToString(failedLayers.size())+" layers failed to save.");
		}
	}
	else ofSystemAlertDialog("Could not save the current configuration... :(\nSave File: "+fullPath);
	
	return success;
}

// keeps them loaded but ensures they're unbound from any shape
bool animationController::unbindAllShapes(){
	bool success = true;
	for(auto layer = layers.begin(); layer!=layers.end(); ++layer){
		list<basicEffect*>& layerEffects = layer->second;
		for( auto it = layerEffects.begin(); it != layerEffects.end(); ++it){
			success *= (*it)->detachFromAllShapes();
		}
	}
	return true;
}

bool animationController::unloadAllEffects(){
	
	for(auto layer = layers.begin(); layer!=layers.end(); ++layer){
		list<basicEffect*>& layerEffects = layer->second;
		for( auto it = layerEffects.rbegin(); it != layerEffects.rend(); ++it){
			delete (*it); // todo: does this call destructor ?
			layerEffects.erase( std::next(it).base() );
		}
		layerEffects.clear();
	}
	
	return true;
}

bool animationController::unloadAllLayers(){
	
	if(layers.size()>0){
		for(auto layer = layers.rbegin(); layer!=layers.rend(); ++layer){
			list<basicEffect*>& layerEffects = layer->second;
			
			for( auto it = layerEffects.rbegin(); it != layerEffects.rend(); ++it){
				delete (*it); // todo: does this call destructor ?
				layerEffects.erase( std::next(it).base() );
			}
			layerEffects.clear();
			
			layers.erase( std::next(layer).base() );
			//delete ( fbo );
			if(layers.size()==0) break;
		}
	}
	layers.clear();
	
	return true;
	
}

bool animationController::unloadAllModules(){
	
	for( auto it = modules.rbegin(); it != modules.rend(); ++it){
		//delete (*it); // todo: does this call destructor ?
		modules.erase( std::next(it).base() );
	}
	
	modules.resize(0);
	
	return true;
	
}

// todo: rename this to reset() ?
void animationController::newConfiguration(){
	unloadAllLayers();
	unbindAllShapes();
	scene.unloadShapes();
	
	// create first FBO
	layers.emplace_back(
		karmaFboLayer(ofGetWidth(),ofGetHeight()),
		list<basicEffect*>()
	);
		
	// setup karmaFboLayer
	layers.back().first.set("Initial Layer", 0);
		
	// setup it's effects
	layers.back().second.clear();
}

karmaModule* animationController::tryLoadModule(const string &_moduleType){
	
	if(_moduleType.empty()){
		ofLogVerbose();
		return nullptr;
	}
	
	{
		// only allow 1 instance of each type
		karmaModule* existing = getModule(_moduleType);
		if ( existing != nullptr) {
			ofLogNotice("animationController::tryLoadModule()") << "Module already loaded: "<< _moduleType << " (not reloading it)";
			return existing;
		}
	}
	
	// check for dependencies
	auto deps = module::getModuleDependencies(_moduleType);
	if(int numDeps = deps.size()>0){
		ofLogVerbose("animationController::tryLoadModule") << "Loading additional modules for module: " << _moduleType << " (" << numDeps <<" dependencies).";
		for(auto dep=deps.begin(); dep!=deps.end(); ++dep){
			if( tryLoadModule(*dep) == nullptr ){
				ofLogError("animationController::tryLoadModule()") << "Dependency " << *dep << " for module " << _moduleType << " not found. (Not loading "<< *dep << ")";
				return nullptr;
			}
		}
		ofLogVerbose("animationController::tryLoadModule()") << "Successfully loaded "<< numDeps << " additional modules for module: " << _moduleType << ".";
	}
	else {
		// no dependencies entry.
		// Should we handle this ?
		ofLogNotice("animationController::tryLoadModule(" +_moduleType + ")") << "No dependencies found. Please make sure your effect is correctly implemented to prevent this message.";
	}
	
	// spawn module
	karmaModule* module = module::create(_moduleType);
	if( module != nullptr ){
		module->enable();
		modules.push_back( module );
		return module;
	}
	else {
		// unknow effect type
		ofLogError("animationController::tryLoadModule") << "Module type not found: " << _moduleType;
		
		return nullptr;
	}
}

// todo: add an alternative const method for this ? (or implement moduleExists() )
karmaModule* animationController::getModule(const string &_moduleType){
	if (_moduleType.empty()) {
		return nullptr;
	}
	
	for (auto m=modules.begin(); m!=modules.end(); ++m) {
		if ( _moduleType.compare((*m)->getType()) == 0 ){
			return *m;
		}
	}
	
	return nullptr;
}

// - - - - - - - -
// GETTERS // UTILITIES
// - - - - - - - -
// todo: ret variable could be cached
unsigned int animationController::getNumEffects() const {
	unsigned int ret = 0;
	for(auto layer = layers.cbegin(); layer!=layers.cend(); ++layer){
		ret += layer->second.size();
	}
	return ret;
}

vector<basicEffect*> animationController::getEffectsOfType(string _type){
	vector<basicEffect*> ret;
	ret.clear();
	ret.resize(0);
	
	for(auto layer = layers.begin(); layer!=layers.end(); ++layer){
		list<basicEffect*>& layerEffects = layer->second;
		// loop trough effects and return the wanted ones
		for(auto it = layerEffects.begin(); it != layerEffects.end(); it++){
			if((*it)->getType() == _type ) ret.push_back((*it));
		}
	}
	
	return ret;
}

map<string, vector<basicEffect*> > animationController::getAllEffectsByType() const {
	map<string, vector<basicEffect*> > ret;
	//layers.
	for(auto layer = layers.begin(); layer!=layers.end(); ++layer){
		auto & layerEffects = layer->second;
		//list<basicEffect*> * layerEffects = &(layer->second);
		
		for( auto it=layerEffects.begin(); it!=layerEffects.end(); ++it ){
			if( ret.find( (*it)->getType() ) == ret.end() ){
				ret[ (*it)->getType() ] = vector<basicEffect*>();
			}
			ret[ (*it)->getType() ].push_back(*it);
		}
	}
	return ret;
}

unsigned int animationController::getNumLayers() const {
	return layers.size();
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
	
	// update effects (run mode)
	for(auto layer = layers.begin(); layer!=layers.end(); ++layer){
		list<basicEffect*>& layerEffects = layer->second;
		
		for(auto e=layerEffects.begin(); e!=layerEffects.end(); ++e){
			(*e)->update(animationParams.params);
		}
	}
	
	// update modules
	for(auto m=modules.begin(); m!=modules.end(); ++m){
		(*m)->update( animationParams.params );
	}
}

void animationController::draw(ofEventArgs& event){
	if(!isEnabled()) return;
	//karmaControllerDrawEventArgs tmp;
	static karmaControllerDrawEventArgs drawEventArgs(animationParams.params);
	drawEventArgs.params = animationParams.params;
	drawEventArgs.stage = DRAW_EVENT_BEFORE_DRAW;
	ofNotifyEvent(animationController::karmaControllerBeforeDraw, drawEventArgs, this);
	
	// tmp
	ofClear(0,1);
	//ofBackground(255,0,0);
	
	// draw modules
	for(auto m=modules.begin(); m!=modules.end(); ++m){
		(*m)->draw(animationParams.params);
	}
	
	// render a scene without effects (tmp?)
	if(layers.size()==0){
		ofSetColor( ofFloatColor(1.f, 1));//params.seasons.summer));
		ofFill();
//		for(auto it=scene.getShapesRef().begin(); it!=scene.getShapesRef().end(); ++it){
//			(*it)->sendToGPU();
//		}
	}
	
	else for(auto layer = layers.begin(); layer!=layers.end(); ++layer){
		list<basicEffect*>& layerEffects = layer->second;
	
		// draw effects
		for(auto e=layerEffects.begin(); e!=layerEffects.end(); ++e){
			(*e)->render(animationParams.params);
		}
	}
	
	// notify end draw (before GUI)
	drawEventArgs.params = animationParams.params;
	drawEventArgs.stage = DRAW_EVENT_AFTER_DRAW;
	ofNotifyEvent(animationController::karmaControllerAfterDraw, drawEventArgs, this);
	
	//recorder.endFrame(true);
	
	
	// draw gui stuff
	ofPushStyle();
	//ofNoFill();
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
						if( ImGui::MenuItem( dir.getName(i).c_str(), "", (bool)(scene.getLoadedScene() == dir.getPath(i)) ) ){
							
							// todo: add error feedback here
							loadConfiguration(dir.getPath(i));

						}
					}
				}
				ImGui::EndMenu();
			}
			
			if ( !loadedConfiguration.empty() && ImGui::MenuItem("Save configuration", ofToString(KM_CTRL_KEY_NAME).append( " + S" ).c_str() )){
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
					ImGui::Text("No files in %s", ofToDataPath( KM_SCENE_SAVE_PATH).c_str());
					ImGui::Separator();
				} else {
					for (int i = 0; i < dir.size(); i++){
						if( ImGui::MenuItem( dir.getName(i).c_str(), "", (bool)(scene.getLoadedScene() == dir.getPath(i)) ) ){
							unbindAllShapes();
							if( scene.loadScene( ofToDataPath(dir.getPath(i)) ) ){
								start();
							}
							else {
								newConfiguration();
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
			
			ImGui::MenuItem("Show Main Window", NULL, &bGuiShowMainWindow);
			
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
		
		if (bGuiShowMainWindow){
			
			ImGui::Begin("karmaMapper", &bGuiShowMainWindow, ImVec2(400, ofGetHeight()*.8f) );
			
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
								
								if( !s->isType( shapeIt->first ) ) continue;
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
			
			ImGui::Separator();
			if( ImGui::CollapsingHeader( GUIEffectsTitle, "GUIEffectsPanel", true, true ) ){
				
				ImGui::Text( "Loaded configuration: %s", loadedConfiguration.c_str() );
				
				ImGui::Separator();
				ImGui::Text( "Number of layers : %u", getNumLayers() );
				ImGui::Text( "Number of effects : %u", getNumEffects() );
				
				// new layer button
				ImGui::Separator();
				if( ImGui::Button("Add new layer") ){
				
					layers.emplace_back(
						karmaFboLayer(ofGetWidth(),ofGetHeight()),
						list<basicEffect*>()
					);
					
					// setup karmaFboLayer
					layers.back().second.clear();
					layers.back().first.set("Initial Layer", layers.size()-1);
				}
				
				static int effectTypeFilter = 0;
				ImGui::Text("Filter: ");
				ImGui::SameLine();
				ImGui::RadioButton("All", &effectTypeFilter, 0);
				
				auto effectsByType = getAllEffectsByType();
				int i = 1;
				for( auto it = effectsByType.begin(); it!=effectsByType.end(); ++it, ++i ){
					if((i+1)%3!=0) ImGui::SameLine();
					ImGui::RadioButton((it->first).c_str(), &effectTypeFilter, i);
				}
				
				// loop trough layers
				for(auto layer = layers.begin(); layer!=layers.end(); ++layer){
					list<basicEffect*>& layerEffects = layer->second;
					karmaFboLayer& fboLayer = layer->first;
					
					ImGui::SetNextTreeNodeOpened(true, ImGuiSetCond_Once );
					if (ImGui::TreeNode( (ofToString(fboLayer.getIndex())+" - "+fboLayer.getName()+"###layer"+ofToString(&fboLayer)).c_str() )){
						
						// layer information
						static char tmpName[32] = "";
						if(ImGui::Button("Change name...")){
							ImGui::OpenPopup("layerNamePopup");
							std::strcpy (tmpName, fboLayer.getName().c_str());
						}
						if(ImGui::BeginPopup("layerNamePopup")){
							bool doSave = false;
							if( ImGui::InputText("Layer Name", tmpName, KM_ARRAY_SIZE(tmpName), ImGuiInputTextFlags_EnterReturnsTrue) ){
								doSave = true;
							}
							ImGui::SameLine();
							if(ImGui::Button("Set###setLayerName")){
								doSave=true;
							}
							if(doSave){
								fboLayer.set(tmpName, fboLayer.getIndex());
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
						else {
							ImGui::SameLine();
							ImGui::Spacing();
							ImGui::SameLine();
							if(ImGui::Button("^")){ // up
								// skip if first layer
								if(layer!=layers.begin()){
									auto prevIt = std::prev(layer);
									int prevLayerCurIndex = prevIt->first.getIndex();
									prevIt->first.setIndex( fboLayer.getIndex() );
									fboLayer.setIndex(prevLayerCurIndex);
								}
								
								//layers.sort(karmaFboLayer::orderByIndex);
							}
							ImGui::SameLine();
							if(ImGui::Button("v")){ // down
								// move to next layer ?
								if( layer != std::prev(layers.end()) ) {
									auto nextIt = std::next(layer);
									int nextLayerCurIndex = nextIt->first.getIndex();
									nextIt->first.setIndex(fboLayer.getIndex());
									fboLayer.setIndex(nextLayerCurIndex);
								}
								
								//layers.sort(karmaFboLayer::orderByIndex);
							}
							ImGui::SameLine();
							ImGui::Spacing();
							ImGui::SameLine();
							if(ImGui::Button("Remove")){
								ImGui::OpenPopup("rm-layer");
							}
							if( ImGui::BeginPopup("rm-layer")){
								ImGui::Text("Remove? ");
								ImGui::SameLine();
								if(ImGui::Button("Cancel")){
									ImGui::CloseCurrentPopup();
								} ImGui::SameLine();
								if( ImGui::Button("Ok") ){
									auto erased = layer;
									layer--; // keep valid iterator
									//erased->second.
									//layers.erase(erased);
									removeLayer(erased->first);
									
									ImGui::CloseCurrentPopup();
									ImGui::EndPopup();
									ImGui::TreePop();
									continue; // skip 1 frame because we're not reverse-iterating
								}
								ImGui::EndPopup();
							}
						}
						
						ImGui::TextWrapped("Effects on layer: %lu", layerEffects.size());
						
						// add new effect on layer stuff
						{
							if( ImGui::Button("Add new effect...") ){
								ImGui::OpenPopup("Add new effect...");
							}
							ImGui::SameLine();
							//ImGui::Text(selected_fish == -1 ? "<None>" : names[selected_fish]);
							if (ImGui::BeginPopup("Add new effect...")){
								ImGui::Separator();
								for(auto it = effect::factory::getEffectRegistry().begin(); it!= effect::factory::getEffectRegistry().end(); ++it){
									if ( ImGui::Selectable( it->first.c_str() )){
										basicEffect* e = effect::create(it->first);
										if( e != nullptr ){
											e->initialise(animationParams.params);
											layerEffects.push_back( e );
										}
									}
								}
								ImGui::EndPopup(); // end add new effect
							}
							
							ImGui::Separator();
							ImGui::Separator();
						}
						
						// show all effects
						if(layerEffects.size()>0){
							//ImGui::SetNextTreeNodeOpened(true, ImGuiSetCond_Once );
							//static ImGuiTextFilter filter;
							//filter.Draw("Filter by name");
							
							
							ImGui::Separator();
							
							ImGui::Columns(6);
							static bool firstTime = true;
							if( firstTime ){
								ImGui::SetColumnOffset(0, 00);
								ImGui::SetColumnOffset(1, 50);
								ImGui::SetColumnOffset(2, 220);
								ImGui::SetColumnOffset(3, 300);
								ImGui::SetColumnOffset(4, 350);
								ImGui::SetColumnOffset(5, 400);
								firstTime = false;
							}
							
							//ImGui::SameLine(20);
							ImGui::Text("On"); ImGui::NextColumn();
							ImGui::Text("Name"); ImGui::NextColumn();
							ImGui::Text("Type"); ImGui::NextColumn();
							ImGui::Text("Shapes"); ImGui::NextColumn();
							ImGui::Text("Order"); ImGui::NextColumn();
							ImGui::Text("Rm?"); ImGui::NextColumn();
							
							ImGui::Separator();
							
							for( auto it=layerEffects.begin(); it!=layerEffects.end(); ++it ){
								basicEffect* e = *it;
								
								// apply filters
								if( effectTypeFilter > 0 ){
									auto effectIt = effectsByType.begin();
									std::advance( effectIt, effectTypeFilter-1 );
									
									if( !e->isType( effectIt->first) ) continue;
								}
								
								ImGui::PushID(e);
								
								ImVec4 statusColor( !e->isReady()*1.f, e->isReady()*1.f + (!e->isReady() && !e->isLoading())*.5f, 0.f, 1.f );
								ImGui::TextColored(statusColor, e->isReady()?"*":"-");
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
								
								if(ImGui::Button("^")){ // up
									// move to next layer ?
									if( layerEffects.size()<=1 || it ==(layerEffects.begin())){
										// skip if first layer
										if(layer!=layers.begin()){
											std::prev(layer)->second.push_back(e);
											
											auto tmpIt = std::prev(it);
											layer->second.erase(it);
											it = tmpIt;
											
											basicEffect* eff = *it;
											//continue;
										}
									}
									
									else {
										if(it!=layerEffects.begin()){
											auto prevIt = std::prev(it);
											int prevEffectCurIndex = (*prevIt)->getIndex();
											(*prevIt)->setIndex(e->getIndex());
											e->setIndex(prevEffectCurIndex);
											layerEffects.sort(basicEffect::orderByIndex);
										}
									}
								}
								ImGui::SameLine();
								if(ImGui::Button("v")){ // down
									// move to next layer ?
									if( layerEffects.size()<=1 || it == (std::prev(layerEffects.end()))){
										// skip if first layer
										if(layer!=std::prev(layers.end())){
											std::next(layer)->second.push_front(e);
											
											auto tmpIt = std::prev(it);
											
											layer->second.erase(it);
											it = tmpIt;
											
											basicEffect* eff = *it;
											//continue;
										}
									}
									else {
										auto nextIt = std::next(it);
										int nextEffectCurIndex = (*nextIt)->getIndex();
										(*nextIt)->setIndex(e->getIndex());
										e->setIndex(nextEffectCurIndex);
										layerEffects.sort(basicEffect::orderByIndex);
									}
								}
								
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
										it--; // keep valid iterator
										removeEffect(e);
										
										//break; // skip 1 frame because we're not reverse-iterating
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
							
							//ImGui::TreePop();
						} // end layer's effects
						
						ImGui::TreePop();
					} // end fboLayer tree node
					
				} // end for(layers as layer)
				
				

			}
			
			// end karma mapper main gui window
			ImGui::End();
			
		} // end bGuiShowMainWindow
		
		if(bGuiShowModules){
			ImGui::Begin( GUIModulesPanel, &bGuiShowModules, ImVec2(400, ofGetHeight()*.8f) );
			
			if (ImGui::CollapsingHeader( GUIModuleInfoTitle, "GUIModuleInfoTitle", true, true)){
				ImGui::TextWrapped("Here's some information about currently running modules.");
				ImGui::Text("Loaded modules: %lu", modules.size() );
			}
			
			if (ImGui::CollapsingHeader( GUIModulesList, "GUIModulesList", true, true)){
				
				if( ImGui::Button("Add new...") ){
					ImGui::OpenPopup("Add new module...");
				}
				ImGui::SameLine();
				//ImGui::Text(selected_fish == -1 ? "<None>" : names[selected_fish]);
				if (ImGui::BeginPopup("Add new module...")){
					ImGui::Separator();
					for(auto it = module::factory::getModuleRegistry().begin(); it!= module::factory::getModuleRegistry().end(); ++it){
						if ( ImGui::Selectable( it->first.c_str() )){
							karmaModule* m = tryLoadModule(it->first);
							if( m != nullptr ){
								//m->initialise(animationParams.params);
							}
							else {
								// todo: make this a gui message ?
								ofLogError("animationController:: ofx ImGui interaction") << "Could not create module. Maybe it already exists, or is not implemented in your configuration.";
							}
						}
					}
					ImGui::EndPopup();
				} // end popup
				
				// show loaded modules
				ImGui::Separator();
				for(auto m=modules.cbegin(); m!=modules.cend(); ++m){
					ImGui::PushID(ofToString(*m).c_str());
					
					if (ImGui::Button("x")) {
						//auto it = sdt::find(modules.begin(), modules.end(), *m);
						//if(
						modules.erase(m);
						--m;
						ImGui::PopID();
						continue;
					}
					
					ImGui::SameLine();
					ImGui::Spacing();
					ImGui::SameLine();
					
					if (ImGui::Checkbox("On", &(*m)->bEnabled)) {
						(*m)->setEnabled((*m)->bEnabled);
					}
					ImGui::SameLine();
					
					if (ImGui::CollapsingHeader( (*m)->getName().c_str(), ofToString(*m).c_str(), true, true)){
						
						ImGui::Indent();ImGui::Indent();
						(*m)->drawMenuEntry();
						ImGui::Separator();
						ImGui::Unindent();ImGui::Unindent();
						
					}
					
					
					ImGui::PopID();
					ImGui::Separator();
					ImGui::Separator();
					
				} // end modules iteration
				
				ImGui::Separator();
			} // end collapsingHeader
			
			ImGui::End();
		} // end karma mapper modules gui window
		
		// show console ?
		if( bGuiShowConsole ){
			karmaConsoleChannel::getLogger()->drawImGui (GUIConsolePanel, bGuiShowConsole );
		}
		
		// show effects gui
		for(auto layer = layers.begin(); layer!=layers.end(); ++layer){
			list<basicEffect*>& layerEffects = layer->second;
			for(auto e=layerEffects.begin(); e!=layerEffects.end(); ++e){
				(*e)->showGuiWindow( scene );
			}
		}
		
		// show modules gui
		for(auto m=modules.begin(); m!=modules.end(); ++m){
			(*m)->showGuiWindow( );
		}
		
	} // end bShowGui
	
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
			saveConfiguration( ofToString(KM_CONFIG_FOLDER) + tmpName + ".xml");
			memset(&tmpName[0], '\0', sizeof(tmpName));
			ImGui::CloseCurrentPopup();
			ShowSaveAsModal = false; // tmp
		}
		ImGui::EndPopup();
	}
	
	gui.end();
	ofPopStyle();
}

// - - - - - - - -
// MENU EVENT LISTENERS
// - - - - - - - -
void animationController::showShapeLoadMenu(){
	//ofRectangle btn = loadButton.getShape();
	ofFileDialogResult openFileResult= ofSystemLoadDialog("Select the XML file to load...", false, ofToDataPath(KM_SCENE_SAVE_PATH));
	
	if(openFileResult.bSuccess){
		// restrict to saveFiles dir
		ofFile file( ofToDataPath( openFileResult.getPath()) );
		if(file.exists()){
			
			unbindAllShapes();
			
			if( scene.loadScene( ofToDataPath( openFileResult.getPath()) )){
				start();
			}
			else {
				ofLogError("animationController::showShapeLoadMenu()") << "Failed loading " << openFileResult.getPath();
				newConfiguration();
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
