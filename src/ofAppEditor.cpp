#ifdef KM_EDITOR_APP

#include "ofAppEditor.h"

//--------------------------------------------------------------
ofAppEditor::ofAppEditor() {
	
#ifdef KM_LOG_INSTANCIATIONS
	ofLogNotice("ofAppEditor::ofAppEditor()");
#endif
}

ofAppEditor::~ofAppEditor(){
	
}

void ofAppEditor::setup(){
	
	// configure app
	ofSetEscapeQuitsApp(false);
	ofSetWindowTitle( "karmaMapper::Editor - For your own security and way beyond..." );
	
	// load scene
	//editor.loadScene("Vendome_1500_1200.xml"); // tmp
	editor.loadScene("Vendome_Full_Small.xml"); // tmp
	//editor.loadLastUsedScene(); // tmp
	
	editor.enableEditing();
	
}

//--------------------------------------------------------------
void ofAppEditor::update(){
	
}

//--------------------------------------------------------------
void ofAppEditor::draw(){
	
}

void ofAppEditor::exit(){

}

void ofAppEditor::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofAppEditor::keyReleased(int key){

}

//--------------------------------------------------------------
void ofAppEditor::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofAppEditor::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofAppEditor::mousePressed(int x, int y, int button){
	//ofVec2f tmp(ofGetWidth(), ofGetHeight());
	//createShape( );
}

//--------------------------------------------------------------
void ofAppEditor::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofAppEditor::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofAppEditor::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofAppEditor::dragEvent(ofDragInfo dragInfo){ 

}

#endif // KM_EDITOR_APP
