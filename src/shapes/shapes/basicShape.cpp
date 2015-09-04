//
//  basicShape.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 22/3/14.
//
//

#include "basicShape.h"

// static
basicPoint basicShape::zeroPoint = basicPoint(0,0);

// - - - - - -
// CONSTRUCTORS
// - - - - - -
basicShape::basicShape() {
	
	initialiseVariables();
	
#ifdef KM_EDITOR_APP
	buildMenu();
#endif
	
#ifdef KM_LOG_INSTANCIATIONS
	ofLogNotice("basicShape::basicShape") << (string)typeid(this).name();
#endif
}

basicShape::~basicShape(){
	initialized = false;

#ifdef KM_EDITOR_APP
	disableEditMode();
	
	position.unbindShape();
	delete shapeGui;
#endif

#ifdef KM_LOG_INSTANCIATIONS
	ofLogNotice("basicShape::basicShape") << (string)typeid(this).name();
#endif
}

// - - - - - - -
// MAIN FUNCTIONS
// - - - - - - -
void basicShape::initialiseVariables(){
	// set position
	position = basicPoint( ofGetWidth()/2, ofGetHeight()/2 );
	boundingBox = ofRectangle(0,0,0,0);
	groupID = -1;
	hasError = false;
	
	shapeTypes.clear();
	shapeTypes.push_back("basicShape");

#ifdef KM_EDITOR_APP
	position.setShape( *this );
	
	bEditMode = false;
	activeHandle = NULL;
	
	maxGroupID = 5;
	
	prevMaxGroupID = maxGroupID;
	
	fgColor = ofColor(255, 210);
	bgColor = ofColor(0);
	
	if(shapeGui!=NULL) delete shapeGui;
	shapeGui = NULL;
#endif
	
	initialized = true;
}

// This function simply uploads data to GPU. basicEffect* will set styling
void basicShape::sendToGPU(){
	
	// prepare for drawing
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(position.x, position.y);
	
	if(hasError){
		ofSetHexColor(0xFFFFFF);
		ofDrawBitmapString("error", -20, -5);
	}
	
	// reset
	ofPopStyle();
	ofPopMatrix();
}

// calculates BoundingBox from shape data
void basicShape::calculateBoundingBox(){
	// 2px box around position
	boundingBox.setPosition( position.x-1, position.y-1 );
	boundingBox.setWidth( 2 );
	boundingBox.setHeight( 2 );
}

// called after the shape data changed
// used to sync extra short-hand data with the basic shape data
// ex: sync relative points with absolute points
void basicShape::onShapeChanged(){
	
	// update boundingbox
	calculateBoundingBox();
	
	// update GUI Toggle ?
	if( isInEditMode() ){
		guiToggle.setPosition( boundingBox.getTopRight()+5 );
	}
}

// called by animator to reset possibly altered properties to the shape's initial (scene) properties
void basicShape::resetToScene(){
	// todo
}

// writes the shape data to XML. xml's cursor is already pushed into the right <shape> tag.
// return success state
bool basicShape::saveToXML(ofxXmlSettings &xml){
	
	// create position tag
	xml.addTag("position");
	xml.pushTag("position");
	xml.addValue("X", getPositionUnaltered()->x);
	xml.addValue("Y", getPositionUnaltered()->y);
	xml.popTag();
	
	xml.addValue("shapeType", "basicShape" );
	xml.addValue("groupID", getGroupID() );
	
	return true;
}

// load shape settings from xml
// xml's cursor is pushed in the shape to load
bool basicShape::loadFromXML(ofxXmlSettings& xml){
	
	xml.pushTag("position");
	position.setPos( xml.getValue("X", 0), xml.getValue("Y", 0));
	xml.popTag(); // pop position
	
	setGroupID( xml.getValue("groupID", -1) );
	
	return true; // todo
}

// - - - - - - -
// SHAPE PROPERTIES
// - - - - - - -
// ### GETTERS
bool basicShape::isReady() const{
	//ofScopedLock()
	return initialized && !hasError;
}

const string basicShape::getShapeType() const {
	return shapeTypes.back();
}

