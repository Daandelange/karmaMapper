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
#include "ofxImGui.h"
#include "moduleFactory.h"

class karmaModule {
	
public:
	karmaModule( );
	~karmaModule();
	
	// MAIN FUNCTIONALITY
	bool setEnabled(const bool& _enabled);
	virtual bool enable();
	virtual bool disable();
	virtual void update(const animationParams& params);
	virtual void draw(const animationParams& params);
	
	// UTILITIES
	string getName() const;
	bool isType(const string _type) const;
	string getType() const;
	bool isEnabled() const;
	
	virtual void showGuiWindow();
	virtual void drawMenuEntry();
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

#define GUIModuleInfoTitle "Module Information"
#define GUIModulesList "Loaded modules"


namespace module
{
	karmaModule* create(const std::string& name);
	void destroy(const karmaModule* comp);
	vector< std::string > getAllModuleTypes();
}

// allow module registration
#define MODULE_REGISTER(TYPE, NAME)                                        \
namespace module {                                                         \
namespace factory {                                                       \
namespace                                                                 \
{                                                                         \
template<class T>                                                         \
class moduleRegistration;                                                  \
\
template<>                                                                \
class moduleRegistration<TYPE>                                             \
{                                                                         \
static const ::module::factory::RegistryEntry<TYPE>& reg;                  \
};                                                                        \
\
const ::module::factory::RegistryEntry<TYPE>&                              \
moduleRegistration<TYPE>::reg =                                            \
::module::factory	::RegistryEntry<TYPE>::Instance(NAME);                \
}}}

