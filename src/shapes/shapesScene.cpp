//
//  shapesScene.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 14/11/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "shapesScene.h"

// forward declare
//namespace shape {
//	basicShape* create(const std::string& name);
//}

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
shapesScene::shapesScene(){
	loadedConfiguration = "";
	
	shapes.resize(0);
	
#ifdef KM_LOG_INSTANCIATIONS
	ofLogNotice("shapesScene::shapesScene");
#endif
}

shapesScene::~shapesScene(){
	
	unloadShapes();
}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
bool shapesScene::isEditorClass() const{
	return false;
}

void shapesScene::loadLastUsedScene() {
	
	ofxXmlSettings sceneSettings;
	
	// do we have a saveFile containing the last used scene ?
	if( sceneSettings.loadFile(KM_SCENE_SAVE_FILE) ){
		sceneSettings.pushTag("shapesScene");
		
		// load created shapes
		loadScene( sceneSettings.getValue("lastLoadedScene", KM_DEFAULT_SCENE) );
		
		sceneSettings.popTag(); // pop shapesScene
	}
	sceneSettings.clear();
	
}

string shapesScene::getLoadedScene() const {
	return loadedConfiguration;
}

// - - - - - - - -
// CUSTOM FUNCTIONS
// - - - - - - - -

// todo: make this shape-specific (check if shape type exists)
basicShape* shapesScene::insertShape(basicShape* _shape){
	// add shape to stage
	shapes.push_back(_shape);
	
	// on fail, inform user
	//ofLogError("shapesScene::insertShape() failed to instantiate a new shape of type `"+_type+"`.\nYou have no choice but to accept this fact by clicking OK.");
	
	return (_shape == shapes.back())?_shape:NULL;
}

bool shapesScene::removeShape(basicShape *_shape){
	// already removed // not existing
	if( !shapeExists(_shape) ) return true;
		
	for(list<basicShape*>::iterator it = shapes.begin(); it != shapes.end(); it++){
		if(_shape==*it){
			shapes.erase(it);
                        return true;
		}
	}
        return true;
}

// - - - - - - - -
// GUI FUNCTIONS
// - - - - - - - -

bool shapesScene::shapeExists(const basicShape* i) const{
	//return (i >= 0 && i < shapes.size());
	
	// no null pointers plz
	if(i==NULL) return false;
	
	// check all shapes
	for(shapesIteratorC it = shapes.begin(); it != shapes.end(); it++){
		if( (*it) == i ) return true;
	}
	
	// not found ?
	return false;
}


// todo: move save functionalities to shape object
bool shapesScene::saveScene(const string& _fileName){
	string fullPath;
	// no save file ?
	if( _fileName.empty() ){
		if( loadedConfiguration.empty() ){
			ofSystemAlertDialog("Please provide a name for the save file!");
			return false;
		}
		else fullPath = loadedConfiguration;
		
	}
	else fullPath = _fileName;
	
	ofxXmlSettings sceneXML;
	sceneXML.addTag("shapes");
	sceneXML.pushTag("shapes");
	
	// save all shapes data
	int s=0;
	vector<int> failed;
	failed.clear();
	for(auto it = shapes.begin(); it != shapes.end(); it++, s++){
		
		sceneXML.addTag("shape");
		sceneXML.pushTag("shape", s);
		
		if(!(*it)->saveToXML(sceneXML)) failed.push_back(s);
		
		sceneXML.popTag(); //pop shape
	}
	
	// write down settings to disk
	if( sceneXML.saveFile(fullPath) ){
		if(failed.size() == 0 ) ofLogNotice("shapesScene::saveScene()", "Saved current configuration to `"+fullPath+"`");
		else ofSystemAlertDialog("The scene has been saved but "+ ofToString(failed.size()) +" out of "+ ofToString(shapes.size()) +" shapes failed to save.");
	}
	else ofSystemAlertDialog("Could not save the current configuration...\nSave File: "+fullPath);
	
	return true; // todo: this should be conditional
}

// (re)loads a scene from file
bool shapesScene::loadScene( const string& _fileName ){
	
	string fullPath;
	
	// use default config ?
	if( _fileName.empty() ){
		if( loadedConfiguration.empty() ) return false; // we have no input file
		else fullPath = loadedConfiguration;
	}
	else fullPath = ofToDataPath( _fileName );
	
	ofxXmlSettings sceneXML;
	
	// can we read the file ?
	if( sceneXML.loadFile(fullPath) ){
		
		unloadShapes();
		
		sceneXML.pushTag("shapes");
		
		// loop for each shape
		int numShapes = sceneXML.getNumTags("shape");
		shapes.clear();
		//shapes.resize(numShapes);
		
		for(int s=0; s<numShapes; s++){
			sceneXML.pushTag("shape", s);
			
			// Some code comes from:
			// --> http://stackoverflow.com/questions/8269465/how-can-i-instantiate-an-object-knowing-only-its-name
			string shapeType = sceneXML.getValue("shapeType", "basicShape");
			basicShape* shape = shape::create(shapeType, basicPoint(0,0) );
			if( shape != nullptr ){
				shapes.push_back( shape );
				shapes.back()->loadFromXML( sceneXML );
			}
			else {
				// unknow shape type
				ofLogError("basicShape* shape::create") << "Shapetype not found: " << shapeType;
			}
			
			sceneXML.popTag(); // pop shape
		}
		
		ofLogNotice("shapesScene::loadScene") << "Loaded scene from " << fullPath << " [" << numShapes << " shapes]";
		
		// remember this scene
		loadedConfiguration = _fileName;
		ofxXmlSettings sceneSettings;
		sceneSettings.load( KM_SCENE_SAVE_FILE );
		sceneSettings.setValue("sceneSettings:lastLoadedScene", _fileName);
		
		if(!sceneSettings.saveFile(KM_SCENE_SAVE_FILE)) ofLogError("shapesScene::saveScene") << "Failed saving global settings...";
		
		return true;
	}
	else{
		ofLogError("shapesScene::loadShapes()", "Loading from `"+fullPath+"` failed!");
		return false;
	}
}


bool shapesScene::unloadShapes(){
	
	// dump'em all! :D
	while( shapes.size() > 0 ){
		auto it = shapes.begin();
		delete (*it);
		shapes.erase( it );
	}
	
	// file is not loaded anymore
	loadedConfiguration = "";

	return true;
}

// - - - - - - -
// UTILITIES
// - - - - - - -
const unsigned int shapesScene::getNumShapes() const {
	return shapes.size();
}
