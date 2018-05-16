#include "KMSettings.h"
#include "ofMain.h"

#ifdef KM_EDITOR_APP
#include "ofAppEditor.h"
#else
#include "ofApp.h"
#endif

#if (OF_VERSION_MINOR < 9) && defined(TARGET_OPENGLES)
#include "ofGLProgrammableRenderer.h"
#endif

//========================================================================
int main( ){
#ifdef KARMAMAPPER_DEBUG
	// Useful for debugging shaders and other inner-OF things
	ofSetLogLevel(OF_LOG_VERBOSE);
	//ofSetLogLevel("ofThread", OF_LOG_ERROR);
#else
	ofSetLogLevel(OF_LOG_SILENT);
#endif
	
#ifdef TARGET_OPENGLES
	
#if (OF_VERSION_MINOR >= 9)
	ofGLESWindowSettings s;
	s.setSize(700, 700);
	s.setGLESVersion(2);
	ofCreateWindow(s);
#else
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(700, 700, OF_WINDOW);
#endif
	
#else
	ofGLWindowSettings s;
	//s.setGLVersion(2,1); // needed for box2d//liquidfun textured particles
    s.setGLVersion(3,2);
    //s.setGLVersion(4,1);
	ofCreateWindow(s);
#endif
	
	//ofAppGlutWindow window;
	//window.setGlutDisplayString("rgba double depth alpha samples>=4");
	//ofSetupOpenGL(&window, 1500,1200,OF_WINDOW);			// <-------- setup the GL context
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
#ifdef KM_EDITOR_APP
	ofRunApp(new ofAppEditor());
#else
	ofRunApp(new ofApp());
#endif
	
}
