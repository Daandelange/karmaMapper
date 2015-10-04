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
	
	initialiseBasicVariables();
	
#ifdef KM_EDITOR_APP
	buildBasicMenu();
#endif
	
#ifdef KM_LOG_INSTANCIATIONS
	ofLogNotice("basicShape::basicShape") << " [" << &*this << "]";
#endif
}

basicShape::~basicShape(){
	initialized = false;

#ifdef KM_EDITOR_APP
	groupID.removeListener(this, &basicShape::groupIDUpdated);
	
	disableEditMode();
	
	position.unbindShape();
	
#endif

#ifdef KM_LOG_INSTANCIATIONS
	ofLogNotice("basicShape::~basicShape") << (string)typeid(this).name();
#endif
}

// - - - - - - -
// MAIN FUNCTIONS
// - - - - - - -
void basicShape::initialiseBasicVariables(){
	// set position
	position = basicPoint( ofGetWidth()/2, ofGetHeight()/2 );
	boundingBox = ofRectangle(0,0,0,0);
	groupID = -1;
	hasError = false;
	
	myShapeTypes.clear();
	myShapeTypes.push_back("basicShape");

#ifdef KM_EDITOR_APP
	position.setShape( *this );
	
	bEditMode = false;
	activeHandle = NULL;
	
	maxGroupID = 5;
	
	prevMaxGroupID = maxGroupID;
	
	fgColor = ofColor(255, 210);
	bgColor = ofColor(0);
	
	drawShapeGui = false;
	
	// name our variables
	groupID.set(GUIinfo_GroupID, -1, -1, maxGroupID);
	groupID.addListener(this, &basicShape::groupIDUpdated);
	
	shapeName.set(GUIinfo_ShapeName, "");
	
	pleaseDeleteMe = false;
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
	
#ifdef KM_EDITOR_APP
	// update GUI Toggle ?
	if( isInEditMode() ){
		guiToggle.setPosition( boundingBox.getTopRight()+5 );
	}
#endif
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
#ifdef KM_EDITOR_APP
	position.setPos( xml.getValue("X", 0), xml.getValue("Y", 0));
#else
	position.x = xml.getValue("X", 0);
	position.y = xml.getValue("Y", 0);
#endif
	xml.popTag(); // pop position
	
	groupID = xml.getValue("groupID", -1);
#ifdef KM_EDITOR_APP
	setColorFromGroupID();
#endif
	
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
	return myShapeTypes.back();
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
	return (std::find( myShapeTypes.begin(), myShapeTypes.end(), _type) != myShapeTypes.end());
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
		
		// draw additional shape gui.
		if( drawShapeGui ) shapeGui.draw();
	}
	
	// reset
	ofPopStyle();
}

void basicShape::buildBasicMenu(){
	// build custom UI
	
	basicShapeGui.clear();
	basicShapeGui.setup( getShapeType() );
	basicShapeGui.setShowHeader(true);
	
	basicShapeGui.add( (new ofxIntSlider( groupID )) );
	
	basicShapeGui.add( (new ofxLabel( shapeName )) );
	
	shapeGui.setup();
	shapeGui.add( &basicShapeGui );
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
			drawShapeGui = !drawShapeGui;
			shapeGui.setPosition( boundingBox.getTopRight()+ofVec2f(5,20));
			return true;
		}
		
		// over shapeGui ?
		else if(drawShapeGui && shapeGui.getShape().inside(e.x,e.y)){
			// if click on name, then ask for new name... #hacky
			ofxBaseGui* nameLabel = basicShapeGui.getControl(GUIinfo_ShapeName);
			if(nameLabel && nameLabel->getShape().inside(e.x, e.y) ){
				shapeName = ofSystemTextBoxDialog("Enter the shape name...", shapeName);
				ofLogNotice("basicShape::interceptMouseClick") << "New shape name: " << shapeName;
			}
			
			return true;
		}
		
		// on position handler ?
		else if( position.interceptMouseClick(e) ){
			
			// erase whole shape ?
			if( ofGetKeyPressed('r') || ofGetKeyPressed('R') ){
				pleaseDeleteMe = true;
			}
			
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

bool basicShape::setGroupID(const int& _id){
	groupID = _id;
	return true;
}

void basicShape::groupIDUpdated(int& val){
	// keep new maxGroupID ?
	if(maxGroupID <= groupID){
		maxGroupID = groupID+1;
	}
	
	setColorFromGroupID();
}

// endif KM_EDITOR_APP
#endif


//
// SHAPE FACTORY
//

// Bind with factory
namespace shape
{
	basicShape* create(const std::string& name){
		//std::cout << "create() --> " << name << std::endl;
		factory::shapeRegistry& reg = factory::getShapeRegistry();
		factory::shapeRegistry::iterator it = reg.find(name);
		
		if (it == reg.end()) {
			// This happens when there is no shape registered to this name.
			ofLogError("basicShape* shape::create") << "Shapetype not found: " << name;
			return nullptr;
		}
		
		factory::CreateShapeFunc func = it->second;
		return func();
	}
	
	void destroy(const basicShape* comp){
		delete comp;
	}
	
	vector< std::string > getAllShapeTypes() {
		factory::shapeRegistry& reg = factory::getShapeRegistry();
		vector< std::string > ret;
		ret.clear();
		for( auto it=reg.begin(); it != reg.end(); ++it){
			ret.push_back( it->first );
		}
		return ret;
	}
} // namespace shape

// register shape type
SHAPE_REGISTER( basicShape , "basicShape" );


