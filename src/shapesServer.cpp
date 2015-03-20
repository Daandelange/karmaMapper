//
//  shapesServer.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 14/11/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "shapesServer.h"


// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
shapesServer::shapesServer(){
	//editor = new shapesEditor( this );
}

shapesServer::~shapesServer(){
	//delete editor;
}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
void shapesServer::setup() {
	
	// load interface settings
	loadedConfiguration = shapesServer_SAVE_FILE;
	shapeSettings = ofxXmlSettings(loadedConfiguration);
	//shapeSettings.clear();
	ofVec2f pos(0,0);
	
	if(shapeSettings.loadFile(shapesServer_SAVE_FILE)){
		shapeSettings.pushTag("shapesServer");
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
basicShape* shapesServer::insertShape(basicShape* _shape){
	// add shape to stage
	shapes.push_back(_shape);
	
	// on fail, inform user
	//ofLogError("shapesServer::insertShape() failed to instantiate a new shape of type `"+_type+"`.\nYou have no choice but to accept this fact by clicking OK.");
	
	return (_shape == shapes.back())?_shape:NULL;
}

bool shapesServer::removeShape(basicShape *_shape){
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

bool shapesServer::shapeExists(const basicShape* i) const{
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
bool shapesServer::saveShapes(const string _fileName){
	// no save file ?
	if( _fileName.empty() ){
		ofSystemAlertDialog("Please provide a name for the save file!");
		return false;
	}
	
	// save interface settings
	shapeSettings.clear();
	shapeSettings.addTag("shapesServer");
	shapeSettings.pushTag("shapesServer");
	
	//shapeSettings.addValue( "loadedConfiguration", _fileName );
	//shapeSettings.addValue("posX", gui->getRect()->x);
	//shapeSettings.addValue("posY", gui->getRect()->y);
	
	shapeSettings.popTag();
	if(!shapeSettings.saveFile(shapesServer_SAVE_FILE)) ofSystemAlertDialog("Failed saving global settings...");
	
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
		if(failed.size() == 0 ) ofLogNotice("shapesServer::saveShapes()", "Saved current configuration to `"+fullPath+"`");
		else ofSystemAlertDialog("The scene has been saved but "+ ofToString(failed.size()) +" out of "+ ofToString(shapes.size()) +" shapes failed to save.");
	}
	else ofSystemAlertDialog("Could not save the current configuration... :(\nSave File: "+fullPath);
	
	return true;
}

// (re)loads a scene from file
bool shapesServer::loadShapes( const string _fileName ){
	
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
			//(*it) = newShape;
			
			shapeSettings.popTag(); // pop shape
		}
		
		return true;
	}
	else{
		ofLogError("shapesServer::loadShapes()", "Loading from `"+fullPath+"` failed!");
		return false;
	}
}

// todo: fully implement this function
bool shapesServer::unloadShapes(){
	
	
	for(list<basicShape*>::iterator it = shapes.begin(); it != shapes.end(); it++){
		delete (*it);
	}
	
}

// - - - - - - - - -
// SHAPE SERVING FUNCTIONS
// - - - - - - - - -

basicShape* shapesServer::getRandomShape(){
	if(shapes.size()<1){
		ofLogError("shapesServer::getRandomShape()", "There are not yet any shapes to serve!");
		
		// todo; return an error shape ?
		return NULL;
	}
	
	int offset( (int) (ofRandomf()*(shapes.size()-1)) );
	list<basicShape*>::iterator it = shapes.begin();
	std::advance(it,offset);
	return *it;
	
}

// todo: add int _limit restriction param (and randomize)
vector<basicShape*> shapesServer::getShapesByType(string _type){
	vector<basicShape*> ret;
	ret.clear();
	ret.resize(0);
	
	// loop trough shapes and return the wanted ones
	for(list<basicShape*>::iterator it = shapes.begin(); it != shapes.end(); it++){
		if((*it)->getShapeType() == _type ) ret.push_back((*it));
	}
	
	return ret;
}

basicShape* shapesServer::getRandomShapeByType(string _type){
	vector<basicShape*> ret = getShapesByType(_type);
	if(ret.size()<=0) return NULL;
	
	return ret[-.49f+ofRandomf()*(ret.size()-0.51f)];
}

// todo: add int _limit restriction param (and randomize)
vector<basicShape*> shapesServer::getShapesByGroup(int _group){
	vector<basicShape*> ret;
	ret.clear();
	ret.resize(0);
	
	// loop trough shapes and return the wanted ones
	for(list<basicShape*>::iterator it = shapes.begin(); it != shapes.end(); it++){
		if((*it)->getGroupID() == _group ) ret.push_back((*it));
	}
	
	return ret;
}

basicShape* shapesServer::getRandomShapeByGroup(int _group){
	vector<basicShape*> ret = getShapesByGroup(_group);
	if(ret.size()<=0) return NULL;
	return ret[-.49f+ofRandomf()*(ret.size()-.51f)];
}

// - - - - - - -
// UTILITIES
// - - - - - - -
const unsigned int shapesServer::getNumShapes() const {
	return shapes.size();
}
