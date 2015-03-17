//
//  karmaSoundAnalyser.h
//  karmaMapper
//
//  Created by Daan de Lange on 17/03/2015.
//
//
//

#pragma once

#include "ofMain.h"

class karmaSoundAnalyser {
	
public:
	karmaSoundAnalyser();
	~karmaSoundAnalyser();
	
	// basic functions
	bool start();
	bool stop();
	bool isEnabled() const;
	
	// listeners
	//void draw(ofEventArgs& event);
	
protected:
	
	
private:
	bool bEnabled;
	
};
