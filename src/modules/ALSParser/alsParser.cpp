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
	bEnableAbletonLinkSyncing = true;
	bEnableNoteEvents = false;
	bEnableTrackEvents = false;
	bEnableMetronomEvents = false;
	eventTimeDelay = 0;
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
	
	parseEventsFromLS();
	
	eventHandler.setEventFireDelay(eventTimeDelay);
	
	return ret;
}

bool alsParser::disable(){
	bool ret = karmaModule::disable();
	
	return ret;
}

void alsParser::update(const animationParams &params){
	karmaModule::update(params);
	
	if(isEnabled()){
		if(bEnableNoteEvents || bEnableTrackEvents){
			eventHandler.timerTick( eventHandler.getAbletonElapsedTime() );
		}
	}
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
						parseEventsFromLS();
						
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
		
		ImGui::SameLine();
		ImGui::TextWrapped((LS.isLoaded())?"(loaded)":"(not loaded)");
		
		ImGui::Separator();
		
		if(LS.isLoaded()){
			if( ImGui::CollapsingHeader( "Show track information", "alsParserTrackInfo", true, false ) ){
				// todo: list track info
				ImGui::TextWrapped("Name: %s", LS.name.c_str());
				ImGui::TextWrapped("Creator: %s", LS.userName.c_str());
				ImGui::TextWrapped("Locators: %lu", LS.locators.size());
				ImGui::TextWrapped("Midi Tracks: %lu", LS.miditracks.size());
				ImGui::TextWrapped("Audio Tracks: %lu", LS.audiotracks.size());
			
				ImGui::Separator();
				
				if (ImGui::ListBoxHeader("Notes", 15)){
					
					for(auto t=LS.miditracks.begin(); t!=LS.miditracks.end(); ++t){
						
						if (ImGui::TreeNode(t->name.c_str())){
							//ImGui::Selectable(t->name.c_str(), false);
							//ImGui::Indent();
							
							for(auto c=t->clips.begin(); c!=t->clips.end(); ++c){
								
								if(ImGui::TreeNode(c->name.c_str())){
									//ImGui::Indent();
								
									for(auto n=c->notes.begin(); n!=c->notes.end(); ++n){
										ImGui::Selectable(ofToString(n->key).append(" @ ").append(ofToString(n->time)).c_str(), false);
									}
									
									//ImGui::Unindent();
									ImGui::TreePop();
								}
							}
						
							ImGui::TreePop();
							//ImGui::Unindent();
						}
					}
					
					ImGui::ListBoxFooter();
				}
				
				ImGui::Separator();
				
				if (ImGui::ListBoxHeader("Tracks", 15)){
					
					for(auto t=LS.audiotracks.begin(); t!=LS.audiotracks.end(); ++t){
						
						if (ImGui::TreeNode(ofToString(t->name).append(" (").append(ofToString(t->clips.size())).append("clips)").c_str())){
							
							int id = 0;
							for(auto c=t->clips.begin(); c!=t->clips.end(); ++c, ++id){
								ImGui::PushID(ofToString(id).c_str());
								ImGui::Selectable( ofToString(c->name).append(" @ ").append(ofToString(c->time)).append("sec [").append(ofToString(c->duration)).append("]").c_str(), false);
								ImGui::PopID();
							}
							
							ImGui::TreePop();
						}
					}
					
					ImGui::ListBoxFooter();
				}
				
				ImGui::Separator();
			}
		}
	}
	
	ImGui::Separator();
	
	if( ImGui::CollapsingHeader( "Event Params", "alsParserEventParams", true, true ) ){
		
		float phase = abletonLink::getInstance().ALStatus.phase;
		ImGui::SliderFloat("Link Phase", &phase, 0, abletonLink::getInstance().getQuantum());
		ImGui::TextWrapped("Link Timeline Seconds: %f", abletonLink::getInstance().getAbletonElapsedTimeSec());
		ImGui::TextWrapped("Link Timeline Beats: %f", abletonLink::getInstance().getAbletonElapsedBeats());
		ImGui::TextWrapped("Link Timeline tempo: %f", abletonLink::getInstance().getTempo());
		
		if(ImGui::Checkbox("Sync with Live Timeline (using midi clock)", &bEnableAbletonLinkSyncing)){
			// apply
			if(bEnableAbletonLinkSyncing) eventHandler.enableSyncWithLive();
			else eventHandler.disableSyncWithLive();
		}
		if(ImGui::SliderInt("Event time delay compensation", &eventTimeDelay, 0, 2000)){
			eventHandler.setEventFireDelay(eventTimeDelay);
		}
		
		if(ImGui::Checkbox("Enable Audio Track Events", &bEnableTrackEvents)){
			if(bEnableTrackEvents){
				eventHandler.enableTrackEvents();
			}
			else {
				//eventHandler.disableTrackEvents();
			}
		}
		if(ImGui::Checkbox("Enable Note Events", &bEnableNoteEvents)){
			if(bEnableNoteEvents){
				eventHandler.enableNoteEvents();
			}
			else {
				//eventHandler.disableTrackEvents();
			}
		}
		if(ImGui::Checkbox("Enable Metronom Events", &bEnableMetronomEvents)){
			if(bEnableMetronomEvents){
				eventHandler.enableMetronomEvents();
			}
			else {
				//eventHandler.disableTrackEvents();
			}
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
	xml.addValue("alsParserbEnableMetronomEvents", bEnableMetronomEvents);
	xml.addValue("alsbEnableAbletonLinkSyncing", bEnableAbletonLinkSyncing);
	xml.addValue("alsEventTimeDelay", eventTimeDelay );
	
    return ret;
}

// load module settings from xml
// xml's cursor is pushed to the root of the <module> tag to load
bool alsParser::loadFromXML(ofxXmlSettings& xml){
    
    bool ret=karmaModule::loadFromXML(xml);
	
	alsFilePath = xml.getValue("alsParserALSFilePath", "");
	parseALSFile();
	
	eventTimeDelay = xml.getValue("alsEventTimeDelay", eventTimeDelay);
	eventHandler.setEventFireDelay(eventTimeDelay);
	
	bEnableNoteEvents = xml.getValue("alsParserbEnableNoteEvents", bEnableNoteEvents );
	bEnableTrackEvents = xml.getValue("alsParserbEnableTrackEvents", bEnableTrackEvents );
	bEnableMetronomEvents = xml.getValue("alsParserbEnableMetronomEvents", bEnableMetronomEvents );
	
	if(bEnableTrackEvents) eventHandler.enableTrackEvents();
	if(bEnableNoteEvents) eventHandler.enableNoteEvents();
	if(bEnableMetronomEvents) eventHandler.enableMetronomEvents();
	
	bEnableAbletonLinkSyncing = xml.getValue("alsbEnableAbletonLinkSyncing", bEnableAbletonLinkSyncing);
	if(bEnableAbletonLinkSyncing) eventHandler.enableSyncWithLive();
	else eventHandler.disableSyncWithLive();
	
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

void alsParser::parseEventsFromLS(){
	if(bEnableAbletonLinkSyncing){
		eventHandler.enableSyncWithLive();
	}
	else {
		eventHandler.disableSyncWithLive();
	}
	
	if(bEnableTrackEvents){
		eventHandler.enableTrackEvents(LS);
	}
	if(bEnableMetronomEvents){
		eventHandler.enableMetronomEvents(LS);
	}
	if(bEnableNoteEvents){
		eventHandler.enableNoteEvents(LS);
	}
}

//alsParserAnalysisData alsParser::alsParserAnalysis;// = mirData();

const static ::module::factory::moduleDependencies  alsParserDependencies({"abletonLink"});
MODULE_REGISTER( alsParser , "alsParser", alsParserDependencies );
