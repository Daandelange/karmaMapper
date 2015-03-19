#pragma once

#include "ofMain.h"

#include "shapes.h"
#include "shapesServer.h"
#include "shapesEditor.h"
#include "animationController.h"
#include "effects.h"
#include "ofxAbletonLiveSet.h"
#include "karmaSoundAnalyser.h"

// note:
// first we load shapes which are attached to effects. Destroy basicEffect instances before destroying basicShape instances; unless coding the complicated way.

// todo: esc key ask for confirm on exit

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
		ofImage background;
	
private:
	ofx::AbletonLiveSet::EventHandler liveSetEvents;
	ofSoundPlayer music;
	shapesServer server;
	shapesEditor* editor;
	animationController controller;
	karmaSoundAnalyser analyser;
	ofSoundStream soundStream;
	
	bool mouseHidden = true;
	
	// recording
	//ofImage fboHack;
	bool recordOutput = false;
	ofFbo recorder;
	GLuint fbo;
	
	bool recordOutputBis = false;
	//ofImage recordingImage;
	//ofxQtVideoSaver saver;
};
