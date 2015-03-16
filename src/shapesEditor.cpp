//
//  shapesEditor.cpp
//  workshopMappingCyril
//
//  Created by Daan de Lange on 14/11/2014.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "shapesEditor.h"


// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
//shapesEditor::shapesEditor( ): scene( NULL ) {
	
//}

shapesEditor::shapesEditor( shapesServer& _scene ): scene(_scene) {
	// init variables
	activeShape = NULL;
	editMode = EDIT_MODE_OFF;
	loadedConfiguration = "";
	
	//batchGui = new ofxUISuperCanvas("Batch Edit Tools");
	//batchGui->disable();
	//gui = new ofxUISuperCanvas("Hello World :)");
	//gui->disable();
	buildMenus();
	
	ofAddListener(ofEvents().mousePressed, this, &shapesEditor::_mousePressed);
}

shapesEditor::~shapesEditor(){
	
	ofRemoveListener( batchGui->newGUIEvent, this, &shapesEditor::batchGuiEvent );
	ofRemoveListener( gui->newGUIEvent, this, &shapesEditor::guiEvent );
	ofRemoveListener( ofEvents().mousePressed, this, &shapesEditor::_mousePressed );
	
	disableEditMode();
	
	delete gui;
	delete batchGui;
	
	// todo: save app settings on quit
}

