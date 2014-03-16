//
//  illuminatiTriangle.cpp
//  illuminatiVisualiser
//
//  Created by Daan de Lange on 24/2/14.
//
//

// todo: dynamic effects loading

#include "illuminatiTriangle.h"

illuminatiTriangle::illuminatiTriangle(){
	cout << "illuminatiTriangle::illuminatiTriangle() - hey!\n" << endl;
	
	disableAllEvents();
	enableMouseEvents();
	enableAppEvents();
	
	isInEditMode = false;
	
	// set position
	set( ofGetWidth()/2, ofGetHeight()/2 , 100, 100);
	
	// spawn 4 points within this triangle.
	float handlerSize = 20.0f;
	
	for(int i=0; i<4; i++){
		movablePoint* p = &points[i];
		
		if(i==3) p->setPosition(x-handlerSize/2, y-handlerSize/2); // center point
		else p->setPosition(x-handlerSize/2+cos(ofDegToRad(120*i))*ofGetHeight()/8, y-handlerSize/2+sin(ofDegToRad(120*i))*ofGetHeight()/8); // corner points
		
		//p->height = handlerSize;
		//p->width = handlerSize;
		p->setSize( handlerSize, handlerSize );
		
		//cout << p->position << " • " << p->width << "*" << p->height << endl;
	}
	
	initEffects();
}

illuminatiTriangle::~illuminatiTriangle(){
	disableAllEvents();
}

// WARNING, this function is not called as I disableAllEvents();
void illuminatiTriangle::setup() {
	//cout << "illuminatiTriangle::setup() - hello!\n" << endl;
}
	
	
void illuminatiTriangle::exit() {
	
}
	
	
void illuminatiTriangle::update() {
	//cout << "illuminatiTriangle::update() - Update cycle done" << endl;
	
	for(int i=0; i<numPoints; i++){
		movablePoint* p = &points[i];
		//cout << p->getPosition() << endl;
	}
}
	
	
void illuminatiTriangle::draw() {
	//cout << "illuminatiTriangle::draw() - OK" << endl;
	ofPushStyle();
	
	ofSetColor(125,255,255);
	//ofFill();
	//ofRect(x, y, width, height);
	
	drawWireFrame();
	
	ofPopStyle();
	
	// triggers effects to animate & render
	animateEffects();
}

void illuminatiTriangle::drawWireFrame(){
	if(!isInEditMode) return;
	
	movablePoint* p = points;
	// draw points
	p = &points[0];
	ofPoint pts[4];
	for(int i=0; i<4; i++){
		//p->draw();
		//p = &points[i];
		pts[i] = p->getPos();
		p++;
	}
	
	//ofPushStyle();
	
	// draw lines
	ofSetColor(255,0,0);
	ofNoFill();
	
	//cout << pts[0] << "-" << pts[1] << endl;
	ofLine(pts[0],pts[3]);
	ofLine(pts[1],pts[3]);
	ofLine(pts[2],pts[3]);
	
	ofSetColor(0,255,0);
	ofLine(pts[0],pts[1]);
	ofLine(pts[1],pts[2]);
	ofLine(pts[2],pts[0]);
	
	//ofPopStyle();
}

void illuminatiTriangle::enableEditMode(){
	isInEditMode = true;
	updateEditModeToChildren();
}

void illuminatiTriangle::disableEditMode(){
	isInEditMode = false;
	updateEditModeToChildren();
	
	// not really the right place to do this; but it does the thing
	updateEffectPoints();
}

void illuminatiTriangle::switchEditMode(){
	isInEditMode?disableEditMode():enableEditMode();
}

void illuminatiTriangle::updateEditModeToChildren(){
	movablePoint* p = points;

	// draw points
	//p = &points[0];
	for(int i=0; i<numPoints; i++){
		p = &points[i];
		p->setEditable(this->isInEditMode);
	}
}

void illuminatiTriangle::initEffects(){
	edgeTransitioner.spawn();
	
	pEmitter.spawn();
	
	imgAnim1.spawn();
	imgAnim1.loadFolder("animations/imageMapper/a1");
	
	imgAnim2.spawn();
	imgAnim2.loadFolder("animations/imageMapper/a2");
}

void illuminatiTriangle::animateEffects(){
	if(isInEditMode) return;
	
	pEmitter.animate();
	
	if(a1) imgAnim1.animate();
	else imgAnim2.animate();
	
	edgeTransitioner.animate();
}

void illuminatiTriangle::updateEffectPoints(){
	movablePoint* p = points;
	// draw points
	p = &points[0];
	ofPoint pts[4];
	for(int i=0; i<4; i++){
		//p->draw();
		//p = &points[i];
		pts[i] = p->getPos();
		p++;
	}
	
	imgAnim1.setPoints(pts);
	imgAnim2.setPoints(pts);
	edgeTransitioner.setPoints(pts);
	pEmitter.setPoints(pts);
}

void illuminatiTriangle::updateData(float _eq){
	//equalizer = _eq;
}
	
void illuminatiTriangle::onRollOver(int x, int y) {
	//printf("illuminatiTriangle::onRollOver(x: %i, y: %i)\n", x, y);
}
	
void illuminatiTriangle::onRollOut() {
	//printf("illuminatiTriangle::onRollOut()\n");
}
	
void illuminatiTriangle::onMouseMove(int x, int y){
	//printf("illuminatiTriangle::onMouseMove(x: %i, y: %i)\n", x, y);
}
	
void illuminatiTriangle::onDragOver(int x, int y, int button) {
	//printf("illuminatiTriangle::onDragOver(x: %i, y: %i, button: %i)\n", x, y, button);
}
	
void illuminatiTriangle::onDragOutside(int x, int y, int button) {
	//printf("illuminatiTriangle::onDragOutside(x: %i, y: %i, button: %i)\n", x, y, button);
}
	
void illuminatiTriangle::onPress(int x, int y, int button) {
	if(button==0){
		switchEditMode();
	}
	//printf("illuminatiTriangle::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
}
	
void illuminatiTriangle::onRelease(int x, int y, int button) {
	//printf("illuminatiTriangle::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
}
	
void illuminatiTriangle::onReleaseOutside(int x, int y, int button) {
	//printf("illuminatiTriangle::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
}
	
void illuminatiTriangle::keyPressed(int key) {
	//printf("illuminatiTriangle::keyPressed(key: %i)\n", key);
}
	
void illuminatiTriangle::keyReleased(int key) {
	//printf("illuminatiTriangle::keyReleased(key: %i)\n", key);
}

