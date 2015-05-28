#include "ofMain.h"
#include "ofApp.h"
//#include "ofAppGlutWindow.h"
#ifdef TARGET_OPENGLES
#include "ofGLProgrammableRenderer.h"
#endif

#define KARMAMAPPER_DEBUG true

//========================================================================
int main( ){
	// Useful for debugging shaders and other inner-OF things
	//ofSetLogLevel(OF_LOG_VERBOSE);
	
	#ifdef TARGET_OPENGLES
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(700, 700, OF_WINDOW);// <-------- setup the GL context
	
	#else
	ofGLWindowSettings s;
	s.setGLVersion(3,3);
	//s.setGLVersion(4,3);
	ofCreateWindow(s);
	#endif
	
	//ofAppGlutWindow window;
	//window.setGlutDisplayString("rgba double depth alpha samples>=4");
	//ofSetupOpenGL(&window, 1500,1200,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
	
}