//
//  basicEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

#include "basicEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

basicEffect::basicEffect(){
	
	basicEffect::reset();
	
	// effect type must match with class
	effectType = "basicEffect";
}

basicEffect::~basicEffect(){
	detachFromAllShapes();
}

// - - - - - - -
// GLOBAL EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
// This function is called before it starts rendering.
// Rendering without having called this function should not be possible.
// isReady() should return true after this is done. (can take a long time)
bool basicEffect::initialise(const animationParams& params){
	
	basicEffect::reset();
	
	bIsLoading = true;
	
	// do specific stuff here
	// eg connect with database, setup a physical device, etc.
	// could this be multi threaded ?
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
	
	return bInitialised;
}

// Usefull ?
bool basicEffect::initialise(){

}

// todo: update -(handled by)-> animation
// returns true if rendering succeeded
bool basicEffect::render(const animationParams& params){
	if( !isReady() ) return false;
	
	// draw bounding box
	ofSetColor( 255,0,0 );
	ofNoFill();
	if(overallBoundingBox.width > 0) ofDrawRectangle( overallBoundingBox );
	
	// by default, basicEffect uses the shape's default rendering mode
	for(int i=0; i<shapes.size(); i++){
		shapes[i]->sendToGPU();
	}
	
	return true;
}

// like ofApp::update()
// todo: update should receive parameters like update rate, time variables, etc.
// todo: this should always be called in fact. imageGrainEffect::update() should be called by it.
void basicEffect::update(const animationParams& params){
	ofScopedLock lock(effectMutex);
	
	if( !isReady() ) return;
	aliveSince = ofGetSystemTime() - startTime;
}

// Usefull ?
void basicEffect::update(){
	
}

// resets all values
void basicEffect::reset(){
	effectMutex.lock();
	
	
	// todo: do this in _reset() which then calls reset();
	aliveSince=0;
	startTime=ofGetSystemTime();

	shapes.clear();
	shapes.resize(0);
	
	bInitialised = false;
	bHasError = false;
	bEnabled = false;
	bIsLoading = false;
	bShowGuiWindow = false;
	effectName = effectType;
	
	overallBoundingBox = ofRectangle(0,0,0,0);
	
	effectMutex.unlock();
}

void basicEffect::enable(){
	bEnabled=true;
}

void basicEffect::disable(){
	bEnabled=false;
}

void basicEffect::setShowGuiWindow(const bool &_b){
	bShowGuiWindow = _b;
}

void basicEffect::toggleGuiWindow() {
	setShowGuiWindow( !bShowGuiWindow );
}

