//
//  realWorldInterface.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 27/3/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "realWorldInterface.h"


void realWorldInterface::setup() {
	activeShape = -1;
	isInEditMode = false;
	
	nextSaveId = 0;
	
	// setup GUI
	gui.setup("Shapes configuration");
	
	// add buttons
	gui.add( addNewShape.setup("Add new shape", 200, 30) );
	gui.add( selectNextShape.setup("SELECT NEXT SHAPE", 200, 30) );
	gui.add( loadButton.setup("Load", 200, 30) );
	gui.add( saveButton.setup("Save", 200, 30) );
	gui.add( removeButton.setup("Remove selected shape", 200, 30));
	gui.add( nextHandleButton.setup("Select next handle", 200, 30));
	
	// bind GUI buttons
	addNewShape.addListener(this, &realWorldInterface::addShape);
	loadButton.addListener(this, &realWorldInterface::loadSettings);
	saveButton.addListener(this, &realWorldInterface::saveSettings);
	selectNextShape.addListener(this, &realWorldInterface::editNextShape);
	removeButton.addListener(this, &realWorldInterface::removeActiveShape);
	nextHandleButton.addListener(this, &realWorldInterface::selectNextShapeHandle);
	
	// bind detailed item editing
	//ofAddListener( ofEvents().keyPressed, this, &realWorldInterface::editShapeItem);
	
	// load previously saved settings
	loadSettings();
	
}


void realWorldInterface::update() {
	if(!isInEditMode) return;
	
}


void realWorldInterface::draw() {
	
	// no edit mode, no draw
	if( !isInEditMode ) return;
	
	// draw wireframe ?
	if( numShapes>0 && isShapesIndex(activeShape) ){
		shapes[activeShape]->basicDrawWireframe();
	}
	
	
	//for(int i=0; i<numShapes; i++){
	//	shapes[i]->basicDrawWireframe();
	//}
	
	// show gui
	gui.draw();
	
}

void realWorldInterface::exit(){
	
	resetShapes(); // ensures the shapes are correctly removed from memory
}

// used to return the right class from a string
// if you made a custom shape you'll need to add yours here.
basicShape* realWorldInterface::stringToShapeClass(string _s){
	
	// add your custom shape in this list, if you made one
	if(_s == "vectorShape") return new vectorShape();
	
	//else if(type == "yourShape") s = new yourShape();
	//else if(type == "yourShape") s = new yourShape();
	//else if(type == "yourShape") s = new yourShape();
	
	//else return new basicShape();
}

void realWorldInterface::addShape(){
	// todo: GUI element that lets you select the shape
	string type="vectorShape";
	
	basicShape* s;
	
	// get shape instance
	s = stringToShapeClass( type );
	
	// add a line here if you made a custom shape
	shapes.push_back(s);
	
	// setup the shape
	shapes.back()->basicSpawn( ofPoint( ofGetWidth()/2, ofGetHeight()/2), nextSaveId );
	
	// update stack size
	numShapes = shapes.size();
	
	// enable edit mode
	if(isShapesIndex(activeShape)) shapes[activeShape]->disableEditMode();
	shapes.back()->enableEditMode();
	activeShape = numShapes-1;
	
	// increment next name
	nextSaveId++;
	
	return;// numShapes-1;
}

void realWorldInterface::removeShape(int shapeId){
	// dont you try to delete the undeletable!
	if(!isShapesIndex(shapeId)) return;
	
	// unselect active shape
	if( shapeId == activeShape) activeShape = -1;
	
	shapes[shapeId]->basicDestroy();
	
	// rm
	// todo | tocheck : is only the pointer deleted or also the instance ?
	//delete &shapes[shapeId];
	shapes.erase( shapes.begin() + shapeId );
	
	//
	numShapes = shapes.size();
}

void realWorldInterface::removeActiveShape(){
	removeShape(activeShape);
}

void realWorldInterface::editShape(int shapeId){
	if(shapeId < 0 || shapeId >= numShapes) return;
	
	// disable others
	for(int i=0; i<numShapes; i++){
		shapes[i]->disableEditMode();
	}
	
	// this would be faster but method above systematicaly disables others
	//if(activeShape >= 0 && activeShape < numShapes) shapes[activeShape]->disableEditMode();
	
	// enable the right one
	shapes[shapeId]->enableEditMode();
	
	activeShape = shapeId;
}

void realWorldInterface::editNextShape(){
	if(numShapes == 0) return;
	
	// get next one
	int next = 0;
	if(activeShape==-1) next=0;
	// loops back to no selection
	else if( activeShape+1 == numShapes ) next=-1;
	// increment
	else next = ( activeShape + 1 ) % numShapes;
	
	// update & apply statuses
	if(next != activeShape){ // if not already in edit mode
		
		// disable previous one
		if(isShapesIndex(activeShape) ) shapes[activeShape]->disableEditMode();
		
		// enable new one
		if(isShapesIndex(next)) shapes[next]->enableEditMode();
	}
	
	// remember
	activeShape = next;
	
}

void realWorldInterface::resetShapes(){
	
	// delete points correctly from memory
	for(int i=0; i<shapes.size(); i++){
		shapes[i]->basicDestroy();
	}
	
	shapes.clear();
	numShapes=shapes.size();
	activeShape=-1;
}

void realWorldInterface::selectNextShapeHandle(){
	if(isShapesIndex(activeShape)) shapes[activeShape]->selectNextHandle();
}

bool realWorldInterface::isShapesIndex(int i){
	return (i >= 0 && i < numShapes);
}

void realWorldInterface::saveSettings(){
	// apply current transformation
	if(isShapesIndex(activeShape)) shapes[activeShape]->disableEditMode();
	
	// put GUI settings
	gui.saveToFile("rwiGuiSettings.xml");
	
	ofxXmlSettings* xml = new ofxXmlSettings();
	
	// remember basic settings
	xml->setValue("RWI:ACTIVE_SHAPE", activeShape);
	xml->setValue("RWI:NUM_SHAPES", numShapes);
	
	// let each shape write its settings.
	for(int i=0; i<numShapes; i++){
		shapes[i]->addXMLValues( xml, i);
	}
	
	// write to file
	xml->saveFile("rwiSettings.xml");
	
	// delete pointer instance
	xml->clear();
	delete xml; // is this correct ? tocheck
}

void realWorldInterface::loadSettings(){
	
	// get GUI settings
	gui.loadFromFile("rwiGuiSettings.xml");
	
	ofxXmlSettings* xml = new ofxXmlSettings();
	if(!xml->loadFile("rwiSettings.xml")){
		
#ifdef KARMA_DEBUG
		cout << "realWorldInterface::loadSettings() -> No save file was found or it was corrupted." << endl;
#endif
		return;
	}
	
	// reset shapes
	this->resetShapes();
	
	// remember basic settings
	activeShape = xml->getValue("RWI:ACTIVE_SHAPE", -1);
	numShapes = xml->getValue("RWI:NUM_SHAPES", 0);
	if(numShapes < 0) numShapes = 0;
	
	// let each shape write its settings.
	shapes.resize(numShapes);
	for(int i=0; i<numShapes; i++){
		string tmpShapeType = xml->getValue("RWI:SHAPE_"+ofToString(i)+":SHAPE_TYPE", "basicShape");
		shapes[i] = stringToShapeClass(tmpShapeType);
		shapes[i]->loadXMLValues( xml, i);
	}
	
	// enable edit mode if needed
	if( isShapesIndex(activeShape) ) shapes[activeShape]->enableEditMode();
	
	// delete pointer instance
	xml->clear();
	delete xml; // is this correct ? tocheck
}
