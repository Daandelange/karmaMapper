#pragma once

#include "ofMain.h"
#include "illuminatiTriangle.h"
//#include "movablePoint.h"
#include "ofxEasyFft.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
private:
	void analyseMusic();
	
	vector<illuminatiTriangle*> triangles;
	bool debug;
	int numTriangles;
	
	bool shiftIsPressed;
	
	// OF FFT
	int bufferSize, nbEqBands;
	ofxEasyFft fft;
	vector<float> equalizer;
	void musicalLine(vector<float>& buffer, ofVec3f intensity, ofVec3f start, ofVec3f end);
};
