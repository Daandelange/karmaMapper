#pragma once

#include "ofMain.h"

#include "shapes.h"
#include "shapesDB.h"
#include "animationController.h"
#include "effects.h"
//#include "ofxAbletonLiveSet.h"
//#include "karmaSoundAnalyser.h"
#include "OSCRouter.h"

// note:
// first we load shapes which are attached to effects. Destroy basicEffect instances before destroying basicShape instances; unless coding the complicated way.

// todo: esc key ask for confirm on exit
// todo: make the recorder a separate tool/class

class ofApp : public ofBaseApp{

	public:
		ofApp();
		~ofApp();
	
		void setup();
		void update();
		void draw();
		void exit();
	
		void audioIn(float *input, int bufferSize, int nChannels);
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
	protected:
		
	
private:
	//ofx::AbletonLiveSet::EventHandler liveSetEvents;
	shapesDB scene;
	animationController controller;
	//karmaSoundAnalyser analyser;
	//ofSoundStream soundStream;
	OSCRouter osc;
	
	bool mouseHidden = true;
	
};
