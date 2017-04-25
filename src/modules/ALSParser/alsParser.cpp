//
//  alsParser.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 24/03/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "alsParser.h"

// forward declarations are needed for event listening
//ofEvent<alsParserTrackEventArgs> alsParser::alsTrackEvent;
//ofEvent<alsParserTempoEventArgs> alsParser::alsParserTempoEvent;
//ofEvent<alsParserNoteEventArgs> alsParser::alsParserNoteEvent;
//ofEvent<alsParserFloatEventArgs> alsParser::alsParserFloatEvent;

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
alsParser::alsParser() {
	
	// karmaModule setup
	bInitialised = true;
	bEnabled = false;
	moduleName = "alsParser";
	moduleType = "alsParser";
	
	alsFilePath = "";
	bEnableNoteEvents = false;
	bEnableTrackEvents = false;
}

alsParser::~alsParser(){
	disable();
}


// - - - - - - - -
// VIRTUALS FROM karmaModule
// - - - - - - - -
bool alsParser::enable(){
	bool ret = karmaModule::enable();
	
	bHasError = ret;
	
	if(bEnableTrackEvents){
		eventHandler.enableTrackEvents(LS);
	}
	
	return ret;
}

bool alsParser::disable(){
	bool ret = karmaModule::disable();
	
	return ret;
}

void alsParser::update(const animationParams &params){
	karmaModule::update(params);
}

void alsParser::draw(const animationParams &params){
	karmaModule::draw(params);
}

bool alsParser::reset(){
	
	return parseALSFile();
}

void alsParser::showGuiWindow(){
	if(!bShowGuiWindow) return;
	
	ImGui::SetNextWindowSize(ImVec2(400,ofGetHeight()*0.8), ImGuiSetCond_Once);
	ImGui::Begin( ((string)"Module: ").append(karmaModule::getName()).append("###module-").append( ofToString(this) ).c_str() , &bShowGuiWindow );
	ImGui::TextWrapped("Ableton Live sets (.als) are GZIPed XML files from which this module extracts data.");
	
	ImGui::Separator();
	
	if( ImGui::CollapsingHeader( ".als Params", "alsParserParams", true, true ) ){

		ImGui::TextWrapped(".als file: %s", alsFilePath.c_str());
		
		static bool bShowFileDialog = false;
		if (ImGui::Button("Choose File...")){
			bShowFileDialog = true;
		}
		if(bShowFileDialog){
			ofFileDialogResult openFileResult= ofSystemLoadDialog("Select the ALS file to load...", false, ofToDataPath( ofToDataPath("")));
			
			if(openFileResult.bSuccess){
				// restrict to saveFiles dir
				ofFile file( ofToDataPath( openFileResult.getPath()) );
				if(file.exists() && ofToLower(file.getExtension()) == "als"){

					alsFilePath = ofToDataPath( openFileResult.getPath() );
					if( parseALSFile() ){
						// OK
					}
					else {
						// failed loading
						alsFilePath = LS.loadedFile;
					}
				}
			}
			bShowFileDialog = false;
		}
		
		ImGui::SameLine();
		if(ImGui::Button("(re)LOAD .als")){
			parseALSFile();
		}
		
		ImGui::Separator();
		
		if(LS.isLoaded()){
			if( ImGui::CollapsingHeader( "Show track information", "alsParserTrackInfo", true, false ) ){
				// todo: list track info
			}
		}
	}
	
	ImGui::Separator();
	
	if( ImGui::CollapsingHeader( "Event Params", "alsParserEventParams", true, true ) ){
		
		if(ImGui::Checkbox("Enable Audio Track Events", &bEnableTrackEvents)){
			
		}
		if(ImGui::Checkbox("Enable Note Events", &bEnableNoteEvents)){
			
		}
	}
	
	ImGui::End();
}

void alsParser::drawMenuEntry() {
	karmaModule::drawMenuEntry();
}

// writes the module data to XML. xml's cursor is already pushed into the right <module> tag.
bool alsParser::saveToXML(ofxXmlSettings& xml) const{
    bool ret = karmaModule::saveToXML(xml);
	
	xml.addValue("alsParserALSFilePath", alsFilePath);
    xml.addValue("alsParserbEnableNoteEvents", bEnableNoteEvents);
    xml.addValue("alsParserbEnableTrackEvents", bEnableTrackEvents);
    
    return ret;
}

// load module settings from xml
// xml's cursor is pushed to the root of the <module> tag to load
bool alsParser::loadFromXML(ofxXmlSettings& xml){
    
    bool ret=karmaModule::loadFromXML(xml);
	
	alsFilePath = xml.getValue("alsParserALSFilePath", "");
	bEnableNoteEvents = xml.getValue("alsParserbEnableNoteEvents", bEnableNoteEvents );
	bEnableTrackEvents = xml.getValue("alsParserbEnableTrackEvents", bEnableTrackEvents );
	parseALSFile();
    
    return ret;
}

bool alsParser::parseALSFile(){
	
	ofxAbletonLiveSet::Parser parser(LS);
	
	if( !parser.open(alsFilePath) ){
#ifdef KARMAMAPPER_DEBUG
		ofLogNotice("animationController::guiEvent") << "Unknown GUI event: " << name << endl;
#endif
		return false;
	}
	else {
		return true;
	}
}


//alsParserAnalysisData alsParser::alsParserAnalysis;// = mirData();

const static ::module::factory::moduleDependencies  alsParserDependencies({});
MODULE_REGISTER( alsParser , "alsParser", alsParserDependencies );