void shapesEditor::update() {
	// only use computing power if in edit mode.
	if( !isInEditMode() ) return;
	
	// prevent pointhandlers from moving
	if( editMode==EDIT_MODE_BATCH_NONE ){
		syncMultiSelectionHandlers();
	}
	// update multiPointHandlers
	else if( multiShapesSelection.size()>0){
		
		// did one point move ? (compare to rescale
		for(int i = 0; i < multiPointHandlers.size(); i++){
			ofVec2f newPos = multiPointHandlers[i].getPos();
			ofVec2f oldPos = i==0?ofVec2f(multiShapesBoundingBox.x, multiShapesBoundingBox.y):i==1?ofVec2f(multiShapesBoundingBox.x+multiShapesBoundingBox.width, multiShapesBoundingBox.y):i==2?ofVec2f(multiShapesBoundingBox.x+multiShapesBoundingBox.width, multiShapesBoundingBox.y+multiShapesBoundingBox.height):i==3?ofVec2f(multiShapesBoundingBox.x, multiShapesBoundingBox.y+multiShapesBoundingBox.height):ofVec2f(0);
			
			// movement detected?
			if(newPos!=oldPos){
				
				// remember current Rect
				ofRectangle originalBoundingBox(multiShapesBoundingBox);
				
				if( editMode==EDIT_MODE_BATCH_SCALE ){
					// lock ratio ?
					if( ofGetKeyPressed( OF_KEY_SHIFT ) ){
						ofPolyline line;
						ofPoint direction(multiShapesBoundingBox.width, multiShapesBoundingBox.height);
						if(i%2==1) direction.y*=-1.f;
						
						direction.normalize();
						line.addVertex(oldPos+direction*-50);
						line.addVertex(oldPos+direction*50);
						line.close();
						newPos=line.getClosestPoint(newPos);
					}
					
					ofVec2f diff = newPos-oldPos;
					
					if(ofGetKeyPressed(OF_KEY_ALT)){
						// make diff relative to multiPointHandlers[0]
						if(i==1) diff.x*=-1.f;
						else if(i==2) diff*=-1.f;
						else if(i==3) diff.y*=-1.f;
						
						multiShapesBoundingBox.set( multiShapesBoundingBox.getPosition()+diff, multiShapesBoundingBox.getWidth()-diff.x*2.f, multiShapesBoundingBox.getHeight()-diff.y*2.f );
						
						//break; // dont look at the other points now they have moved too
					}
					else switch (i) {
						case 0:
							multiShapesBoundingBox.set( multiShapesBoundingBox.getPosition()+diff, multiShapesBoundingBox.width-diff.x, multiShapesBoundingBox.height-diff.y );
							break;
						case 1:
							multiShapesBoundingBox.set( multiShapesBoundingBox.getPosition()-ofVec2f(0,-diff.y), multiShapesBoundingBox.width+diff.x, multiShapesBoundingBox.height-diff.y );
							break;
						case 2:
							multiShapesBoundingBox.set( multiShapesBoundingBox.getPosition(), multiShapesBoundingBox.width+diff.x, multiShapesBoundingBox.height+diff.y );
							break;
						case 3:
							multiShapesBoundingBox.set( multiShapesBoundingBox.getPosition()+ofVec2f(diff.x,0), multiShapesBoundingBox.width-diff.x, multiShapesBoundingBox.height+diff.y );
							break;
							
						default:
							break;
					}
					
					// put handlers on new container rect
					syncMultiSelectionHandlers();
					
					// calc transformation variables
					ofVec2f scale(multiShapesBoundingBox.width/originalBoundingBox.width, multiShapesBoundingBox.height/originalBoundingBox.height);
					
					// apply transformation to shapes
					for(list<basicShape*>::iterator it=multiShapesSelection.begin(); it!=multiShapesSelection.end(); it++){
						basicShape* s = (*it);
						
						// move its position according to scale factor
						ofVec2f newShapePos( ofMap(s->getPositionPtr()->x, originalBoundingBox.x, originalBoundingBox.x+originalBoundingBox.width, multiShapesBoundingBox.x, multiShapesBoundingBox.x+multiShapesBoundingBox.width), ofMap(s->getPositionPtr()->y, originalBoundingBox.y, originalBoundingBox.y+originalBoundingBox.height, multiShapesBoundingBox.y, multiShapesBoundingBox.y+multiShapesBoundingBox.height) );
						s->setPosition( newShapePos );
						
						// apply scale to shape
						s->applyScale(scale);
						
					}
					
					// dont check other points
					break;
				}
				
				// simple move tool
				else if( editMode==EDIT_MODE_BATCH_MOVE ){
					ofVec2f diff = newPos-oldPos;
					multiShapesBoundingBox.set( multiShapesBoundingBox.getPosition()+diff, multiShapesBoundingBox.width, multiShapesBoundingBox.height );
					syncMultiSelectionHandlers();
					
					// apply transformation to shapes
					for(list<basicShape*>::iterator it=multiShapesSelection.begin(); it!=multiShapesSelection.end(); it++){
						basicShape* s = (*it);
						
						// just move the shape
						s->setPosition( (*s->getPositionPtr()) + diff );
						
					}
					
					// dont check other points
					break;
				}
			}
		}
	}
	
	/*/ update GUI ?
	if(isInEditMode()){
		guiToggle.setPosition( boundingBox.getTopRight()+5 );
		
		if(gui!=NULL){
			
			// update displayed information with real-time data
			((ofxUITextArea*) gui->getWidget("info_Vertexes"))->setTextString("Number of Vertexes:  " + ofToString(points.size()));
		}
	}*/
}

void shapesEditor::draw() {
	// only use computing power if in edit mode.
	if( !isInEditMode() ) return;
	
	// draw shapes (edit mode)
	list<basicShape*>& shapes = scene.getShapesRef();
	for(shapesServer::shapesIterator it = shapes.begin(); it != shapes.end(); it++){
		if( (*it)->isReady() ){
			if( (*it)->isInEditMode() ) (*it)->drawForEditor();
			else (*it)->render();
		}
	}

	// draw rescale tool
	if(editMode!=EDIT_MODE_NORMAL && multiShapesSelection.size()>0){
		// highlight selection
		for(list<basicShape*>::iterator it = multiShapesSelection.begin(); it != multiShapesSelection.end(); it++){
			if( (*it)->isReady() ){
				ofSetColor(0);
				(*it)->drawForEditor();
			}
		}
		
		// show bounding box
		ofPushStyle();
		ofSetColor(255,60);
		ofDrawRectangle(multiShapesBoundingBox);
		ofSetColor(0);
		ofNoFill();
		ofDrawRectangle(multiShapesBoundingBox);
		ofPopStyle();
		
		// draw handlers
		for(int i=0; i<multiPointHandlers.size(); i++){
			multiPointHandlers[i].draw();
		}
	}
}

