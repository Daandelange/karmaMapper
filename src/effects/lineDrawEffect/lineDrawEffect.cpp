//
//  lineDrawEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 10/10/15.
//
//  - - - -
//
//  Applies on vertexShape
//	Distorts it's vertexes

#include "lineDrawEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

lineDrawEffect::lineDrawEffect(){
	lineDrawEffect::reset();
}

lineDrawEffect::~lineDrawEffect(){
	ofRemoveListener(mirReceiver::mirTempoEvent, this, &lineDrawEffect::tempoEventListener);
	ofRemoveListener(liveGrabberOSC::liveGrabberNoteEvent, this, &lineDrawEffect::liveGrabberNoteEventListener);
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool lineDrawEffect::initialise(const animationParams& params){
	// init values
	basicEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	reset();
	
	return bInitialised;
}

bool lineDrawEffect::render(karmaFboLayer& renderLayer, const animationParams &params){
	if(!isReady()) return false;
	
	renderLayer.begin();
	
	// set drawing environment
	ofPushStyle();
	ofNoFill();
	
	// draw shape so GPU gets their vertex data
	for(auto it=lines.begin(); it!=lines.end(); ++it){
		(*it).render();
	}
	ofPopStyle();
	
	// flush the pipeline! :D
	renderLayer.end(false);

	// dirty fix for flipped texture when using double FBO
	//fbo.getTexture().texData.bFlipTexture = true;
	//fbo.getTexture().setTextureWrap( 1, -1);
	//ofGetCurrentRenderer().get()->isVFlipped() << endl;
	//ofGetCurrentRenderer().get()->setupGraphicDefaults();
	
	return true;
}

// updates shape data
void lineDrawEffect::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	// do basic Effect function
	basicEffect::update( renderLayer, params );
	
	if(bStressTestMode){
		if( params.fps >= fStressTestTargetFPS){
			fStressTestMultiplier += (params.idleTimeMillis/(1000.0/fStressTestTargetFPS))*fStressTestAddTolerance;
			//cout << fStressTestMultiplier << endl;
		}
		else {
			fStressTestMultiplier -= (fStressTestTargetFPS-params.fps)*fStressTestRemoveTolerance;
		}
		if(fStressTestMultiplier<0) fStressTestMultiplier=0;
		
			
		for(auto s=shapes.begin(); s!=shapes.end(); ++s){
			if( (*s)->isType("vertexShape") ){
				vertexShape* shape = (vertexShape*) *s;
				for(int i=0; i<fStressTestMultiplier; ++i){
					lines.push_back( lineDrawEffectLine( shape, fLineBeatDuration, ofColor(mainColor[0]*255, mainColor[1]*255,mainColor[2]*255, mainColor[3]*255) ) );
				}
			}
		}
	}
	
	ofScopedLock(oscMutex);
	
	// check for dead lines
	for(auto it=lines.rbegin(); it!=lines.rend(); it++){
		
		// remve dead ones
		if( !(*it).isAlive() ) {
			it = std::list<lineDrawEffectLine>::reverse_iterator( lines.erase( std::next(it).base() ));
			it = std::prev(it);
		}
	}
}

