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
	
	// do stuff
	ofEnableAlphaBlending();
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
	
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 8);
	fbo.begin();
	ofClear(0,0,0,0); // clear all, including alpha
	fbo.end();
	
	lines.clear();
	
	ofAddListener(mirReceiver::mirTempoEvent, this, &lineDrawEffect::tempoEventListener);
	
	return bInitialised;
}

bool lineDrawEffect::render(const animationParams &params){
	if(!isReady()) return false;
	
	fbo.begin();
	
	{	// erase BG ?
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		glBlendFunc(GL_ONE, GL_ONE);

		ofSetColor(0.0f, 3.0f* 1-(ofClamp( mirReceiver::mirCache.zcr-0.2f, 0, 1.0f )) );
		ofFill();
		ofDrawRectangle(0,0, fbo.getWidth(), fbo.getHeight());
		
		glDisable(GL_BLEND);
	}
	
	ofPushStyle();
	// set drawing environment
	ofSetColor( linesColor[0]*255, linesColor[1]*255, linesColor[2]*255, linesColor[3]*255);
	ofNoFill();
	
	// tmp
	//ofDrawRectangle(50,50, 200,200);
	
	// draw shape so GPU gets their vertex data
	for(auto it=lines.begin(); it!=lines.end(); ++it){
		(*it).render();
	}
	ofPopStyle();
	
	// flush the pipeline! :D
	fbo.end();
	
	// draw fbo
	fbo.draw(0,0);
	
	return true;
}

// updates shape data
void lineDrawEffect::update(const animationParams& params){
	
	// do basic Effect function
	basicEffect::update( params );
	
	// check for dead lines
	for(auto it=lines.rbegin(); it!= lines.rend(); it--){
		
		// remve dead ones
		if( !(*it).isAlive() ) {
			it++;
			it= std::list<lineDrawEffectLine>::reverse_iterator( lines.erase( it.base() ));
			it--;
		}
	}
}

// resets all values
void lineDrawEffect::reset(){
	basicEffect::reset();
	
	// effect type must match with class
	effectType = "lineDrawEffect";
	
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 8);
	fbo.begin();
	ofClear(0,0,0,0); // clear all, including alpha
	fbo.end();
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool lineDrawEffect::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUILineDrawEffectPanel, "GUILineDrawEffectPanel", true, true ) ){
		
		ImGui::TextWrapped("Draws lines on shapes on the rythm of music.");
		
		ImGui::Separator();
		
		ImGui::LabelText("Number of lines", "%u", 0 );
		ImGui::ColorEdit4("test", linesColor, true);
		//ImGui::Checkbox("React to mirTempoEvents");
		
		ImGui::Separator();
	}
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool lineDrawEffect::saveToXML(ofxXmlSettings& xml) const{
	bool ret = basicEffect::saveToXML(xml);
	
	xml.addTag("linesColor");
	if(xml.pushTag("linesColor")){
		xml.addValue("r", linesColor[0] );
		xml.addValue("g", linesColor[1] );
		xml.addValue("b", linesColor[2] );
		xml.addValue("a", linesColor[3] );
		xml.popTag();
	}
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool lineDrawEffect::loadFromXML(ofxXmlSettings& xml){
	bool ret = basicEffect::loadFromXML(xml);
	
	if(xml.pushTag("linesColor")){
		linesColor[0] = xml.getValue("r", 1.0f );
		linesColor[1] = xml.getValue("g", 1.0f );
		linesColor[2] = xml.getValue("b", 1.0f );
		linesColor[3] = xml.getValue("a", 1.0f );
		xml.popTag();
	}
	
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
	
	if(shapes.size()<=0) return;
	
	if(_args.isTempoBis) for(auto s=shapes.begin(); s!=shapes.end(); ++s){
		if( (*s)->isType("vertexShape") ){
			lines.push_back( lineDrawEffectLine( (vertexShape*)*s, (1.0f/(mirReceiver::mirCache.bpm/60.0f) )*4.0f ) );
		}
	}
}


// register effect type
EFFECT_REGISTER( lineDrawEffect , "lineDrawEffect" );
