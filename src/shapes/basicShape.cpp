//
//  basicShape.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 22/3/14.
//
//

#include "basicShape.h"

// static
int basicShape::maxGroupID = 5;

// - - - - - -
// CONSTRUCTORS
// - - - - - -
basicShape::basicShape() {
	
	gui = NULL;
	
	reset();
	shapeType = "basicShape";
	
	ofAddListener(ofEvents().keyPressed, this, &basicShape::keyPressed );
}

basicShape::~basicShape(){
	ofRemoveListener(ofEvents().keyPressed, this, &basicShape::keyPressed );
	
	disableEditMode();
	initialized = false;
	
}

// - - - - - - -
// MAIN FUNCTIONS
// - - - - - - -
void basicShape::reset(){
	// set position
	position = ofVec2f( ofGetWidth()/2, ofGetHeight()/2 );
	boundingBox = ofRectangle(0,0,0,0);
	bEditMode = false;
	initialized = true;
	activeHandle = NULL;
	groupID = -1;
	hasError = false;
	shapeId = 0;
	maxGroupID = 5;
	prevMaxGroupID = maxGroupID;
	
	fgColor = ofColor(255, 210);
	bgColor = ofColor(0);
	
	pointHandlers.clear();
	
	if(gui!=NULL) delete gui;
	gui = NULL;
}

// This function simply uploads data to GPU. basicEffect will set styling
// todo: provide another way for this to allow more complex rendering modes ?
// Maybe effects should be able to render from shapeType.getSpecificShapeData();
void basicShape::render(){
	
	// synch color if maxGroupID changed
	if(prevMaxGroupID != maxGroupID){
		prevMaxGroupID = maxGroupID;
		setColorFromGroupID();
	}
	
	// prepare for drawing
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(position);
	
	if(hasError){
		ofSetHexColor(0xFFFFFF);
		ofDrawBitmapString("error", -20, -5);
	}
	
	// reset
	ofPopStyle();
	ofPopMatrix();
}

// draws the shape for the editor
void basicShape::drawForEditor(){
	
	ofPushStyle();
	
	// show container box
	ofSetColor( 255, 150 );
	ofNoFill();
	ofDrawRectangle( boundingBox );
	
	if(isInEditMode()){
		
		// new colors!
		ofSetColor( fgColor, 200 );
		ofFill();
		
		// draw pointhandlers
		int i=0;
		for(list<movablePoint>::iterator it = pointHandlers.begin(); it != pointHandlers.end(); it++){
			
			(*it).draw();
			
			// show handler ID ?
			if(it->isActive()) ofDrawBitmapStringHighlight(ofToString(i), (*it).getPos()+10);
			
			// incremment
			i++;
		}
		
		// draw center position
		positionPointHandler.draw();

		// draw gui toggle
		ofSetColor(fgColor, 200);
		ofFill();
		ofDrawRectangle(guiToggle);
	}
	
	// reset
	ofPopStyle();
}

// calculates BoundingBox from shape data
void basicShape::calculateBoundingBox(){
	boundingBox.setPosition( position - ofVec2f(-1.f) );
	boundingBox.setWidth( 2 );
	boundingBox.setHeight( 2 );
}

// called after the shape data changed
// used to sync extra short-hand data with the basic shape data
// ex: sync relative points with absolute points
void basicShape::onShapeChanged(){
	
	// update boundingbox
	calculateBoundingBox();
}

// called by animator (or shapesServer?) to resync possibly altered properties by the shape's real properties possibly altered after this call
void basicShape::resetChangedValues(){
	// todo
}

