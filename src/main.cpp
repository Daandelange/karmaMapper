#include "ofMain.h"
#include "karmaMapper.h"

#ifdef TARGET_OPENGLES
#include "ofGLProgrammableRenderer.h"
#include "ofAppGlutWindow.h"
#else
#include "ofGLProgrammableRenderer.h"
#include "ofAppGLFWWindow.h"
#endif

//========================================================================
int main( ){
	//ofSetLogLevel(OF_LOG_VERBOSE);
	
#ifdef TARGET_OPENGLES
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
#endif
	
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	
	//ofAppGlutWindow window;
	ofAppGLFWWindow window;
	window.enableSetupScreen();
	
	ofSetupOpenGL(&window, 1024,768,OF_WINDOW);	// <-------- setup the GL context
	//ofSetupOpenGL(1024,768,OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new karmaMapper() );

}