// - - - - - - - -
// GUI FUNCTIONS
// - - - - - - - -

void shapesEditor::enableEditMode(){
	if( !isInEditMode() ) setEditMode(EDIT_MODE_NORMAL);
}

void shapesEditor::disableEditMode(){
	
	// disable active shape
	selectShape(NULL);
	
	setEditMode( EDIT_MODE_OFF );
}

void shapesEditor::switchEditMode(){
	isInEditMode()?disableEditMode():enableEditMode();
}

bool shapesEditor::isInEditMode() const{
	return !(editMode==EDIT_MODE_OFF);
}

void shapesEditor::guiEvent(ofxUIEventArgs &e){
	string name = e.getName();
	
	// scan for open dropdowns
	// (dirty) bypass for issue https://github.com/rezaali/ofxUI/issues/68
	//ofxUIDropDownList* addShapeDDL = (ofxUIDropDownList*) gui->getWidget("Add Shape");
	//bool dropdownOpen = addShapeDDL->isOpen();
    //if( selectConfigFileOpen && name != "Configuration File") return;
	
	
	// interface options
	// - - - - - - - - -
	if(name==GUIToggleFullScreen){
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		ofSetFullscreen(toggle->getValue());
	}
	else if(name==GUIToggleMouseCursor){
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		toggle->getValue()?ofHideCursor():ofShowCursor();
	}
	
	// selection handling
	// - - - - - - - - - -
	else if(name==GUISelectNextShape){
		if( ( (ofxUILabelButton*) e.getButton())->getValue() == true )
			selectNextShape();
	}
	else if(name==GUISelectPrevShape){
		if( ( (ofxUILabelButton*) e.getButton())->getValue() == true )
			selectPrevShape();
	}
	else if(name==GUIDeleteShape){
		if( ( (ofxUILabelButton*) e.getButton())->getValue() == true ){
			// is a shape selected & does it still exist ?
			if(activeShape!=NULL && scene.shapeExists(activeShape) ){
				scene.removeShape(activeShape);
				
				// deselect
				selectShape(NULL);
			}
		}
	}
	else if(name==GUImultiShapesSelection){
		if( ( (ofxUILabelButton*) e.getButton())->getValue() == true ){
			setEditMode(EDIT_MODE_BATCH_NONE);
		}
	}
	
	// FILE SAVING & LOADING
	// - - - - - - - - - - -
	else if(name==GUIConfigFile){
        //selectConfigFileOpen = !selectConfigFileOpen;
		ofxUIDropDownList* ddl = (ofxUIDropDownList*) e.widget;
		
		// update configuration
		vector<string> names = ddl->getSelectedNames();
		// set (or empty) config file
		loadedConfiguration=(names.size()>0)?names[0]:"";
		
		// add new configuration ?
		if(loadedConfiguration=="- new configuration -"){
			loadedConfiguration = ofSystemTextBoxDialog("Enter a name for the new configuration save file.", "MyGeniusNewConfiguration")+".xml";
			ddl->addToggle(loadedConfiguration);
			ddl->clearSelected();
			ddl->activateToggle(loadedConfiguration); // select current item in the list
		}
		
		// update dropdown title
		ddl->setLabelText("File: "+loadedConfiguration);
		ddl->setName(ddl->getName());
		ddl->activateToggle(loadedConfiguration);
	}
	else if(name==GUILoadConfig){
		if( ( (ofxUILabelButton*) e.getButton())->getValue() == true ){
			if(!scene.loadShapes(loadedConfiguration)) ofSystemAlertDialog("Failed to load from file: "+loadedConfiguration);
		}
	}
	else if(name==GUISaveConfig){
		if( ( (ofxUILabelButton*) e.getButton())->getValue() == true ){
			// warn if no save file
			if(loadedConfiguration.empty()){
				ofSystemAlertDialog("In order to save, you must first select a configuration file.");
			}
			else scene.saveShapes(loadedConfiguration);
		}
	}
	else if(name==GUIAddShape){
		ofxUIDropDownList* ddl = (ofxUIDropDownList*) e.widget;
		
		// update configuration
		vector<string> shapes = ddl->getSelectedNames();
		
		// never remain selected
		ddl->clearSelected();
		
		// set (or empty) config file
		string selected=(shapes.size()>0)?shapes[0]:"";
		
		// no selection ?
		if(selected.empty()) return;
		
		// try to add shape
		bool shapeExists = addShape( selected );
		
	}
	//else cout << "shapesEditor::guiEvent() --> " << name << endl;
}

