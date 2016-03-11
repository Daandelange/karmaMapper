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
	
	shortStatus = "I'm fine! ;)\n That is my status.";
	
	// do specific stuff here
	// eg connect with database, setup a physical device, etc.
	// could this be multi threaded ?
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
	bHasError = false;
	
	return bInitialised;
}

// Usefull ?
bool basicEffect::initialise(){
	return true;
}

// todo: update -(handled by)-> animation
// returns true if rendering succeeded
// only swap the FBo if you also transfer the pixels
bool basicEffect::render(karmaFboLayer& renderLayer, const animationParams& params){
	ofScopedLock lock(effectMutex);
	if( !isReady() ) return false;
	
	// only swap if you need access to the previous FBO's source
	//renderLayer.swap(); // ping-pong!
	
	renderLayer.begin();
	
	// draw bounding box
	ofSetColor( 255,0,0 );
	ofNoFill();
	if(overallBoundingBox.width > 0) ofDrawRectangle( overallBoundingBox );
	
	// by default, basicEffect uses the shape's default rendering mode
	for(int i=0; i<shapes.size(); i++){
		shapes[i]->sendToGPU();
	}
	
	renderLayer.end();
	
	return true;
}

// like ofApp::update()
// todo: update should receive parameters like update rate, time variables, etc.
// todo: this should always be called in fact. imageGrainEffect::update() should be called by it.
void basicEffect::update(karmaFboLayer& renderLayer, const animationParams& params){
	ofScopedLock lock(effectMutex);
	
	if( !isReady() ) return;
	aliveSince = ofGetSystemTime() - startTime;
}

// Usefull ?
//void basicEffect::update(){
//	
//}