// writes the shape data to XML. xml's cursor is already pushed into the right <shape> tag.
// return success state
bool basicShape::saveToXML(ofxXmlSettings &xml){
	
	// create position tag
	xml.addTag("position");
	xml.pushTag("position");
	xml.addValue("X", getPositionUnaltered().x);
	xml.addValue("Y", getPositionUnaltered().y);
	xml.popTag();
	
	/*/ create GUI position tag
	shapeSettings.addTag("GUIposition");
	shapeSettings.pushTag("GUIposition");
	shapeSettings.addValue("X", (*it)->getGuiPosition().x);
	shapeSettings.addValue("Y", (*it)->getGuiPosition().y);
	shapeSettings.popTag();
	// */
	
	xml.addValue("shapeType", shapeType );
	xml.addValue("groupID", getGroupID() );
	
	return false;
}

// load shape settings from xml
// xml's cursor is pushed in the shape to load
bool basicShape::loadFromXML(ofxXmlSettings& xml){
	
	xml.pushTag("position");
	ofVec2f pos( xml.getValue("X", 0), xml.getValue("Y", 0));
	xml.popTag(); // pop position
	
	// todo: must be  a basicShape;
	position = pos;
	setGroupID( xml.getValue("groupID", -1) );
	
	return true; // todo
}

// - - - - - - -
// SHAPE PROPERTIES
// - - - - - - -
// ### GETTERS
bool basicShape::isReady() const{
	return initialized && !hasError;
}

const string& basicShape::getShapeType() const {
	return shapeType;
}

ofRectangle basicShape::getBoundingBox() const{
	if(!initialized){
		ofLogError("basicShape not yet ready");
		return ofRectangle();
	}
	return boundingBox.getStandardized();
}

int basicShape::getGroupID() const{
	return groupID;
}

vector<string> basicShape::getTypes() const{
	// todo
}

bool basicShape::isType(const string _type) const {
	// todo
}

bool basicShape::isInside( const ofVec2f _pos, const bool _isPositionAbsolute) const{
	if( _isPositionAbsolute ) return boundingBox.inside(_pos);
	else return boundingBox.inside( -position + _pos  );
}

/*ofVec2f basicShape::getPosition() const{
	return position;
}*/

ofVec2f* basicShape::getPositionPtr(){
	// todo: make positionAltered;
	return &position;
}

// allows to retrieve the original, unaltered shape position
ofVec2f basicShape::getPositionUnaltered() const{
	return position;
}

// ### SETTERS
bool basicShape::setPosition( const ofVec2f _pos) {
	position.set( _pos );
	onShapeChanged();
}

bool basicShape::setGroupID(const int _id){
	groupID = _id;
	
	// keep new maxGroupID ?
	if(maxGroupID <= groupID){
		maxGroupID = groupID+1;
	}
	
	setColorFromGroupID();
	
	return true;
}

bool basicShape::isInEditMode() const{
	return bEditMode;
}

// - - - - - - -
// VERTEX SHAPE PROPERTIES
// - - - - - - -


// - - - - - -
// edit stuff
// - - - - - -

bool basicShape::enableEditMode(){
	if(isInEditMode()) return true;
	
	// remember
	bEditMode = true;
	activeHandle = NULL;
	
	positionPointHandler = movablePoint();
	positionPointHandler.setup();
	positionPointHandler.setPos( position );
	positionPointHandler.setEditable( true );
	
	// enable GUI
	guiToggle = ofRectangle( boundingBox.getTopRight()+5, 10, 10 );
	//guiToggle
	ofAddListener(ofEvents().mousePressed, this, &basicShape::mouseClicked );
	//guiTabBar->addLabel("+", OFX_UI_FONT_SMALL);
	
	// build custom UI
	gui = new ofxUISuperCanvas(shapeType);
	gui->setFontSize(OFX_UI_FONT_SMALL, 5);
	gui->setVisible(false);
	gui->setPosition( boundingBox.getTopRight().x+10, boundingBox.getTopRight().y+30 );
	gui->setName(shapeType);
	gui->setColorFill(ofColor(255));
	gui->setColorFill(ofColor(0,200));
	gui->setGlobalButtonDimension(OFX_UI_GLOBAL_BUTTON_DIMENSION);
	gui->addLabel(shapeType+" ["+ofToString(shapeId)+"]", OFX_UI_FONT_MEDIUM);
	//gui->addTextArea("info_GroupID", "Group ID:"+ofToString(groupID), OFX_UI_FONT_SMALL);
	ofxUITextInput* ti = gui->addTextInput("Num Groups", ofToString(maxGroupID) );
	ti->setOnlyNumericInput(true);
	ti->setAutoClear(false);
	ti->setInputTriggerType(OFX_UI_TEXTINPUT_ON_UNFOCUS);
	gui->addIntSlider("Group ID", -1, maxGroupID, groupID);
	
	gui->addSpacer();
	gui->addLabel("Keyboard Shortcuts", OFX_UI_FONT_MEDIUM);
	gui->addTextArea("prevNextHandle", "[ ]   Select prev/next handle", OFX_UI_FONT_SMALL);
	
	// calculate dimensions
	gui->autoSizeToFitWidgets();
	
	// buind GUI
	ofAddListener(gui->newGUIEvent,this,&basicShape::guiEvent);
	
	//guiToggle.setPosition( boundingBox.getTopRight() );
	//cout << guiToggle.getPosition() << endl;
	// store GUI element
	//guiTabBar->loadSettings("settings/", "ui-");
	
	// set GUI color
	setColorFromGroupID();
	
	// call virtual function for sub-classes
	//editModeChanged(bEditMode);
	
	return (bEditMode==true);
}

