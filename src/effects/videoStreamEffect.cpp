//
//  videoStreamEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Bla bla bla
//

#include "videoStreamEffect.h"

videoStreamEffect::videoStreamEffect(){
	basicEffect::basicEffect();
	
	// effectType must match the class name
	effectType = "videoStreamEffect";

}

videoStreamEffect::~videoStreamEffect(){
	basicEffect::~basicEffect();
	
	ofRemoveListener(ofx::AbletonLiveSet::EventHandler::noteEvent, this, &videoStreamEffect::noteEventListener);
}

// update --> animation
// overrule this function with your own
bool videoStreamEffect::render(){
	if( shapes.size()==0 || currentStream < 0 ) return;
	

	ofSetColor(255);
	ofFill();
	
	ofTexture texture = grabber.getTexture();
	
	int i=0;
	for(vector<basicShape*>::iterator it=shapes.begin(); it!=shapes.end(); it++, i++){
		//(*it)->render();
		
		ofPushMatrix();
		ofTranslate( *(*it)->getPositionPtr() );
		
		// calc scale
		bool coverMode = true;
		ofRectangle s = (*it)->getBoundingBox();
		float scaleH = s.height/grabber.getHeight();
		float scaleW = s.width/grabber.getWidth();
		float scale = 1.f;
		
		ofVec2f offset(0);
		if(!coverMode){
			if(scaleH > scaleW) scale = scaleW;
			else scale = scaleH;
		}
		else{
			if(scaleW > scaleH){
				scale = scaleW;
				offset.y = (s.height-grabber.getHeight())/2;
			}
			else{
				scale = scaleH;
				offset.y = (s.width-grabber.getWidth())/2;
			}
		}
		
		ofScale(scale, scale);
		// draw
		//ofDrawRectangle(-grabber.width/2+offset.x,-grabber.height/2, grabber.width-offset.x, grabber.height);
		//grabber.draw(-grabber.width/2,-grabber.height/2, grabber.width, grabber.height);
		texture.drawSubsection( -s.getWidth()/2, -s.getHeight()/2, s.getWidth(), s.getHeight(), offset.x, offset.y );
		
		
		ofPopMatrix();
	}
	
}

void videoStreamEffect::update(){
	// you can chose to do the default behaviour too or not by commenting this out
	basicEffect::update();
	
	if( currentStream >= 0 ) grabber.update();
}

// resets all values
// overrule this function with your own.
void videoStreamEffect::reset(){
	basicEffect::reset();
}

bool videoStreamEffect::grabShape(){
	// clear
	shapes.clear();
	shapes.resize(0);
	
	// we just want all vertex shapes
	//shapes = shapesHandler::getInstance().getShapesByType("vertexShape");
	return true;
	// and look for a shapeType id
	for(vector<basicShape*>::reverse_iterator it=shapes.rbegin(); it!=shapes.rend(); it++){
		
		if( (*it)->getGroupID() != 2 ){
			it = vector<basicShape*>::reverse_iterator( shapes.erase( it.base() - 1) );
			it--;
		}
	}
	
	return shapes.size() > 0;
}

bool videoStreamEffect::grabStream(){
	
	//we can now get back a list of devices.
	vector<ofVideoDevice> devices = grabber.listDevices();
	
	for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName;
		if( devices[i].bAvailable ){
			cout << endl;
		}else{
			cout << " - unavailable " << endl;
		}
	}
	
	grabber.setDeviceID(0);
	grabber.setDesiredFrameRate(60);
	currentStream = grabber.setup(640,460) ? 0 : -1;
	
	//videoInverted.allocate(camWidth,camHeight,OF_PIXELS_RGB);
	//videoTexture.allocate(videoInverted);
	//ofSetVerticalSync(true);
}

void videoStreamEffect::noteEventListener(ofx::AbletonLiveSet::LSNoteEvent &noteEvent){
	cout << "ok!" << endl;
}