// resets all values
void basicEffect::reset(){
	ofScopedLock lock(effectMutex);
	
	// todo: do this in _reset() which then calls reset();
	aliveSince=0;
	startTime=ofGetSystemTime();

	shapes.clear();
	shapes.resize(0);
	
	bInitialised = true;
	bHasError = false;
	bEnabled = false;
	bIsLoading = false;
	bShowGuiWindow = false;
	effectName = effectType;
	bUsePingpong = false;
	
	overallBoundingBox = ofRectangle(0,0,0,0);
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
	
	ImGui::SetNextWindowSize(ImVec2(400,ofGetHeight()*0.8), ImGuiSetCond_Once);
	ImGui::Begin( ((string)"Effect Settings: ").append(getName()).append("###effect-").append( ofToString(this) ).c_str() , &bShowGuiWindow );
	
	ImGui::LabelText("Type", "%s", getType().c_str() );
	static char nameBuffer[32] = "";
	if( ImGui::InputText("Name", nameBuffer, 32) ){
		effectName = nameBuffer;
	}
	ImGui::LabelText("Status", "%s", getShortStatus().c_str() );
	if( ImGui::Button("Reset effect") ){
		reset();
	}
	if (!ImGui::IsItemActive()){
		memcpy(nameBuffer, effectName.c_str(), effectName.size() );
	}
	if( ImGui::Checkbox("Enabled", &bEnabled) ){
		bEnabled?enable():disable();
	}
	ImGui::SameLine(-150);
	ImGui::LabelText((isLoading()?"(Loading...)":"(Loaded)" ), "");
	ImGui::SameLine(-50);
	ImGui::LabelText((bHasError?"(Has Error)":"(No Errors)" ), "");
	
	ImGui::Spacing();
	ImGui::Spacing();
	
	if (ImGui::CollapsingHeader( GUIBoundShapesTitle, "GUIBoundShapesTitle", true, true)){
		
		ImGui::PushID("shapeBindings");
		
		ImGui::TextWrapped("Bound to %i shapes", getNumShapes());
		
		ImGui::Spacing();
		
		ImGui::SameLine();
		if( ImGui::Button("Bind to shape group...") ){
				ImGui::OpenPopup("batchBindShapes");
		}
		if( ImGui::BeginPopup("batchBindShapes") ){
			ImGui::SameLine();
			
			//ImGui::Text( "" );
			//ImGui::Separator();
			
			if( ImGui::Selectable( "All shapes", false) ){
				int newBoundShapes = 0;
				for(auto s=_scene.getShapesItConstBegin(); s!=_scene.getShapesItConstEnd(); ++s){
					if( !isBoundWithShape(*s) ){
						if( bindWithShape(*s) ) newBoundShapes++;
					}
				}
				// toggle selection ?
				if( newBoundShapes==0 ){
					detachFromAllShapes();
				}
			}
			
			else {
				auto groups = _scene.getAllShapesByGroup();
				for (auto g=groups.cbegin(); g!=groups.cend(); ++g){
					if( ImGui::Selectable( ofToString(g->first).c_str(), false) ){
						for( auto s=g->second.cbegin(); s!=g->second.cend(); ++s ){
								bindWithShape(*s);
						}
					}
				}
			}
		
			ImGui::EndPopup();
		}
		
		ImGui::SameLine();
		if (ImGui::Button("Unbind All")){
			detachFromAllShapes();
		}
		
		// list shapes
		ImGui::ListBoxHeader("shapeBindings");
		if(_scene.getNumShapes()<1) ImGui::Selectable("<None Available>", false);
		else for (auto s=_scene.getShapesItConstBegin(); s!=_scene.getShapesItConstEnd(); ++s){
			bool tmpSelected = (bool)(std::find(shapes.cbegin(), shapes.cend(), *s) !=shapes.cend() );
			
			if( ImGui::Selectable( (*s)->getName().c_str(), &tmpSelected ) ){
				if (tmpSelected) bindWithShape( *s );
				else detachFromShape( *s );
			};
		}
		ImGui::ListBoxFooter();
		
		ImGui::PopID(); // pop boundShapes
		
		ImGui::Spacing();
	}
	
	ImGui::Spacing();
	ImGui::Spacing();
	
	printCustomEffectGui();
	
	// spacing
	ImGui::Text(" ");
	
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
	xml.addValue("enabled", bEnabled );
	xml.addValue("effectIndex", effectIndex);
	xml.addValue("showGuiWindow", bShowGuiWindow);
	
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
	xml.getValue("enabled", false )?enable():disable();
	effectIndex = xml.getValue("effectIndex", 0);
	bShowGuiWindow = xml.getValue("showGuiWindow", false);

	return true; // todo
}


// - - - - - - -
// EFFECT PROPERTIES
// - - - - - - -
	
bool basicEffect::isReady() const {
	return bInitialised && !bHasError && !bIsLoading && bEnabled;
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

string basicEffect::getType() const {
	return effectType;
}

const string basicEffect::getShortStatus() const {
	return shortStatus;
}

bool basicEffect::setIndex(const int &_index){
	effectIndex = _index;
	return true;
}

const int& basicEffect::getIndex() const {
	return effectIndex;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool basicEffect::randomizePresets(){
	return true;
}

// formats URLS for getting files within the effect folder itself
string basicEffect::effectFolder(string _file) const{
	return ofToDataPath( ((string)"effects/").append(getType()).append("/").append(_file));
}

bool basicEffect::usesPingPong() const {
	return bUsePingpong;
}

bool basicEffect::setUsePingPong(const bool& _usePingpong){
	bUsePingpong = _usePingpong;
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

bool basicEffect::isBoundWithShape(basicShape *_shape) const {
	return (std::find(shapes.cbegin(), shapes.cend(), _shape) != shapes.cend() );
}

bool basicEffect::detachFromAllShapes(){
	if(shapes.size()==0) return true;
	
	// loop trough shapes
	for(int i=shapes.size()-1; i>=0; i--){
		shapes.erase(shapes.begin()+i);
	}
	
	return (shapes.size()==0); // should always be true
}

// always returns true, simply unbinds 
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
