//
//  karmaModule.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 21/01/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "karmaModule.h"


// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
karmaModule::karmaModule( ){
	
}

karmaModule::~karmaModule(){
	
}

// - - - - - - - -
// MAIN FUNCTIONALITY
// - - - - - - - -
void karmaModule::draw(const animationParams& params){
	
}

void karmaModule::update(const animationParams& params){
	
}

// - - - - - - - -
// UTILITIES
// - - - - - - - -
string karmaModule::getName() const {
	return moduleName;
}

bool karmaModule::isType(const string _type) const {
	return _type==moduleType;
}

string karmaModule::getType() const{
	return moduleType;
}

void karmaModule::showGuiWindow(){
	if(!bShowGuiWindow) return;
	
	ImGui::SetNextWindowSize(ImVec2(400,ofGetHeight()*0.8));
	ImGui::Begin( ((string)"Module: ").append(getName()).append("###module-").append( ofToString(this) ).c_str() , &bShowGuiWindow );
	
	ImGui::End();
}

void karmaModule::drawMenuEntry() {
	if (ImGui::CollapsingHeader( getName().c_str(), ofToString(this).c_str(), true, true)){
		ImGui::TextWrapped("This is a standard module.");
	}
}

// writes the module data to XML. xml's cursor is already pushed into the right <module> tag.
bool karmaModule::saveToXML(ofxXmlSettings& xml) const{
	
	xml.addValue("moduleType", moduleType );
	xml.addValue("moduleName", getName() );
	
	return true;
}

// load module settings from xml
// xml's cursor is pushed to the root of the <module> tag to load
bool karmaModule::loadFromXML(ofxXmlSettings& xml){
	
	moduleName = xml.getValue("moduleName", getType() );
	
	//initialise(animationParams.params);
	
	return true; // todo
}

// Bind with factory
namespace module
{
	karmaModule* create(const std::string& name){
		//std::cout << "create() --> " << name << std::endl;
		factory::moduleRegistry& reg = factory::getModuleRegistry();
		factory::moduleRegistry::iterator it = reg.find(name);
		
		if (it == reg.end()) {
			// This happens when there is no effect registered to this name.
			ofLogError("basicEffect* effect::create") << "Effect type not found: " << name;
			return nullptr;
		}
		
		factory::CreateModuleFunc func = it->second;
		return func();
	}
	
	void destroy(const karmaModule* comp){
		delete comp;
	}
	
	vector< std::string > getAllModuleTypes() {
		factory::moduleRegistry& reg = factory::getModuleRegistry();
		vector< std::string > ret;
		ret.clear();
		for( auto it=reg.begin(); it != reg.end(); ++it){
			ret.push_back( it->first );
		}
		return ret;
	}
} // namespace effect

// register module type
MODULE_REGISTER( karmaModule , "karmaModule" );