void shapesEditor::batchGuiEvent(ofxUIEventArgs &e){
	string name = e.getName();
	
	if(name==batchGUISelectAll){
		if( ( (ofxUILabelButton*) e.getButton())->getValue() == true ){
			multiShapesSelection.clear();
			list<basicShape*> &shapes = scene.getShapesRef();
			for(shapesServer::shapesIteratorC it=shapes.begin(); it!=shapes.end(); it++){
				multiShapesSelection.push_back( *it );
			}
			updateMultiShapesSelection();
		}
	}
	else if(name==batchGUISelectNone){
		if( ( (ofxUILabelButton*) e.getButton())->getValue() == true ){
			multiShapesSelection.clear();
			updateMultiShapesSelection();
		}
	}
	// return back to main menu
	else if(name==batchGUICancel){
		if( ( (ofxUILabelButton*) e.getButton())->getValue() == true ){
			setEditMode( EDIT_MODE_NORMAL );
			multiPointHandlers.clear();
			multiShapesSelection.clear();
			updateMultiShapesSelection();
			batchGui->disable();
			gui->enable();
		}
	}
	else if (name==batchGUIRescaleMode){
		if( ( (ofxUILabelToggle*) e.getButton())->getValue() == true )
			setEditMode( EDIT_MODE_BATCH_SCALE );
		else setEditMode( EDIT_MODE_BATCH_NONE );
	}
	else if (name==batchGUIMoveMode){
		if( ( (ofxUILabelToggle*) e.getButton())->getValue() == true )
			setEditMode( EDIT_MODE_BATCH_MOVE );
		else setEditMode( EDIT_MODE_BATCH_NONE );
	}
	else if(name==batchGUIFlipXMode || name==batchGUIFlipYMode){
		if( ( (ofxUILabelToggle*) e.getButton())->getValue() == true){
			ofVec2f scale(name==batchGUIFlipXMode?-1:1, name==batchGUIFlipYMode?-1:1);
			
			// mirror the positions
			ofRectangle originalBoundingBox(multiShapesBoundingBox);
			//multiShapesBoundingBox.set(multiShapesBoundingBox.getPosition()*scale);
			multiShapesBoundingBox.scaleFromCenter(scale.x, scale.y);
			
			// apply transformation to shapes
			for(list<basicShape*>::iterator it=multiShapesSelection.begin(); it!=multiShapesSelection.end(); it++){
				basicShape* s = (*it);
				
				// move its position according to scale factor
				ofVec2f newShapePos( ofMap(s->getPositionPtr()->x, originalBoundingBox.x, originalBoundingBox.x+originalBoundingBox.width, multiShapesBoundingBox.x, multiShapesBoundingBox.x+multiShapesBoundingBox.width), ofMap(s->getPositionPtr()->y, originalBoundingBox.y, originalBoundingBox.y+originalBoundingBox.height, multiShapesBoundingBox.y, multiShapesBoundingBox.y+multiShapesBoundingBox.height) );
				s->setPosition( newShapePos );
				
				// apply scale to shape
				s->applyScale(scale);
			}
			// also prevents them from being moved
			syncMultiSelectionHandlers();
			
			setEditMode( EDIT_MODE_BATCH_NONE );
		}
		// dont continue flipping forever
		else setEditMode( EDIT_MODE_BATCH_NONE );
	}
	else cout << "batchGuiEvent: " << name << endl;
}

