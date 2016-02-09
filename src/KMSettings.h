#pragma once

// needed for using OF constants
#include "ofMain.h"

#define KM_VERSION "0.2+ alpha (master)"

// - - - - - -
// ENABLE / DISABLE some features on compilation time ?
// - - - - - -
#ifdef TARGET_OSX
	// comment line to force-disable
	#define KM_ENABLE_SYPHON
#endif


// - - - - - -
// helpful for debugging
// - - - - - -
#define KARMAMAPPER_DEBUG true

// define to see debug instantiation order
//#define KM_LOG_INSTANCIATIONS true

// compiles some additional warnings that might help you locate code problems
#define KM_LOCATE_PROBLEMS true

#ifdef TARGET_OSX
	#define KM_CTRL_KEY_NAME "CMD"
	#define KM_CTRL_KEY_CODE OF_KEY_COMMAND
#else
	#define KM_CTRL_KEY_NAME "CTRL"
	#define KM_CTRL_KEY_CODE OF_KEY_CONTROL
#endif

// - - - - - - - -
// SAVE & LOAD...
// Shapes
#define KM_SCENE_SAVE_FILE "saveFiles/karmaMapperSceneSettings.xml"
#define KM_SCENE_SAVE_PATH "saveFiles/scenes/"
#define KM_DEFAULT_SCENE "defaultScene.xml"
// Effect configurations
#define KM_LAST_CONFIG_FILE "saveFiles/lastUsedConfiguration.xml"
#define KM_CONFIG_FOLDER "saveFiles/configurations/"
#define KM_CONFIG_DEFAULT "defaultConfig.xml"