ofRectangle basicShape::getBoundingBox() const{
	if(!initialized){
		ofLogError("basicShape not yet ready. This message can lead to memory leaks.");
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
	return (std::find( shapeTypes.begin(), shapeTypes.end(), _type) != shapeTypes.end());
}

bool basicShape::isInside( const ofVec2f _pos, const bool _isPositionAbsolute) const{
	if( _isPositionAbsolute ) return boundingBox.inside(_pos);
	else return boundingBox.inside( -position.x + _pos.x, -position.y + _pos.y );
}

/*ofVec2f basicShape::getPosition() const{
	return position;
}*/

basicPoint* basicShape::getPositionPtr(){
	// todo: make positionAltered;
	return &position;
}

// allows to retrieve the original, unaltered shape position
basicPoint* basicShape::getPositionUnaltered(){
	return &position;
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
#ifdef KARMAMAPPER__DEBUG
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

#ifdef KM_EDITOR_APP
// * * * * * * * * * * * * * * * * * * * * * * * * * * *
// basicShape

int basicShape::maxGroupID = 6;

//
// EDIT ESSENTIALS
//

// draws the shape for the editor
void basicShape::render(){
	
	// synch color if maxGroupID changed
	/*if(prevMaxGroupID != maxGroupID){
		prevMaxGroupID = maxGroupID;
		setColorFromGroupID();
	 }*/
	
	ofPushStyle();
	
	// show container box
	ofSetColor( 255, 150 );
	ofNoFill();
	ofDrawRectangle( boundingBox );
	
	if(isInEditMode()){
		
		// new colors!
		ofSetColor( fgColor, 200 );
		ofFill();
		
		// draw center position
		position.draw();
		
		// draw gui toggle
		ofSetColor(fgColor, 200);
		ofFill();
		ofDrawRectangle(guiToggle);
	}
	
	// reset
	ofPopStyle();
}

void basicShape::buildMenu(){
	// build custom UI
	
	menuParams.clear();
	menuParams.setup( getShapeType() );
	menuParams.setShowHeader(false);
	
	menuParams.add( (new ofxLabelExtended())->setup("LabelName", getShapeType()+" ["+ofToString(this)+"]")->setShowLabelName(false));
	//gui->addTextArea("info_GroupID", "Group ID:"+ofToString(groupID), OFX_UI_FONT_SMALL);
	/*ofxUITextInput* ti = gui->addTextInput("Num Groups", ofToString(maxGroupID) );
	 ti->setOnlyNumericInput(true);
	 ti->setAutoClear(false);
	 ti->setInputTriggerType(OFX_UI_TEXTINPUT_ON_UNFOCUS);
	 gui->addIntSlider("Group ID", -1, maxGroupID, groupID);
	 
	 gui->addSpacer();
	 gui->addLabel("Keyboard Shortcuts", OFX_UI_FONT_MEDIUM);
	 gui->addTextArea("prevNextHandle", "[ ]   Select prev/next handle", OFX_UI_FONT_SMALL);*/
	
	
	shapeGui = new ofxPanelExtended();
	shapeGui->add( &menuParams );
	//shapeGui->setShowHeader(false);
}

// - - - - - -
// EDITING CONTROL
// - - - - - -
bool basicShape::enableEditMode(){
	if(isInEditMode()) return true;
	
	// remember
	bEditMode = true;
	activeHandle = NULL;
	
	position.setEditable( true );
	
	// enable GUI
	guiToggle = ofRectangle( boundingBox.getTopRight()+5, 10, 10 );
	//guiToggle
	//guiTabBar->addLabel("+", OFX_UI_FONT_SMALL);
	
	// set GUI color
	setColorFromGroupID();
	
	return (bEditMode==true);
}

bool basicShape::disableEditMode(){
	
	if( bEditMode ){
		
		// remember
		bEditMode = false;
		
		// clear memory
		selectHandle(NULL);
		
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

bool basicShape::isInEditMode() const{
	return bEditMode;
}

void basicShape::selectPrevHandle(){
	if( activeHandle==&position ) selectHandle(NULL);
	else selectHandle( &position );
}

void basicShape::selectNextHandle(){
	basicShape::selectPrevHandle();
}

void basicShape::selectHandle(basicPoint* _i){
	if( _i == NULL || handleExists(_i) ){
		// deselect prev active handle
		if(handleExists(activeHandle)) activeHandle->blur();
		
		// remember
		activeHandle = _i;
		
		// make new handle active
		if( _i != NULL ) _i->focus();
	}
}

void basicShape::translateActiveHandle(basicPoint _offset){
	if(!bEditMode) return;
	
	if( handleExists(activeHandle) ) activeHandle->translate(_offset);
	
}

void basicShape::applyScale(basicPoint scale){
	// no size, no scale! xD
}

bool basicShape::handleExists( basicPoint* _i ){
	return (_i == &position);
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
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
	else if( bEditMode && handleExists(activeHandle) ){
		basicPoint translation(0,0);
		int amplifier = 1 + ofGetKeyPressed(OF_KEY_SHIFT)*10;
		
		if( e.key == OF_KEY_DOWN ) translation += basicPoint(0,1);
		else if( e.key == OF_KEY_UP ) translation += basicPoint(0,-1);
		else if( e.key == OF_KEY_LEFT ) translation += basicPoint(-1,0);
		else if( e.key == OF_KEY_RIGHT ) translation += basicPoint(1,0);
		
		if(translation.x!=0 || translation.y!=0)translateActiveHandle( translation*amplifier );
	}
	
	//else ofLogNotice("basicShape::keyPressed()","Unrecognised key: "+ofToString(e.key));
}

// returns true if intercepted (stops event bubbling), false otherwise
bool basicShape::interceptMouseClick(ofMouseEventArgs &e){
	if( isInEditMode() ){
		// toggle GUI ?
		if( guiToggle.inside(e.x, e.y) ){
			//gui->toggleVisible();
			// todo
			return true;
		}
		
		// move shape ?
		else if( position.interceptMouseClick(e) ){
			
			return true;
		}
	}
	
	return false;
}

void basicShape::setColorFromGroupID(){
	
	fgColor.setHsb( (255/maxGroupID)*groupID, 150, 180);
	bgColor = fgColor.getNormalized();
	bgColor.setHue(fgColor.getHue()+128);
	//if(gui!=NULL && isInEditMode()) gui->setColorBack(fgColor);
	
}

//
// SETTERS
//
bool basicShape::setPosition( const basicPoint _pos) {
	position.setPos( _pos );
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
// endif KM_EDITOR_APP
#endif