void shapesEditor::updateMultiShapesSelection(){
	if(multiShapesSelection.size()==0){
		multiShapesBoundingBox.set(0,0,0,0);
		multiPointHandlers.clear();
		return;
	}
	
	// getRectFromShapes
	ofVec2f from(ofGetWidth(),ofGetHeight());
	ofVec2f to(0,0);
	for(list<basicShape*>::iterator it=multiShapesSelection.begin(); it!=multiShapesSelection.end(); it++){
		ofRectangle bb = (*it)->getBoundingBox();
		
		if(bb.x<from.x) from.x=bb.x;
		if(bb.y<from.y) from.y=bb.y;
		
		if(bb.x+bb.width > to.x) to.x=bb.x+bb.width;
		if(bb.y+bb.height > to.y) to.y=bb.y+bb.height;
	}
	multiShapesBoundingBox.set(from.x, from.y, to.x-from.x, to.y-from.y);
	
	// update gui nb selected shapes
	// todo
	//batchGui->getWidget(batchGUINbSelected);
	
	syncMultiSelectionHandlers();
}

void shapesEditor::syncMultiSelectionHandlers(){
	
	// create pointHandlers
	if(multiPointHandlers.size()!=4){
		multiPointHandlers.clear();
		multiPointHandlers.resize(4);
		for(int i=0; i<multiPointHandlers.size(); i++){
			multiPointHandlers[i].setup();
			multiPointHandlers[i].setEditable( true );
		}
	}
	// setHandlersOnRect
	for(int i=0; i<multiPointHandlers.size(); i++){
		switch (i) {
			case 0:
				multiPointHandlers[i].setPos( ofVec2f(multiShapesBoundingBox.x, multiShapesBoundingBox.y) );
				break;
			case 1:
				multiPointHandlers[i].setPos( ofVec2f(multiShapesBoundingBox.x+multiShapesBoundingBox.width, multiShapesBoundingBox.y) );
				break;
			case 2:
				multiPointHandlers[i].setPos( ofVec2f(multiShapesBoundingBox.x+multiShapesBoundingBox.width, multiShapesBoundingBox.y+multiShapesBoundingBox.height) );
				break;
			case 3:
				multiPointHandlers[i].setPos( ofVec2f(multiShapesBoundingBox.x, multiShapesBoundingBox.y+multiShapesBoundingBox.height) );
				break;
			default:
				break;
		}
	}
}


// - - - - - - - -
// CUSTOM FUNCTIONS
// - - - - - - - -

// adds a shape
bool shapesEditor::addShape(string _shapeType){
	// todo: make this shape-specific (check if shape type exists)
	vertexShape* s = new vertexShape();
	
	basicShape* result = scene.insertShape(s);
	if( result == NULL ) return false;
	
	// setup the shape
	//shapes.back()->spawn();
	
	// enable edit mode
	selectShape(result);
	
	return true;
}

// returns true if shape is unexistent after deletion (even if it didn't exist)
bool shapesEditor::removeShape(basicShape *_s){
	
	if( !scene.removeShape(_s) ) return false;
	
	// deselect ?
	if(activeShape==_s) selectShape(NULL);
	
	return true;
}

void shapesEditor::selectShape(basicShape* _i){
	
	// toggle selection ?
	if( activeShape==_i && scene.shapeExists(_i) ){
		_i->disableEditMode();
		activeShape = NULL;
		return;
	}
	
	// disable edit mode on current one
	if( scene.shapeExists(activeShape) ) activeShape->disableEditMode();
	
	// select it
	if( scene.shapeExists(_i) ){
		
		// remember
		activeShape = _i;
		
		// enter edit mode
		activeShape->enableEditMode();
	}
	else activeShape = NULL;
}

void shapesEditor::selectNextShape(){
	if( scene.getNumShapes()==1 && activeShape!=NULL) selectShape(NULL);
	else if(activeShape==NULL) selectShape( scene.getShapesRef().front() );
	
	else{
		basicShape* next = NULL;
		list<basicShape*> &shapes = scene.getShapesRef();
		for(shapesServer::shapesIteratorC it = shapes.begin(); it != shapes.end(); it++){
			if(activeShape == (*it)){
				it++;
				next=(*it);
				break;
			}
		}
		selectShape( next );
	}
}

