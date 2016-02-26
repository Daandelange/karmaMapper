//
//  distortEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Applies on vertexShape
//	Distorts it's vertexes

#include "distortEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

distortEffect::distortEffect(){
	
	distortEffect::reset();
	
	// effect type must match with class
	effectType = "distortEffect";
}

distortEffect::~distortEffect(){
	ofRemoveListener(mirReceiver::mirTempoEvent, this, &distortEffect::tempoEventListener);
}

// - - - - - - -
// GLOBAL EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
// This function is called before it starts rendering.
// Rendering without having called this function should not be possible.
// isReady() should return true after this is done. (can take a long time)
bool distortEffect::initialise(const animationParams& params){
	// init values
	basicEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	// do stuff
	reset();
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
	
	return bInitialised;
}

bool distortEffect::render(const animationParams &params){
	// everything happens in the update!
	//return;
	
	// tmp
	for(std::vector<basicShape*>::iterator it=shapes.begin(); it!=shapes.end(); ++it){
		// todo: basicEffect should have a nice downcast function like .asType<vertexShape>()
		if( (*it)->isType("vertexShape") ){
//			if(params.seasons.summer > 0 ){
//				//ofEnableAlphaBlending();
//				ofSetColor( ofFloatColor(1.f, params.seasons.summer*.1f));
//				ofFill();
//				
//			   (*it)->sendToGPU();
//			}
//			ofSetColor( ofFloatColor(1.f, 1));
//			ofFill();
			
//			ofPushMatrix();
//			ofTranslate((*it)->getPositionPtr()->x, (*it)->getPositionPtr()->y);
//			list<basicPoint>& pts = ((vertexShape*)(*it))->getPoints();
//			for(auto it=pts.begin(); it!=pts.end(); ++it){
//				ofDrawRectangle((*it).x, (*it).y, 2, 2);
//			}
//			ofPopMatrix();
		}
	}
	
	return true;
}

// updates shape data
void distortEffect::update(const animationParams& params){
	
	if(!isReady()) return;
	
	// do basic Effect function
	basicEffect::update( params );
	
	ofScopedLock lock(effectMutex);
	
	if(shapes.size()<1) return;
	
	int offset = 0;
	for(auto it=shapes.begin(); it!=shapes.end(); it++){
		if( !(*it)->isReady() ) continue;
		
		if( (*it)->isType("vertexShape") ){
			
			// todo: basicEffect should have a nice downcast getter function like .asType<vertexShape>()
			list<basicPoint>& pts = ((vertexShape*)(*it))->getPoints(POINT_POSITION_RELATIVE);
		
			if( bVariateInSeason ){
				
				for(auto itb=pts.begin(); itb!=pts.end(); ++itb){
					(*itb) += basicPoint(
						( sin( params.elapsedTime *(TWO_PI+(offset/3%4)))*25 + cos( params.elapsedTime* (TWO_PI+(offset/5%8)/4))*25) * (params.seasons[seasonVariation] *0.9f + 0.05f),
						( cos( params.elapsedTime *(TWO_PI+(offset/4%7)))*25 - sin( params.elapsedTime* (TWO_PI+(offset/8%3)))*25) * (params.seasons[seasonVariation]*0.9f + 0.05f)
					);
					offset++;
				}
				
			}
			else if( bReactToBpm && BPMCurrentMagnitude>0 ){
				for(auto itb=pts.begin(); itb!=pts.end(); ++itb){
					(*itb) *= 1+0.1*BPMCurrentMagnitude;
				}
				
				BPMCurrentMagnitude -= 0.01f; // todo: make this time-based ?
			}
			
			(*it)->onShapeModified();
		}
	}
}

// resets all values
void distortEffect::reset(){
	basicEffect::reset();
	
	bVariateInSeason = false;
	seasonVariation = 1;
	bReactToBpm = true;
	BPMMetronom = 1;
	BPMCurrentMagnitude = 0;
	
	ofRemoveListener(mirReceiver::mirTempoEvent, this, &distortEffect::tempoEventListener);
	ofAddListener(mirReceiver::mirTempoEvent, this, &distortEffect::tempoEventListener);
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool distortEffect::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUIDistortPanel, "GUIDistortPanel", true, true ) ){
		
		ImGui::TextWrapped("This effect alters the points of a vertex shape.");
		
		ImGui::Checkbox("Move in seasons.", &bVariateInSeason );
		if(bVariateInSeason){
			ImGui::SliderInt("Season #", &seasonVariation, 0, 3);
		}
		ImGui::Checkbox("React to BPM.", &bReactToBpm );
		if(bReactToBpm){
			ImGui::DragInt("BPM time mesure", &BPMMetronom, 1);
			//BPMCurrentMagnitude
			ImGui::DragFloat("BPM Magnitude", &BPMMagnitude, 0.05);
			
		}
	}
	
	return true;
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool distortEffect::saveToXML(ofxXmlSettings& xml) const{
	bool ret = basicEffect::saveToXML(xml);
	
	xml.addValue("VariateInSeason", bVariateInSeason);
	xml.addValue("SeasonNb", seasonVariation);
	
	xml.addValue("ReactToBPM", bReactToBpm);
	xml.addValue("BPMMetronom", BPMMetronom);
	xml.addValue("BPMMagnitude", BPMMagnitude);
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool distortEffect::loadFromXML(ofxXmlSettings& xml){
	bool ret = basicEffect::loadFromXML(xml);
	
	bVariateInSeason = xml.getValue("VariateInSeason", false);
	seasonVariation = xml.getValue("SeasonNb", 0);
	
	bReactToBpm = xml.getValue("ReactToBPM", true);
	BPMMetronom = xml.getValue("BPMMetronom", 1);
	BPMMagnitude = xml.getValue("BPMMagnitude", 1);
	BPMCurrentMagnitude = 0;
	
	return ret;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool distortEffect::randomizePresets(){
	if(!basicEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}

void distortEffect::tempoEventListener(mirTempoEventArgs &_args){
	ofScopedLock( effectMutex );
	if(BPMCurrentMagnitude < BPMMetronom*-1.f){
		BPMCurrentMagnitude = BPMMagnitude;
	}
	else BPMCurrentMagnitude -= 1.f;
}

// register effect type
EFFECT_REGISTER( distortEffect , "distortEffect" );
