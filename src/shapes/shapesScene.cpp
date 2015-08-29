//
//  shapesScene.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 14/11/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "shapesScene.h"


// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
shapesScene::shapesScene(){
	//editor = new shapesEditor( this );
}

shapesScene::~shapesScene(){
	//delete editor;
}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
void shapesScene::setup() {
	
	// load interface settings
	loadedConfiguration = shapesScene_SAVE_FILE;
	shapeSettings = ofxXmlSettings(loadedConfiguration);
	//shapeSettings.clear();
	ofVec2f pos(0,0);
	
	if(shapeSettings.loadFile(shapesScene_SAVE_FILE)){
		shapeSettings.pushTag("shapesScene");
		pos = ofVec2f( shapeSettings.getValue("posX", 0), shapeSettings.getValue("posY", 0));
		loadedConfiguration = shapeSettings.getValue("loadedConfiguration", "");
		shapeSettings.popTag(); // pop position
	}
	else ofSystemAlertDialog("Failed loading app settings...");
	shapeSettings.clear();
	
	// load created shapes
	loadShapes( loadedConfiguration );
	
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
			break;
		}
	}
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
bool shapesScene::saveShapes(const string _fileName){
	// no save file ?
	if( _fileName.empty() ){
		ofSystemAlertDialog("Please provide a name for the save file!");
		return false;
	}
	
	// save interface settings
	shapeSettings.clear();
	shapeSettings.addTag("shapesScene");
	shapeSettings.pushTag("shapesScene");
	
	//shapeSettings.addValue( "loadedConfiguration", _fileName );
	//shapeSettings.addValue("posX", gui->getRect()->x);
	//shapeSettings.addValue("posY", gui->getRect()->y);
	
	shapeSettings.popTag();
	if(!shapeSettings.saveFile(shapesScene_SAVE_FILE)) ofSystemAlertDialog("Failed saving global settings...");
	
	string fullPath = "saveFiles/"+_fileName;
	
	// clear settings
	shapeSettings.clear();
	
	// save all shapes data
	int s=0;
	vector<int> failed;
	failed.clear();
	for(list<basicShape*>::iterator it = shapes.begin(); it != shapes.end(); it++, s++){
		
		shapeSettings.addTag("shape");
		shapeSettings.pushTag("shape", s);
		
		if(!(*it)->saveToXML(shapeSettings)) failed.push_back(1);
		
		shapeSettings.popTag(); //pop shape
	}
	
	// write down settings to disk
	if(shapeSettings.saveFile(fullPath)){
		if(failed.size() == 0 ) ofLogNotice("shapesScene::saveShapes()", "Saved current configuration to `"+fullPath+"`");
		else ofSystemAlertDialog("The scene has been saved but "+ ofToString(failed.size()) +" out of "+ ofToString(shapes.size()) +" shapes failed to save.");
	}
	else ofSystemAlertDialog("Could not save the current configuration... :(\nSave File: "+fullPath);
	
	return true;
}

// (re)loads a scene from file
bool shapesScene::loadShapes( const string _fileName ){
	
	string fullPath;
	
	// use default config ?
	if( _fileName.empty() ){
		if( loadedConfiguration.empty() ) return false; // we have no input file
		else fullPath = "saveFiles/"+loadedConfiguration;
	}
	else fullPath = "saveFiles/"+_fileName;
	
	// can we read the file ?
	if(shapeSettings.loadFile(fullPath)){
		
		// unload scene
		unloadShapes();
		
		// loop for each shape
		int numShapes = shapeSettings.getNumTags("shape");
		shapes.clear();
		shapes.resize(numShapes);
		int s=0;
		for(list<basicShape*>::iterator it = shapes.begin(); it != shapes.end(); it++, s++){
			shapeSettings.pushTag("shape", s);
			
			// todo: make this shape specific
			// --> http://stackoverflow.com/questions/8269465/how-can-i-instantiate-an-object-knowing-only-its-name
			(*it) = new vertexShape();
			(*it)->loadFromXML(shapeSettings);
			
			shapeSettings.popTag(); // pop shape
		}
		
		return true;
	}
	else{
		ofLogError("shapesScene::loadShapes()", "Loading from `"+fullPath+"` failed!");
		return false;
	}
}

// todo: fully implement this function
bool shapesScene::unloadShapes(){
	
	
	for(list<basicShape*>::iterator it = shapes.begin(); it != shapes.end(); it++){
		delete (*it); // todo: does this call destructor ?
	}
	
}

// - - - - - - -
// UTILITIES
// - - - - - - -
const unsigned int shapesScene::getNumShapes() const {
	return shapes.size();
}