void shapesEditor::selectPrevShape(){
	if( scene.getNumShapes()==1 && activeShape!=NULL) selectShape(NULL);
	else if(activeShape==NULL) selectShape( scene.getShapesRef().back() );
	
	else{
		basicShape* prev = NULL;
		list<basicShape*> &shapes = scene.getShapesRef();
		for(list<basicShape*>::reverse_iterator it = shapes.rbegin(); it != shapes.rend(); it++){
			if(activeShape == (*it)){
				it++;
				prev=(*it);
				break;
			}
		}
		selectShape( prev );
	}
}

bool shapesEditor::setEditMode(shapesEditMode _mode){
	
	if(editMode!=EDIT_MODE_OFF && _mode==EDIT_MODE_OFF){
		selectShape(NULL);
		//ofRemoveListener(ofEvents().mousePressed, this, &shapesEditor::_mousePressed);
		gui->disable();
		batchGui->disable();
	}
	
	// show normal GUI ?
	else if(editMode!=EDIT_MODE_NORMAL && _mode==EDIT_MODE_NORMAL){
		gui->enable();
		batchGui->disable();
	}
	
	// sho batch edit GUI ?
	else if(_mode==EDIT_MODE_BATCH_NONE && !batchGui->isEnabled() ){
		gui->disable();
		batchGui->enable();
	}
	
	// remember
	editMode = _mode;
	
	// add listeners if previously off
	//if( editMode==EDIT_MODE_NORMAL && editMode!=EDIT_MODE_NORMAL){
	//}
	
	// rm listener
	if( _mode==EDIT_MODE_NORMAL ){
		//ofRemoveListener(ofEvents().mousePressed, this, &shapesEditor::_mousePressed);
	}
	
	// desselect batchGui elements
	if( batchGui!=NULL ){
		if( _mode!=EDIT_MODE_BATCH_MOVE ) ((ofxUILabelToggle*)batchGui->getWidget(batchGUIMoveMode))->setValue(false);
		if( _mode!=EDIT_MODE_BATCH_SCALE ) ((ofxUILabelToggle*)batchGui->getWidget(batchGUIRescaleMode))->setValue(false);
		if( _mode!=EDIT_MODE_BATCH_FLIPX ) ((ofxUILabelToggle*)batchGui->getWidget(batchGUIFlipXMode))->setValue(false);
		if( _mode!=EDIT_MODE_BATCH_FLIPY ) ((ofxUILabelToggle*)batchGui->getWidget(batchGUIFlipYMode))->setValue(false);
	}
	
	return (editMode == _mode);
}

// - - - - - - - -
// EVENT LISTENERS
// - - - - - - - -

void shapesEditor::_mousePressed(ofMouseEventArgs &e){
	if(!isInEditMode()) return;
	
	// let user select shapes by clicking on them
	if( editMode==EDIT_MODE_BATCH_NONE && e.button==0){
		list<basicShape*> &shapes = scene.getShapesRef();
		for(shapesServer::shapesIterator it=shapes.begin(); it!=shapes.end(); it++){
			if( (*it)->isInside( e )){
				
				// check if already inside ?
				bool found = false;
				for(list<basicShape*>::iterator its=multiShapesSelection.begin(); its!=multiShapesSelection.end(); its++){
					// match ?
					if((*its) == (*it)){
						found = true;
						
						// remove from selection
						multiShapesSelection.erase(its);
						
						break;
					}
				}
				
				if(!found) multiShapesSelection.push_back( *it );
					
				updateMultiShapesSelection();
			}
		}
	}
}

