#ifdef KM_EDITOR_APP

#pragma once

#include "KMSettings.h"

#include "ofMain.h"
#include "shapes.h"
#include "shapesEditor.h"

// note:
// first we load shapes which are attached to effects. Destroy basicEffect instances before destroying basicShape instances; unless coding the complicated way.

class ofAppEditor : public ofBaseApp {

	public:
		ofAppEditor();
		~ofAppEditor();
	
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
	
	protected:
		
	
private:
	shapesEditor editor;
	
};

#endif // KM_EDITOR_APP