bool basicShape::disableEditMode(){
	
	if( bEditMode ){
		
		ofRemoveListener(ofEvents().mousePressed, this, &basicShape::mouseClicked );
		ofRemoveListener(gui->newGUIEvent,this,&basicShape::guiEvent);
		
		// update position of whole shape
		ofVec2f difference = positionPointHandler.getPos() - position;
		position += difference;
	
		/*/ update relative point positions to absolute
		list<ofVec2f>::iterator it = absolutePoints.begin();
		for(list<ofVec2f>::iterator it = points.begin(); it != points.end(); it++){
			(*it) = pointHandlers.front().getPos() - this->position;
			// clear first item when we copied it
			pointHandlers.pop_front();
		}*/
		// update points from pointHandlers
		synchronisePointHandlers();
	
		// remember
		bEditMode = false;
		//guiPos = ofVec2f(gui->getRect()->x, gui->getRect()->y);
		
		// clear memory
		delete gui;
		selectHandle(NULL);
		
		// free memory
		pointHandlers.clear();
		
		// recalc some variables
		//onShapeChanged();
	}
	
	return (bEditMode==false);
}

bool basicShape::switchEditMode(){
	bool tmp = bEditMode;
	bEditMode ? disableEditMode() : enableEditMode();
	
	return (bEditMode!=tmp);
}

// updates shape data from it's respective pointhandlers
bool basicShape::synchronisePointHandlers(){
	
	// update shape position
	position = positionPointHandler.getPos();
	
	// sync shape variables
	onShapeChanged();
	
	// update GUI Toggle ?
	if( isInEditMode() ){
		guiToggle.setPosition( boundingBox.getTopRight()+5 );
	}
	
	return true;
}

void basicShape::selectPrevHandle(){
	if( pointHandlers.size()==1 && activeHandle!=NULL) selectHandle(NULL);
	else if(activeHandle==NULL) selectHandle( &positionPointHandler );
	else if( activeHandle==&positionPointHandler ){
		selectHandle(&pointHandlers.back());
	}
	else if( activeHandle == &*pointHandlers.begin() ){
		selectHandle(NULL);
	}
	else{
		movablePoint* prev = NULL;
		for(list<movablePoint>::reverse_iterator it = pointHandlers.rbegin(); it != pointHandlers.rend(); it++){
			if(activeHandle == &(*it)){
				// incremment it
				it++;
				
				prev=&(*it);
				break;
			}
		}
		selectHandle( prev );
	}
}

