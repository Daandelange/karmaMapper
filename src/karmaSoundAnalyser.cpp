//
//  karmaSoundAnalyser.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 17/03/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "karmaSoundAnalyser.h"

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
karmaSoundAnalyser::karmaSoundAnalyser( ){
	bEnabled = false;
	
}

karmaSoundAnalyser::~karmaSoundAnalyser(){
	
}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
bool karmaSoundAnalyser::start(){
	bEnabled = true;
	
	return isEnabled()==true;
}

bool karmaSoundAnalyser::stop(){
	
	
	
	bEnabled = false;
	return isEnabled()==false;
}

bool karmaSoundAnalyser::isEnabled() const {
	return bEnabled;
}

// - - - - - - - -
// EVENT LISTENERS
// - - - - - - - -


