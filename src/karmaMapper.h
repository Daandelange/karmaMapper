#pragma once

#include "ofMain.h"
#include "realWorldInterface.h"
#include "karmaSoundAnalyser.h"

class karmaMapper : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	void audioReceived (float * input, int bufferSize, int nChannels);
	
private:
	realWorldInterface rwi;
	karmaSoundAnalyser soundAnalyser;
};