void basicShape::selectNextHandle(){
	if( pointHandlers.size()==1 && activeHandle!=NULL) selectHandle(NULL);
	else if(activeHandle==NULL) selectHandle( &pointHandlers.front() );
	else if( activeHandle==&positionPointHandler ){
		selectHandle(NULL);
	}
	// select point handler on.
	else if( activeHandle == &*pointHandlers.rend() ){
		selectHandle(&positionPointHandler);
	}
	else{
		movablePoint* next = NULL;
		for(list<movablePoint>::iterator it = pointHandlers.begin(); it != pointHandlers.end(); it++){
			if(activeHandle == &(*it)){
				// incremment it
				it++;
				
				next=&(*it);
				break;
			}
		}
		selectHandle( next );
	}
}

void basicShape::selectHandle(movablePoint* _i){
	if( !isSelectableHandle(_i) && _i!=NULL ) return;
	
	// deselect prev active handle
	if(isSelectableHandle(activeHandle)) activeHandle->blur();
	
	// make new handle active
	activeHandle = _i;
	if(isSelectableHandle(_i)) _i->focus();
}

void basicShape::keyPressed(ofKeyEventArgs& e){
	if(!bEditMode) return;
	
	// select another handle ?
	if( e.key ==  91 ){ // 91 = [
		selectPrevHandle();
		return;
	}
	else if( e.key == 93){ // 93 = ]
		selectNextHandle();
	}
	else if( e.key == OF_KEY_RETURN ){
		selectHandle(NULL);
	}
	
	// editActiveHandleWithArrows
	else if( bEditMode && isSelectableHandle(activeHandle) ){
		ofVec2f translation(0,0);
		int amplifier = 1 + ofGetKeyPressed(OF_KEY_SHIFT)*10;
	
		if( e.key == OF_KEY_DOWN ) translation += ofVec2f(0,1);
		else if( e.key == OF_KEY_UP ) translation += ofVec2f(0,-1);
		else if( e.key == OF_KEY_LEFT ) translation += ofVec2f(-1,0);
		else if( e.key == OF_KEY_RIGHT ) translation += ofVec2f(1,0);
	
		if(translation.x!=0 || translation.y!=0)translateActiveHandle( translation*amplifier );
	}
	
	//else ofLogNotice("basicShape::keyPressed()","Unrecognised key: "+ofToString(e.key));
}

void basicShape::translateActiveHandle(ofVec2f _offset){
	if(!bEditMode) return;
	
	if( isSelectableHandle(activeHandle) ) activeHandle->translate(_offset);
	
}

void basicShape::applyScale(ofVec2f scale){
	// no size, no scale! xD
}

bool basicShape::isSelectableHandle( movablePoint* _i ){
	if(_i==NULL) return false;
	else if(_i == &positionPointHandler) return true;
		
	//return (_i>=0 && _i<pointHandlers.size() );
	for(list<movablePoint>::iterator h=pointHandlers.begin(); h!=pointHandlers.end(); h++){
		movablePoint* tmp = &*h;
		if( &(*h) == _i ) return true;
	}
	
	return false;
}


// GUI STUFF
// - - - - - -
void basicShape::guiEvent(ofxUIEventArgs &e){
	string name = e.getName();
	
	if( name.empty() ){
		//if(gui!=NULL && ((ofxUIToggle* )e.widget)->getValue() ) gui->enable();
	}
	else if(name=="Num Groups"){
		ofxUITextInput* ti = (ofxUITextInput*) e.widget;
		
		if( !ti->getTextString().empty() ){
			maxGroupID = ti->getIntValue();
			//ti->setTextString(ofToString(maxGroupID));
			
			// update max for group ID slider
			ofxUIIntSlider* groupIDSlider = (ofxUIIntSlider*) gui->getWidget("Group ID");
			if(groupIDSlider!=NULL) groupIDSlider->setMax(maxGroupID);
			
			// update color
			setColorFromGroupID();
			
			// re-sync all other shapes ?
			// todo
		}
	}
	else if(name=="Group ID"){
		ofxUIIntSlider* groupIDSlider = (ofxUIIntSlider*) e.widget;
		
		// update value
		setGroupID( groupIDSlider->getValue() );
		
		// remember highest group ID.
		//if(maxGroupID<=groupID) maxGroupID=groupID+1;
		
		// update slider
		//groupIDSlider->setMax(maxGroupID);
		
		// update GUI color
		setColorFromGroupID();
	}
	// todo
	//cout << "-->" << e.getName() << endl;
}

