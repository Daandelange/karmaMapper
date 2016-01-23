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

void karmaModule::drawMenu() {
	
}

// writes the module data to XML. xml's cursor is already pushed into the right <module> tag.
bool karmaModule::saveToXML(ofxXmlSettings& xml) const{
	
	xml.addValue("moduleType", moduleType );
	xml.addValue("moduleName", getName() );
	
//	// remember bound shapes
//	xml.addTag("boundShapes");
//	xml.pushTag("boundShapes");
//	int s = 0;
//	for(auto it=shapes.cbegin(); it!=shapes.cend(); ++it){
//		
//		xml.addValue("shape", (*it)->getName() );
//		xml.addAttribute("shape", "type", (*it)->getShapeType(), s);
//		xml.addAttribute("shape", "name", (*it)->getName(), s);
//		
//		s++;
//	}
//	xml.popTag(); // pop boundShapes
	
	return true;
}

// load module settings from xml
// xml's cursor is pushed to the root of the <module> tag to load
bool karmaModule::loadFromXML(ofxXmlSettings& xml){
	
	moduleName = xml.getValue("moduleName", getType() );
	
	//initialise(animationParams.params);
	
	return true; // todo
}