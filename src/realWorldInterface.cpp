//
//  realWorldInterface.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 27/3/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "realWorldInterface.h"

#define HELP_ZONE_WIDTH 600

realWorldInterface::realWorldInterface(){
	showHelp = true;
}

realWorldInterface::~realWorldInterface(){
	// rm eventlisteners
	ofRemoveListener( ofEvents().keyPressed, this, &realWorldInterface::_keyPressed);
	ofRemoveListener( ofEvents().windowResized, this, &realWorldInterface::_resized);
	
	resetShapes(); // ensures the shapes are correctly removed from memory
}


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
	ofAddListener( ofEvents().keyPressed, this, &realWorldInterface::_keyPressed);
	ofAddListener( ofEvents().windowResized, this, &realWorldInterface::_resized);
	
	// setup help box
	textBox.loadFont("F25_Bank_Printer.ttf", 12); // get --> http://www.dafont.com/fr/f25-bank-printer.font?fpp=50&a=on&e=on&l[]=10&l[]=1
	textBox.setText("HELP\n - - - -\nGlobal:\n\te = toggle edit mode\n\th = toggle this help box\n\nShape editing\n\tr+click [on a point] = remove point\n\tClick+drag [on a point] = drag\n\tRight click [on a vertexShape line] = add point\n\n");
	textBox.showsFrame(false);
    textBox.setRectangle( ofRectangle( ofGetWidth()-HELP_ZONE_WIDTH+20 ,40,HELP_ZONE_WIDTH,400) );
	
	// load previously saved settings
	loadSettings();
	
}


void realWorldInterface::update() {
	if(!isInEditMode) return;
	
}


void realWorldInterface::draw() {
	
	// no edit mode, no draw
	if( isInEditMode ){
	
		// draw wireframe ?
		if( numShapes>0 && isShapesIndex(activeShape) ){
			shapes[activeShape]->basicDrawWireframe();
		}
		
		if(showHelp){
			ofSetHexColor(0x000000);
			ofFill();
			ofRect( textBox.getRect() );
			ofSetHexColor(0xFFFFFF);
			ofNoFill();
			textBox.draw();
		}
	
		// show gui
		gui.draw();
	}
	
	// render effects on shapes
	else {
		//effects.render();
		for(int s=0; s<shapes.size(); s++){
			ofPushMatrix();
			ofTranslate( shapes[s]->position );
			shapes[s]->drawWireframe();
			ofPopMatrix();
		}
	}
}

void realWorldInterface::exit(){
	
	
}

// used to return the right class from a string
// if you made a custom shape you'll need to add yours here.
basicShape* realWorldInterface::stringToShapeClass(string _s){
	
	// add your custom shape in this list, if you made one
	if(_s == "vertexShape") return new vertexShape();
	//else if(_s == "") return new yourShape();
	//else if(_s == "yourShape") return new yourShape();
	
	else{
		ofLogNotice("realWorldInterface::stringToShapeClass() --> Unknown shape loaded from XML file: "+_s);
		return NULL; // default shape is vertex
	}
}

void realWorldInterface::addShape(){
	// todo: GUI element that lets you select the shape
	string type="vertexShape";
	
	basicShape* s;
	
	// get shape instance
	s = stringToShapeClass( type );
	
	if(s==NULL){
		ofLogNotice("realWorldInterface::addShape() --> Could not create a shape of type "+type);
		return;
	}
	
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

void realWorldInterface::_keyPressed( ofKeyEventArgs& args ){
	if( args.key == 'h' || args.key == 'H' ){
		showHelp = !showHelp;
	}
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
	delete xml;
	
	// reselect current shape
	if(isShapesIndex(activeShape)) shapes[activeShape]->enableEditMode();
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
		// load shape into memory
		string tmpShapeType = xml->getValue("RWI:SHAPE_"+ofToString(i)+":SHAPE_TYPE", "vertexShape");
		shapes[i] = stringToShapeClass(tmpShapeType);
		
		// verification
		if( shapes[i] == NULL ){
			// rm shape
			shapes.erase(shapes.begin()+i);
			numShapes--;
			
			// rm saved stuff ? (or not to keep the crap in the file)
		}
		
		// put back data into shape
		else shapes[i]->loadXMLValues( xml, i);
		
	}
	
	// enable edit mode if needed
	if( isShapesIndex(activeShape) ) shapes[activeShape]->enableEditMode();
	
	// delete pointer instance
	xml->clear();
	delete xml; // is this correct ? tocheck
}

void realWorldInterface::_resized( ofResizeEventArgs &args ){
	textBox.setRectangle( ofRectangle( ofGetWidth()-HELP_ZONE_WIDTH+20 ,40,HELP_ZONE_WIDTH,400) );
}