void basicShape::mouseClicked(ofMouseEventArgs &e){
	if(!isInEditMode()) return;
	
	// toggle GUI ?
	else if( guiToggle.inside(e.x, e.y) ){
		gui->toggleVisible();
	}
}

void basicShape::setColorFromGroupID(){
	
	fgColor.setHsb( (255/maxGroupID)*groupID, 150, 180);
	bgColor = fgColor.getNormalized();
	bgColor.setHue(fgColor.getHue()+128);
	if(gui!=NULL && isInEditMode()) gui->setColorBack(fgColor);
	
}

/*void basicShape::addXMLValues(ofxXmlSettings* xml, int _nb){
	string nb = ofToString(_nb);
	xml->setValue("RWI:SHAPE_"+nb+":SHAPE_ID", shapeId);
	xml->setValue("RWI:SHAPE_"+nb+":SHAPE_TYPE", shapeType);
	xml->setValue("RWI:SHAPE_"+nb+":SHAPE_CENTER_X", position.x);
	xml->setValue("RWI:SHAPE_"+nb+":SHAPE_CENTER_Y", position.y);
	xml->setValue("RWI:SHAPE_"+nb+":SHAPE_NUM_POINTS", (int) points.size());
	
	for(int i=0; i<points.size(); i++){
		string l = ofToString(i);
		xml->setValue("RWI:SHAPE_"+nb+":POINTS_"+l+":POS_X", points[i].x);
		xml->setValue("RWI:SHAPE_"+nb+":POINTS_"+l+":POS_Y", points[i].y);
	}
	
	// that's all folks !
}

// loads settings from file
bool basicShape::loadXMLValues(ofxXmlSettings *xml, int _nb){
	// todo -> reset something before loading ?
	
	string nb = ofToString(_nb);
	
	// get basic info
	shapeId = xml->getValue("RWI:SHAPE_"+nb+":SHAPE_ID", 0);
	string tmpShapeType = xml->getValue("RWI:SHAPE_"+nb+":SHAPE_TYPE", "basicShape");
	
	// check if everything went OK
	if(tmpShapeType != shapeType){
#ifdef KARMA_DEBUG
		cout << "basicShape::loadXMLValues() -> ["+shapeType+"] can not load settings of shapeType ["+tmpShapeType+"]." << endl;
#endif
		return false;
	}
	
	// get more info
	position.x = xml->getValue("RWI:SHAPE_"+nb+":SHAPE_CENTER_X", ofGetWidth()/2 );
	position.y = xml->getValue("RWI:SHAPE_"+nb+":SHAPE_CENTER_Y", ofGetHeight()/2);

	basicSpawn( position, shapeId);
	int tmpNumPoints = xml->getValue("RWI:SHAPE_"+nb+":SHAPE_NUM_POINTS", (int) points.size());
	
	// load points
	points.resize(tmpNumPoints);
	for(int i=0; i<tmpNumPoints; i++){
		string l = ofToString(i);
		points[i].x = xml->getValue("RWI:SHAPE_"+nb+":POINTS_"+l+":POS_X", 0);
		points[i].y = xml->getValue("RWI:SHAPE_"+nb+":POINTS_"+l+":POS_Y", 0);
	}
	
	// sets height and width
	updateDimensions();
	
	return true;
}*/

/*void basicShape::setPoints( ofPoint _pts[4] ){
	for(int i=0; i<4; i++) points[i] = _pts[i];
	
	// call thsi over-ruleable "event" function
	pointsUpdated();
} // */

/*ofPoint basicShape::getCenterOffsetFromBoundingBox(){
	return boundingBox.getPosition()-position;
}*/

