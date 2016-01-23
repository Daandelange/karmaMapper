//
//  karmaModule
//  karmaMapper
//
//  Created by Daan de Lange on 21/01/2016.
//
//	An abstract class for making karmaMapper modules
//

#pragma once

#include "ofxXmlSettings.h"
#include "animationParams.h"

class karmaModule {
	
public:
	karmaModule( );
	~karmaModule();
	
	// MAIN FUNCTIONALITY
	virtual void update(const animationParams& params);
	virtual void draw(const animationParams& params);
	
	// UTILITIES
	string getName() const;
	bool isType(const string _type) const;
	string getType() const;
	
	virtual void drawMenu();
	virtual bool saveToXML(ofxXmlSettings& xml) const;
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
protected:
	string moduleType;
	string moduleUID;
	string moduleName; // must stay unique
	//effectParams params;
	
	bool bHasError;
	bool bEnabled;
	bool bInitialised;
	bool bIsLoading;
	bool bShowGuiWindow;
	
	
private:
	
	
};


