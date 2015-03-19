#include "ofMain.h"
#include "ofApp.h"
//#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
	ofSetupOpenGL(700, 700, OF_WINDOW);			// <-------- setup the GL context
	//ofAppGlutWindow window;
	//window.setGlutDisplayString("rgba double depth alpha samples>=4");
	//ofSetupOpenGL(&window, 1500,1200,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
	
}