// resets all values
void lineDrawEffect::reset(){
	basicEffect::reset();
	
	// effect type must match with class
	effectType = "lineDrawEffect";
	
	fLineBeatDuration = 1.0;
	spawnAmount = 1;
	
	bReactToMusic = true;
	
	bStressTestMode = false;
	fStressTestMultiplier = 1;
	fStressTestAddTolerance = 0.01;
	fStressTestRemoveTolerance = 0.28;
	fStressTestTargetFPS = 60;
	
	// do stuff
	ofEnableAlphaBlending();
	
//	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 8);
//	fbo.begin();
//	ofClear(0,0,0,0); // clear all, including alpha
//	fbo.end();
	
	lines.clear();
	
	ofRemoveListener(mirReceiver::mirTempoEvent, this, &lineDrawEffect::tempoEventListener);
	ofAddListener(mirReceiver::mirTempoEvent, this, &lineDrawEffect::tempoEventListener);
	
	ofRemoveListener(liveGrabberOSC::liveGrabberNoteEvent, this, &lineDrawEffect::liveGrabberNoteEventListener);
	ofAddListener(liveGrabberOSC::liveGrabberNoteEvent, this, &lineDrawEffect::liveGrabberNoteEventListener);
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool lineDrawEffect::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUILineDrawEffectPanel, "GUILineDrawEffectPanel", true, true ) ){
		
		ImGui::TextWrapped("Draws lines on shapes in various ways.");
		ImGui::Separator();
		ImGui::LabelText("Number of lines", "%lu", lines.size() );
		ImGui::SliderInt("Spawn Amount", &spawnAmount, 0, 50);
		
		ImGui::Separator();
		ImGui::Separator();
		effectMutex.lock();
		if( ImGui::Checkbox("React to music", &bReactToMusic )){
			// check if MIR router is running
			if(bReactToMusic && mirReceiver::getInstance().isEnabled() ) bReactToMusic = false;
			// todo: else request mir receiver
		}
		effectMutex.unlock();
		if(ImGui::IsItemHovered()){
			ImGui::SetTooltip("Draws lines on shapes on the rythm of music. (uses aubioTempo)");
		}
		
		ImGui::Separator();
		ImGui::Separator();
		
		//ImGui::ColorEdit4("Color", linesColor, true);
		//ImGui::Checkbox("React to mirTempoEvents");
		ImGui::SliderFloat("Line Duration (in beats)", &fLineBeatDuration, 1, 10);
		
		ImGui::Separator();
		ImGui::Separator();
		ImGui::TextWrapped("Controls");
		
		bool spawnSomeLines = false;
		
		if (ImGui::Button("Spawn some lines")) {
			spawnSomeLines = true;
		}
		ImGui::SameLine();
		static bool spawnCheckbox = false;
		ImGui::Checkbox("Lock", &spawnCheckbox);
		
		if(isReady() && (spawnSomeLines || spawnCheckbox)){
			ofScopedLock(effectMutex);
			
			for(auto s=shapes.begin(); s!=shapes.end(); ++s){
				if( (*s)->isType("vertexShape") ){
					int duration = 2; // sec
					vertexShape* shape = (vertexShape*) *s;
					lines.push_back( lineDrawEffectLine( shape, fLineBeatDuration, ofColor(mainColor[0]*255, mainColor[1]*255,mainColor[2]*255, mainColor[3]*255) ));
				}
			}
		}
		
		ImGui::Separator();
		
		ImGui::Checkbox("Stress Test Mode", &bStressTestMode);
		if(bStressTestMode){
			ImGui::Indent();
			ImGui::InputInt("fStressTestTargetFPS", &fStressTestTargetFPS);
			ImGui::DragFloat("fStressTestMultiplier", &fStressTestMultiplier);
			ImGui::SliderFloat("fStressTestAddTolerance", &fStressTestAddTolerance, 0.01, 10);
			ImGui::SliderFloat("fStressTestRemoveTolerance", &fStressTestRemoveTolerance, 0.001, 10);
			ImGui::Unindent();
		}
	}

	return true;
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool lineDrawEffect::saveToXML(ofxXmlSettings& xml) const{
	bool ret = basicEffect::saveToXML(xml);
	
//	xml.addTag("linesColor");
//	if(xml.pushTag("linesColor")){
//		xml.addValue("r", linesColor[0] );
//		xml.addValue("g", linesColor[1] );
//		xml.addValue("b", linesColor[2] );
//		xml.addValue("a", linesColor[3] );
//		xml.popTag();
//	}
	xml.addValue("LineDuration", fLineBeatDuration);
	xml.addValue("spawnAmount", spawnAmount);
	
	xml.addValue("bReactToMusic", bReactToMusic);
	
	xml.addValue("bStressTestMode", bStressTestMode);
	xml.addValue("fStressTestMultiplier", fStressTestMultiplier);
	xml.addValue("fStressTestAddTolerance",fStressTestAddTolerance);
	xml.addValue("fStressTestRemoveTolerance",fStressTestRemoveTolerance);
	xml.addValue("fStressTestTargetFPS", fStressTestTargetFPS);
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool lineDrawEffect::loadFromXML(ofxXmlSettings& xml){
	bool ret = basicEffect::loadFromXML(xml);
	
	fLineBeatDuration = xml.getValue("LineDuration", 1.0f);
	spawnAmount = xml.getValue("spawnAmount", spawnAmount);
	
	bReactToMusic = xml.getValue("bReactToMusic", bReactToMusic);
	
	bStressTestMode = xml.getValue("bStressTestMode", bStressTestMode);
	fStressTestMultiplier = xml.getValue("fStressTestMultiplier", fStressTestMultiplier);
	fStressTestAddTolerance = xml.getValue("fStressTestAddTolerance",fStressTestAddTolerance);
	fStressTestRemoveTolerance = xml.getValue("fStressTestRemoveTolerance",fStressTestRemoveTolerance);
	fStressTestTargetFPS = xml.getValue("fStressTestTargetFPS", fStressTestTargetFPS);
	
	return ret;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool lineDrawEffect::randomizePresets(){
	if(!basicEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}

// - - - - -
// lineDrawEffect Methods
// - - - - -
void lineDrawEffect::tempoEventListener(mirTempoEventArgs &_args){
	ofScopedLock lock(effectMutex);
	
	if(!bReactToMusic) return;
	
	if(shapes.size()<=0) return;
	
	if(_args.isTempoBis) for(auto s=shapes.begin(); s!=shapes.end(); ++s){
		if( (*s)->isType("vertexShape") ){
			vertexShape* shape = (vertexShape*)*s;
			for(int i=0; i<spawnAmount; i++){
				lines.push_back( lineDrawEffectLine( shape, (1.0f/(mirReceiver::mirCache.bpm/60.0f) )*fLineBeatDuration, ofColor(mainColor[0]*255, mainColor[1]*255,mainColor[2]*255, mainColor[3]*255) ));
			}
		}
	}
}

void lineDrawEffect::liveGrabberNoteEventListener(liveGrabberNoteEventArgs &_args){
	ofScopedLock lock(effectMutex);
	
	if(!bReactToMusic) return;
	
	if(shapes.size()<=0) return;
	
	if(_args.key.compare("A0")==0){
		for(auto s=shapes.begin(); s!=shapes.end(); ++s){
			if( (*s)->isType("vertexShape") ){
				vertexShape* shape = (vertexShape*)*s;
				for(int i=0; i<spawnAmount; i++){
					lines.push_back( lineDrawEffectLine( shape, (1.0f/(mirReceiver::mirCache.bpm/60.0f) )*fLineBeatDuration, ofColor(mainColor[0]*255, mainColor[1]*255,mainColor[2]*255, mainColor[3]*255) ));
				}
			}
		}
	}
}

// register effect type
EFFECT_REGISTER( lineDrawEffect , "lineDrawEffect" );
