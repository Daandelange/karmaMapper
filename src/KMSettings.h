#pragma once

// needed for using OF constants
#include "ofMain.h"

#define KM_VERSION "0.2+ alpha (master)"

// since of 0.9.4, we'll jump to glm later
//#define OF_USE_LEGACY_MESH 1

// - - - - - -
// ENABLE / DISABLE some features on compilation time ?
// - - - - - -
#ifdef TARGET_OSX

        // for the moment Syphon only compiles on XCODE....
#if defined(KM_XCODE_COMPILER) || defined(KM_QT_CREATOR)

	// comment line to force-disable
	//#define KM_ENABLE_SYPHON

	#ifdef KM_QT_CREATOR
                #define KM_ENABLE_SYPHON
		//#pragma message "Enabling Syphon. On Qt Creator you'll need to copy Syphon.framework to /Library/Frameworks. Also verify your git branch depending on your window type and OF version."
	#else
		#define KM_ENABLE_SYPHON
		#pragma message "Enabling Syphon. Make sure you use OSX + XCode to compile. Also verify your git branch depending on your window type and OF version."
	#endif

#endif
#endif


// - - - - - -
// helpful for debugging
// - - - - - -
//
#define KARMAMAPPER_DEBUG true

#ifdef KARMAMAPPER_DEBUG
	// compiles some additional warnings that might help you locate code problems
	#define KM_LOCATE_PROBLEMS true

	// define to see debug instantiation order
	//#define KM_LOG_INSTANCIATIONS true
#endif

#ifdef TARGET_OSX
	#define KM_CTRL_KEY_NAME "CMD"
	#define KM_CTRL_KEY_CODE OF_KEY_COMMAND
#else
	#define KM_CTRL_KEY_NAME "CTRL"
	#define KM_CTRL_KEY_CODE OF_KEY_CONTROL
#endif

// - - - - - - - -
// SAVE & LOAD...
#define KM_DIRECTORY_SEPARATOR "/"
// Shapes
#define KM_SCENE_SAVE_FILE "saveFiles/karmaMapperSceneSettings.xml"
#define KM_SCENE_SAVE_PATH "saveFiles/scenes/"
#define KM_DEFAULT_SCENE "defaultScene.xml"
// Effect configurations
#define KM_LAST_CONFIG_FILE "saveFiles/lastUsedConfiguration.xml"
#define KM_CONFIG_FOLDER "saveFiles/configurations/"
#define KM_CONFIG_DEFAULT "defaultConfig.xml"