// ImGui is ready to be used, just place your Gui elements.
bool basicEffect::showGuiWindow( const shapesDB& _scene ) {
	if( !bShowGuiWindow ) return false;
	
	ImGui::Begin( ((string)"Effect Settings: ").append(getName()).c_str() , &bShowGuiWindow );
	
	ImGui::LabelText("Type", "%s", getType().c_str() );
	static char nameBuffer[32] = "";
	if( ImGui::InputText("Name", nameBuffer, 32) ){
		effectName = nameBuffer;
	}
	else if (!ImGui::IsItemActive()){
		memcpy(nameBuffer, effectName.c_str(), effectName.size() );
	}
	
	ImGui::Spacing();
	ImGui::Spacing();
	
	if (ImGui::CollapsingHeader( GUIBoundShapesTitle, "GUIBoundShapesTitle", true, true)){
		
		ImGui::TextWrapped("Bound to %i shapes", getNumShapes());
		
		ImGui::Spacing();
		
		if (ImGui::Button("Bind to shape...")){
			// todo
		}
		if (ImGui::IsItemHovered()) ImGui::SetTooltip("todo");
		ImGui::SameLine();
		if( ImGui::Button("Bind to shape group...") ){
			// todo
			
		}
		if (ImGui::IsItemHovered()) ImGui::SetTooltip("todo");
		
		// list shapes
		ImGui::ListBoxHeader("Bound Shapes");
		if(_scene.getNumShapes()<1) ImGui::Selectable("<None Available>", false);
		else for (auto s=_scene.getShapesItConstBegin(); s!=_scene.getShapesItConstEnd(); ++s){
			bool tmpSelected = (bool)(std::find(shapes.cbegin(), shapes.cend(), *s) !=shapes.cend() );
			
			if( ImGui::Selectable( (*s)->getName().c_str(), &tmpSelected ) ){
				if (tmpSelected) bindWithShape( *s );
				else detachFromShape( *s );
			};
		}
		ImGui::ListBoxFooter();
		
		ImGui::Spacing();
	}
	
	ImGui::Spacing();
	ImGui::Spacing();
	
	printCustomEffectGui();
	
	ImGui::End();
	
	return true;
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool basicEffect::saveToXML(ofxXmlSettings& xml) const{
	
	xml.addValue("effectType", effectType );
	xml.addValue("effectName", getName() );
	
	// remember bound shapes
	xml.addTag("boundShapes");
	xml.pushTag("boundShapes");
	int s = 0;
	for(auto it=shapes.cbegin(); it!=shapes.cend(); ++it){
		
		xml.addValue("shape", (*it)->getName() );
		xml.addAttribute("shape", "type", (*it)->getShapeType(), s);
		xml.addAttribute("shape", "name", (*it)->getName(), s);
		
		s++;
	}
	xml.popTag(); // pop boundShapes
	
	//xml.addValue("groupID", getGroupID() );
	//xml.addValue("shapeName", shapeName );
	
	return true;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool basicEffect::loadFromXML(ofxXmlSettings& xml){
	
	effectName = xml.getValue("effectName", getType() );
	
	//initialise(animationParams.params);
	
	return true; // todo
}


// - - - - - - -
// EFFECT PROPERTIES
// - - - - - - -
	
bool basicEffect::isReady() const {
	return bInitialised && !bHasError && !bIsLoading;
}

bool basicEffect::isLoading() const {
	return bInitialised && bIsLoading;
}

string basicEffect::getName() const {
	return effectName;
}

bool basicEffect::isType(const string _type) const {
	return _type==effectType;
}

string basicEffect::getType() const{
	return effectType;
}

string basicEffect::shortStatus(){
	return "I'm fine! ;)";
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool basicEffect::randomizePresets(){
	return true;
}

void basicEffect::updateBoundingBox(){
	// no shapes ?
	if(shapes.size()==0){
		// todo correctly
		// return center if no shapes are selected
		overallBoundingBox = ofRectangle( ofGetWidth()/2, ofGetHeight()/2, 0, 0);
		return;
	}
	
	// cache variables
	ofVec2f LTCorner( ofGetWidth(), ofGetHeight() );
	ofVec2f RBCorner( 0, 0);
	
	// analyse all contained boundingBoxes
	for(int i=shapes.size()-1; i>=0; i--){
		if( !shapes[i]->isReady() ) continue;
		
		// cache needed vars
		ofVec2f tmpLTCorner( shapes[i]->getBoundingBox().x, shapes[i]->getBoundingBox().y);
		ofVec2f tmpRBCorner( shapes[i]->getBoundingBox().x+shapes[i]->getBoundingBox().width, shapes[i]->getBoundingBox().y+shapes[i]->getBoundingBox().height);
		
		if(tmpLTCorner.x < LTCorner.x) LTCorner.x = tmpLTCorner.x;
		if(tmpLTCorner.y < LTCorner.y) LTCorner.y = tmpLTCorner.y;
		if(tmpRBCorner.x < RBCorner.x) RBCorner.x = tmpRBCorner.x;
		if(tmpRBCorner.y < RBCorner.y) RBCorner.y = tmpRBCorner.y;
	}
	
	overallBoundingBox = ofRectangle( LTCorner.x, LTCorner.y-LTCorner.x, RBCorner.x, RBCorner.y-LTCorner.y);
	
}

// - - - - - - -
// SHAPE BINDING FUNCTIONS
// - - - - - - -

bool basicEffect::bindWithShape(basicShape* _shape){
	if(_shape==NULL) return false;
	
	// prevent adding the same shape several times
	for(auto it=shapes.begin(); it!=shapes.end(); ++it){
		if( _shape == *it ) return true;  // already exists
	}
	
	//shapes.push_back( _shape );
	shapes.insert(shapes.end(), _shape);
	
	updateBoundingBox();
	
	return true;
}

// returns true if all given shapes are bound
bool basicEffect::bindWithShapes(vector<basicShape*> _shapes){
	if(_shapes.size()==0) return false;
	
	bool success = true;
	for(auto _shape=_shapes.begin(); _shape!=_shapes.end();	++_shape){
		
		success *= bindWithShape(*_shape);
	}

	updateBoundingBox();
	
	return success;
}

// copy of above function for list<>.
// todo: make this a templated function
bool basicEffect::bindWithShapes(list<basicShape *>& _shapes){
	if(_shapes.size()==0) return false;
	
	bool success = true;
	for(auto _shape=_shapes.cbegin(); _shape!=_shapes.cend();	++_shape){
		
		if( *_shape == NULL ){
			success = false;
			continue;
		}
		
		// prevent adding the same shape several times
		for(auto it=shapes.begin(); it!=shapes.end();	++it){
			if( *_shape == *it ) continue;  // already exists
		}
		
		//shapes.push_back( *_shape );
		shapes.insert(shapes.end(), *_shape);
	}
	
	updateBoundingBox();
	
	return success;
}

bool basicEffect::detachFromAllShapes(){
	if(shapes.size()==0) return true;
	
	// loop trough shapes
	for(int i=shapes.size()-1; i>=0; i--){
		shapes.erase(shapes.begin()+i);
	}
	
	return (shapes.size()==0); // should always be true
}

// always returns true, just unbinds
// todo: rename this to unBindWithShape() ?
bool basicEffect::detachFromShape(basicShape* _shape){
	if(shapes.size()==0) return true;
	
	if(_shape == nullptr || _shape == NULL) return true;
	
	// loop trough shapes
	for(auto it=shapes.begin(); it!=shapes.end(); ++it){
		if( (*it)!=NULL && _shape == (*it) ){
			shapes.erase(it);
			break;
		}
	}
	
	// no more shapes available for animation ? disable effect!
	if (shapes.size()==0) disable();
	
	return true;
}

int basicEffect::getNumShapes() const{
	return shapes.size();
}


// Bind with factory
namespace effect
{
	basicEffect* create(const std::string& name){
		//std::cout << "create() --> " << name << std::endl;
		factory::effectRegistry& reg = effect::factory::getEffectRegistry();
		effect::factory::effectRegistry::iterator it = reg.find(name);
		
		if (it == reg.end()) {
			// This happens when there is no effect registered to this name.
			ofLogError("basicEffect* effect::create") << "Effect type not found: " << name;
			return nullptr;
		}
		
		factory::CreateEffectFunc func = it->second;
		return func();
	}
	
	void destroy(const basicEffect* comp){
		delete comp;
	}
	
	vector< std::string > getAllEffectTypes() {
		factory::effectRegistry& reg = factory::getEffectRegistry();
		vector< std::string > ret;
		ret.clear();
		for( auto it=reg.begin(); it != reg.end(); ++it){
			ret.push_back( it->first );
		}
		return ret;
	}
} // namespace effect

// register effect type
EFFECT_REGISTER( basicEffect , "basicEffect" );