void shapesEditor::buildMenus(){
	// New GUI setup
	gui = new ofxUISuperCanvas("SHAPES EDITOR");
	gui->setColorBack(ofColor(41,123,117,180));
	gui->setColorFill(ofColor(255,160));
	gui->setColorFillHighlight(ofColor(255,220));
	//gui->setColorPadded(ofColor(255,150));
	
	//gui->setPosition(pos.x, pos.y);
	
	gui->addSpacer();
	string textString = "Here you can edit, control and handle different shapes to map them on physical objects.";
	gui->addTextArea("textarea", textString, OFX_UI_FONT_SMALL);
	gui->addToggle(GUIToggleFullScreen, false);
	gui->addToggle(GUIToggleMouseCursor, false);
	gui->addLabel("Press E to toggle edit mode...", OFX_UI_FONT_SMALL);
	gui->addFPSSlider("FPS");
	
	gui->addSpacer();
	gui->addLabel("Load & Save...", OFX_UI_FONT_LARGE);
	vector<string> configurations;
	configurations.push_back("- new configuration -");
	ofDirectory dir("saveFiles/");
	if( !dir.exists() || !dir.canRead() ){
		ofLogNotice("shapesEditor") << "Unable to load save files directory." << endl;
	}
	else{
		dir.allowExt("xml");
		dir.listDir();
		dir.sort();
		for(int i=0; i<dir.size(); i++){
			if(dir.getPath(i)!=shapesEditor_SAVE_FILE) configurations.push_back(dir.getName(i));
		}
	}
	ofxUIDropDownList* ddl = gui->addDropDownList(GUIConfigFile, configurations);
	ddl->setAllowMultiple(false);
	ddl->setAutoClose(true);
	//ddl->setModal(true);
	
	//ddl->setShowCurrentSelected(true);
	//gui->addLabel("Save/Load a configuration", OFX_UI_FONT_SMALL);
	gui->addLabelButton(GUILoadConfig, false);
	gui->addLabelButton(GUISaveConfig, false);
	
	gui->addSpacer();
	gui->addLabel("Edit Shapes", OFX_UI_FONT_LARGE);
	gui->addLabelButton(GUISelectNextShape, false);
	gui->addLabelButton(GUISelectPrevShape, false);
	gui->addLabelButton(GUIDeleteShape, false);
	gui->addLabelButton(GUImultiShapesSelection, false);
	//gui->addModalWidget();
	//remove shape
	gui->addLabel("New shape", OFX_UI_FONT_LARGE);
	vector<string> items;
	items.push_back("vertexShape"); // todo: auto populate this with all available shapes
	ddl = gui->addDropDownList(GUIAddShape, items);
	ddl->setAllowMultiple(false);
	ddl->setAutoClose(true);
	
	gui->autoSizeToFitWidgets();
	
	ofAddListener(gui->newGUIEvent, this, &shapesEditor::guiEvent);
	
	
	// setup rescale GUI
	batchGui = new ofxUISuperCanvas("Shapes re-scale tool");
	batchGui->setColorBack(ofColor(41,123,117,180));
	batchGui->setColorFill(ofColor(255,160));
	batchGui->setColorFillHighlight(ofColor(255,220));
	batchGui->setPosition(10,10);
	
	textString = "This tool lets you rescale several shapes.";
	batchGui->addTextArea("textarea", textString, OFX_UI_FONT_SMALL);
	textString = "Select the target shapes by clicking on them, then proceed to scaling.";
	batchGui->addTextArea("textarea", textString, OFX_UI_FONT_SMALL);
	textString = "Scale modifier keys: ALT for reflected scaling, SHIFT to lock aspect ratio";
	batchGui->addTextArea("textarea", textString, OFX_UI_FONT_SMALL);
	batchGui->addSpacer();
	
	batchGui->addLabelButton(batchGUISelectAll, false);
	batchGui->addLabelButton(batchGUISelectNone, false);
	
	/*/ generate list of all shapes
	 int i = 0;
	 for(list<basicShape*>::iterator it=shapes.begin(); it!=shapes.end(); it++){
	 batchGui->addToggle((*it)->shapeType+"[" + ofToString(i)+"]" + ofToString(shapes.size()), false);
	 i++;
	 }*/
	
	batchGui->addSpacer();
	batchGui->addLabel("Actions to permorm:", OFX_UI_FONT_MEDIUM);
	batchGui->addLabelToggle(batchGUIRescaleMode, false);
	batchGui->addLabelToggle(batchGUIMoveMode, false);
	batchGui->addLabelToggle(batchGUIFlipXMode, false);
	batchGui->addLabelToggle(batchGUIFlipYMode, false);
	
	batchGui->addSpacer();
	batchGui->addLabelButton(batchGUICancel, false);
	
	batchGui->autoSizeToFitWidgets();
	batchGui->disable();
	
	ofAddListener(batchGui->newGUIEvent, this, &shapesEditor::batchGuiEvent);
}
