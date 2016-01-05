//
//  animationController.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/03/2015.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "animationController.h"
#include "ofxVLCRemote.h"
#include "durationRC.h"

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
animationController::animationController( shapesDB& _scene ): scene(_scene){
	bEnabled = false;
	bShowGui = false;
	
	bMouseHidden = false;
	bIsFullScreen = false;
	showShortcuts = false;
	
	effects.clear();
	effects.resize(0);
	
	ofAddListener( ofEvents().draw , this, &animationController::draw, OF_EVENT_ORDER_APP );
	ofAddListener( ofEvents().update , this, &animationController::update, OF_EVENT_ORDER_APP );
	
	ofAddListener( ofEvents().keyPressed, this, &animationController::_keyPressed, OF_EVENT_ORDER_APP );
	
	// build GUI
	gui.setup();
	ImGui::GetIO().MouseDrawCursor = false;
	
	// Setup Effects info
	//effectsMenu.setup( GUIEffectsTitle );
	//guiNumEffects.set( GUIEffectsNumEffects, "0" );
//	effectsMenu.add( new ofxLabel(guiNumEffects) );
//	bGuiShowAnimParams.set(GUIEffectsViewParams, false);
//	bGuiShowAnimParams.addListener(this, &animationController::showAnimationsGui);
//	effectsMenu.add( new ofxMinimalToggle(bGuiShowAnimParams) );
//	
//	animationGui.add( &effectsMenu );

	// hide gui
	bShowGui = false;
}

animationController::~animationController(){
	ofRemoveListener( ofEvents().draw , this, &animationController::draw );
	ofRemoveListener( ofEvents().update , this, &animationController::update );
	
	ofRemoveListener( ofEvents().keyPressed , this, &animationController::_keyPressed );
	
	// rm GUI stuff correctly
	//guiLoadShapesSceneBtn.removeListener(this, &animationController::showShapeLoadMenu);
	bShowGui = false;

}

// - - - - - - - -
// BASIC FUNCTIONS
// - - - - - - - -
bool animationController::start(){
	bEnabled = true;
	bShowGui = true;
	
	// all black! xD
	ofSetBackgroundAuto(false);
	ofClear(0,0,0,255);
	
	// syphon
#ifdef TARGET_OS_MAC
//	syphonTexture.setName("Texture Output");
#endif
	
	// play music
	//sound.loadSound("TEST MIX V0.1.wav");
	//music.load("music.wav");
	//music.setLoop(true);
	//music.play();
	
	// Setup video recording
	//recorder.startRecording("", 850, 700);
	
//	{
//		// play music with VLC
//		ofxVLCRemote vlc;
//		vlc.setup("/Users/daan/vlc.sock"); // client-specific... :(
//		vlc.run("repeat on");
//		vlc.run("normal"); // playback speed
//		vlc.run("volume 256"); // 0-1024 (256=normal)
//		
//		//vlc.run("adev 7"); // tell VLC to use device 7 (soundflower 2ch)
//		vlc.run("play");
//		vlc.run("prev");
//		//vlc.run("got0 0");
//		
//	}
//	
//	{	// SYNC DURATION
//		durationRC rc;
//		rc.setupOSC();
//		rc.rewindAndPlay();
//	}
	
	
	// setup the OSC Router
	oscRouter.start();
	
	// enable mirOSCReceiver
	mirOSCReceiver.start();
	oscRouter.addNode( &mirOSCReceiver );
	
	// enable durationOSCReceiver
	//durationOSCReceiver.start();
	//oscRouter.addNode( &durationOSCReceiver );
	
	// get shape instance
	//string type="basicEffect";
	//e = stringToShapeClass( type );
	
	// re-enable when uncommenting 2 lines above
	/*if(e==NULL){
	 ofLogError("--> Could not create an effect of type "+type);
	 return;
	 }*/
	
	/*
	if(false){	// VIDEO EFFECT
		basicEffect* e;
		e = new videoEffect();
		e->initialise();
		e->bindWithShape( scene.getRandomShapeByGroup(5) ); // 3 = bg
		effects.push_back(e);
	}
	
	//
	if(true){	// IMAGE EFFECT
		basicEffect* e;
		e = new imageEffect();
		e->initialise();
		((imageEffect*) e)->loadImageFromFile("vendome_full_ok.jpg");
		((imageEffect*) e)->listenToOscIntensity("bgVendomeOpacity");
		e->bindWithShape( scene.getRandomShapeByGroup(5) ); // 3 = blue windows
		
		effects.push_back(e);
	}
	
	// no mutexes --> unstable
	if(true){	// IMAGE BEAT EFFECT
		basicEffect* e;
		e = new imageBeatEffect();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(3) ); // windows
		e->enable();
		effects.push_back(e);
	}
	
	if(false){	// BASIC EFFECT
		basicEffect* e;
		e = new basicEffect();
		e->initialise();
		e->bindWithShapes( scene.getAllShapes() );
		e->enable();
		effects.push_back(e);
	}
	
	if(false){	// LINE EFFECT
		basicEffect* e;
		e = new lineEffect();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(3) ); // windows
		e->bindWithShapes( scene.getShapesByGroup(0) ); // columns
		e->enable();
		effects.push_back(e);
	}
	
	/ *{
	basicEffect* e;
	e = new imageMeltingEffect();
	e->initialise();
	e->bindWithShapes( scene.getShapesByGroup(3) );
	e->bindWithShapes( scene.getShapesByGroup(1) );
	effects.push_back(e);
	}// */
	
	/*{
	basicEffect* e;
	e = new imageGrainEffect();
	e->initialise();
	e->bindWithShapes( scene.getShapesByGroup(1) );
	effects.push_back(e);
	} // */
	
	/*{ // MESH 3D
		basicEffect* e;
		e = new meshRenderer();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(1) );
		effects.push_back(e);
	} // */
	
	/*{ // GIF effect
		basicEffect* e;
		e = new gifEffect();
		e->initialise();
		e->bindWithShapes( scene.getShapesByGroup(1) );
		effects.push_back(e);
	}// */
	
//	{
//		basicEffect* e;
//		e = new distortEffect();
//		e->initialise(animationParams.params);
//		e->bindWithShapes( scene.getShapesRef() );
//		effects.push_back(e);
//	}
	
	{
		//ofSetLogLevel(OF_LOG_VERBOSE);
		shaderEffect* e;
		e = new shaderEffect();
		e->initialise(animationParams.params);
		e->bindWithShapes( scene.getShapesRef() );
		//e->loadShader("",  "../../src/effects/shaderEffect/menger_journey.frag");
		e->loadShader("../../src/effects/shaderEffect/defaultShader.vert", "../../src/effects/shaderEffect/defaultShader.frag" );
		effects.push_back(e);
	}
	
//	{
//		basicEffect* e;
//		e = new lineEffect();
//		e->initialise(animationParams.params);
//		e->bindWithShapes( scene.getShapesRef() );
//		effects.push_back(e);
//	}
	
	
	return isEnabled()==true;
}

bool animationController::stop(){
	
	// disable mirOSCReceiver
	mirOSCReceiver.stop();
	oscRouter.removeNode( &mirOSCReceiver );
	
	// disable durationOSCReceiver
	//durationOSCReceiver.stop();
	//oscRouter.removeNode( &durationOSCReceiver );
	
	oscRouter.stop();
	
	bShowGui = false;
	
//	{
//		// play music with VLC
//		ofxVLCRemote vlc;
//		vlc.setup("/Users/daan/vlc.sock"); // client-specific... :(
//		vlc.run("stop");
//	}
	
	{	// SYNC DURATION
//		durationRC rc;
//		rc.setupOSC();
//		rc.stop();
	}
	
	// delete effect pointers
	// todo: this can make it crash...
	for(int i=effects.size()-1;i>=0;i--){
		delete effects[i];
	}
	
	effects.clear();
	effects.resize(0);
	
	// save recorded file ?
	//recorder.stopRecording();
	
	bEnabled = false;
	return !isEnabled();
}

bool animationController::isEnabled() const {
	return bEnabled;
}

// - - - - - - - -
// EVENT LISTENERS
// - - - - - - - -
void animationController::update(ofEventArgs &event){
	if(!isEnabled()) return;
	
	// reset shapes data to original state
	// every frame, effects can alter this
	for(auto it=scene.getShapesRef().begin(); it!=scene.getShapesRef().end(); ++it){
		(*it)->resetToScene();
	}
	
	// todo:
	// update/create animation state (bunch of variables)
	
	// update effects (run mode)
	for(int i=0; i<effects.size(); i++){
		effects[i]->update(animationParams.params);
	}
}

void animationController::draw(ofEventArgs& event){
	if(!isEnabled()) return;
	
	//ofScale(0.5, 0.5); // tmp
	
	//recorder.beginFrame();
	
	// tmp
	ofClear(0,1);
	//ofBackground(255,0,0);
	
	// render a scene without effects (tmp?)
	if(effects.size()==0 || ofGetKeyPressed(OF_KEY_LEFT_SHIFT)){
		ofSetColor( ofFloatColor(1.f, 1));//params.seasons.summer));
		ofFill();
		for(auto it=scene.getShapesRef().begin(); it!=scene.getShapesRef().end(); ++it){
			(*it)->sendToGPU();
		}
	}
	
	
	// draw effects
	else for(int i=0; i<effects.size(); i++){
		effects[i]->render(animationParams.params);
	}
	
	//recorder.endFrame(true);
	
	// syphon
#ifdef TARGET_OS_MAC
	//if(recorder.isRecording()){
//		syphonTexture.publishTexture( &recorder.getTexture() );
	//}
	//else {
//		syphonTexture.publishScreen();
	//}
#endif
	
	// draw gui stuff
	gui.begin();
	if( bShowGui ){
		ImGui::BeginMainMenuBar();
		if( ImGui::BeginMenu("karmaMapper :: Animator") ){
			if (ImGui::MenuItem("Version", "0.2 alpha")) {}
			if (ImGui::MenuItem("FPS", ofToString( ofGetFrameRate() ).c_str() ) ) {}
			char buffer[20];
			snprintf(buffer, 20, "%d x %i", ofGetWidth(), ofGetHeight() );
					 
			ImGui::MenuItem("Resolution", buffer );
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options")){
			static bool tmp_full_screen = false;
			if (ImGui::MenuItem("Full screen", NULL, &tmp_full_screen )){
				setFullScreen( tmp_full_screen );
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Extras")){
			if (ImGui::MenuItem("Show animation parameters", NULL, &bGuiShowAnimParams )){
				showAnimationsGui(bGuiShowAnimParams);
			}
			ImGui::MenuItem("Keyboard shortcuts...", NULL, &showShortcuts );
			
			
//			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
//			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
//			ImGui::Separator();
			ImGui::EndMenu();
		}
		
//		if( ImGui::BeginMenu("") ){
//			
//			ImGui::EndMenu();
//		}
		
		ImGui::EndMainMenuBar();
		
		
		ImGui::Begin("karmaMapper", &bShowGui );
		
//		if( ImGui::Button("Hide Gui") ){
//			bShowGui = !bShowGui;
//		}
		
		if( showShortcuts ) {
			//ImGui::SetNextWindowSize( ofVec2f(200,200), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Keyboard Shortcuts", &showShortcuts);
			
			ImGui::TextWrapped("These are some keyboard shortcuts you can use:");
			ImGui::Columns(2, "kb_shortcuts");
			ImGui::Separator();
			ImGui::Text("Description"); ImGui::NextColumn();
			ImGui::Text("Shortcut"); ImGui::NextColumn();
			ImGui::Separator();
			
			ImGui::Text("Toggle Cursor"); ImGui::NextColumn();
			ImGui::Text("M"); ImGui::NextColumn();
			
			ImGui::Text("Toggle Gui"); ImGui::NextColumn();
			ImGui::Text("H"); ImGui::NextColumn();
			
			
			ImGui::End();
			//ImGui::Columns(1);
		}
		
		ImGui::Spacing();
		if( ImGui::CollapsingHeader( GUIShapesInfo, "GUIShapesInfo", true, true ) ){
			ImGui::Separator();
			
			ImGui::Columns(2);
			if( ImGui::Button( GUILoadScene ) ){
				//showShapeLoadMenu();
				ImGui::OpenPopup("loadShapes");
				ImGui::SameLine();
			}
			if (ImGui::BeginPopup("loadShapes")){
				ofDirectory dir;
				dir.listDir( ofToDataPath("saveFiles/scenes/") );
				dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
				if( dir.size() <= 0 ){
					ImGui::Text("No files in /bin/data/");
					ImGui::Separator();
				}else {
					for (int i = 0; i < dir.size(); i++){
						if( ImGui::Selectable( dir.getPath(i).c_str(), (bool)(scene.getLoadedScene() == dir.getName(i)) ) ){
							stop();
							scene.loadScene( dir.getName(i) );
							start();
						}
					}
				}
				ImGui::EndPopup();
			}
			ImGui::NextColumn();
			ImGui::Text( "%s", scene.getLoadedScene().c_str() ); ImGui::NextColumn();
			ImGui::Columns(1);
			
			ImGui::Separator();
			ImGui::Text( "Number of shapes: %u", scene.getNumShapes() );
			
			// todo: list specific shape data here (n vertexShapes, etc)
			if(scene.getNumShapes()>0){
				if (ImGui::TreeNode("All Shapes")){
					static ImGuiTextFilter filter;
					filter.Draw("Filter by name");
					
					static int shapeTypeFilter = 0;
					ImGui::Text("Type: ");
					ImGui::SameLine();
					ImGui::RadioButton("All", &shapeTypeFilter, 0);
					
					auto shapesByType = scene.getAllShapesByType();
					int i = 1;
					for( auto it = shapesByType.begin(); it!=shapesByType.end(); ++it, ++i ){
						ImGui::SameLine();
						ImGui::RadioButton((it->first).c_str(), &shapeTypeFilter, i);
					}
					
					ImGui::Separator();
					
					ImGui::Columns(4);
					static bool firstTime = true;
					if( firstTime ){
						ImGui::SetColumnOffset(0, 00);
						ImGui::SetColumnOffset(1, 50);
						ImGui::SetColumnOffset(2, 230);
						ImGui::SetColumnOffset(3, 330);
						firstTime = false;
					}
					
					//ImGui::SameLine(20);
					ImGui::Text("On"); ImGui::NextColumn();
					ImGui::Text("Name"); ImGui::NextColumn();
					ImGui::Text("Type"); ImGui::NextColumn();
					ImGui::Text("Group"); ImGui::NextColumn();
					
					ImGui::Separator();
					
					for( auto it=scene.getShapesRef().begin(); it!=scene.getShapesRef().end(); ++it ){
						basicShape* s = *it;
						
						if( shapeTypeFilter > 0 ){
							auto shapeIt = shapesByType.begin();
							std::advance( shapeIt, shapeTypeFilter );
							
							if( s->getShapeType() == shapeIt->first ) continue;
						}
						
						if( filter.PassFilter( s->getName().c_str() ) ){
						
							ImVec4 statusColor( !s->isReady()*1.f, s->isReady()*1.f + (!s->isReady() && !s->hasFailed())*.5f, 0.f, 1.f );
							
							// todo: let users enable/disable shapes ?
							ImGui::Checkbox("", &s->isSelected );// Text("-");
							ImGui::NextColumn();// ImGui::SameLine(60);
							//ImGui::Text( "%s", s->getName().c_str() );
							if( ImGui::Selectable(s->getName().c_str(), &s->isSelected ) ){
								//s->isSelected = !s->isSelected;
							}
							ImGui::NextColumn();// ImGui::SameLine(180);
							ImGui::TextColored( statusColor, "%s", (*it)->getShapeType().c_str() );
							ImGui::NextColumn();// ImGui::SameLine(100);
							ImGui::Text( "%i", s->getGroupID() );
							ImGui::NextColumn(); // next line
							
							//ImGui::SameLine(600); ImGui::Text(" 2,345 bytes");
							// TODO: add shape-specific information line ?
						}
					}
					ImGui::Columns(1);
					
					ImGui::TreePop();
				}
					
//				ImGui::Separator();
//				
//				if (ImGui::TreeNode("Shape Groups"))
//				{
//					static bool selected[3] = { false, false, false };
//					ImGui::Selectable("main.c", &selected[0]);    ImGui::SameLine(300); ImGui::Text(" 2,345 bytes");
//					ImGui::Selectable("Hello.cpp", &selected[1]); ImGui::SameLine(300); ImGui::Text("12,345 bytes");
//					ImGui::Selectable("Hello.h", &selected[2]);   ImGui::SameLine(300); ImGui::Text(" 2,345 bytes");
//					ImGui::TreePop();
//				}
			}
			
			ImGui::Spacing();
			if( ImGui::CollapsingHeader( GUIEffectsTitle, "GUIShapesTitle", true, true ) ){
				ImGui::Separator();
				ImGui::Text( "Number of effects : %u", (int) effects.size() );
			}
			
		} // end bShowGui
		
		// end karma mapper main gui window
		ImGui::End();
	}
	
	gui.end();
}

// - - - - - - - -
// MENU EVENT LISTENERS
// - - - - - - - -
void animationController::showShapeLoadMenu(){
	//ofRectangle btn = loadButton.getShape();
	ofFileDialogResult openFileResult= ofSystemLoadDialog("Select the XML file to load...", false, ofToDataPath("saveFiles/scenes/"));
	
	if(openFileResult.bSuccess){
		// restrict to saveFiles dir
		ofFile file( ofToDataPath("saveFiles/")+openFileResult.getName() );
		if(file.exists()){
			stop();
			scene.loadScene( openFileResult.getName() );
			start();
		}
	}
}

void animationController::showSaveDialog(){
	ofFileDialogResult openFileResult= ofSystemSaveDialog(scene. getLoadedScene(), "Where shall I save your scene?");
	
	if(openFileResult.bSuccess){
		// only keep the file name, not the path
		scene.saveScene( openFileResult.getName() );
	}
}

void animationController::setFullScreen(bool & _fullScreen){
	if( bIsFullScreen != _fullScreen ){
		bIsFullScreen = _fullScreen;
		ofSetFullscreen( _fullScreen );
	}
}

void animationController::showAnimationsGui( bool& _b){
	// sync var bGuiShowAnimParams
	animationParams.setShowParams(_b);
}

//

//void animationController::guiEvent(ofxUIEventArgs &e){
//	string name = e.getName();
//	
//	// scan for open dropdowns
//	// (dirty) bypass for issue https://github.com/rezaali/ofxUI/issues/68
//	//ofxUIDropDownList* addShapeDDL = (ofxUIDropDownList*) gui->getWidget("Add Shape");
//	//bool dropdownOpen = addShapeDDL->isOpen();
//	//if( selectConfigFileOpen && name != "Configuration File") return;
//	
//	
//	// interface options
//	// - - - - - - - - -
//	if(name==GUIToggleFullScreen){
//		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
//		ofSetFullscreen(toggle->getValue());
//	}
//	else if(name=="lololol"){
//		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
//		toggle->getValue()?ofHideCursor():ofShowCursor();
//	}
//	
//	#ifdef KARMAMAPPER__DEBUG
//	else {
//		ofLogNotice("animationController::guiEvent") << "Unknown GUI event: " << name << endl;
//	}
//	#endif
//}

void animationController::_keyPressed(ofKeyEventArgs &e){
	
	// ignore keypress if imGui is typing
	if( ImGui::GetIO().WantTextInput ) return;
	
	// ignore case for shortcuts
	int keyToLower = ofToChar( ofToLower( ofToString((char) e.key )));
	
	// check key combinations
	if (ofGetKeyPressed(OF_KEY_COMMAND)) {
		
		// save ?
//		if ( 's' == keyToLower ){
//			
//		}
	}
	
	else {
		
		// toggle gui ?
		if ( 'h' == keyToLower ){
			bShowGui = !bShowGui;
		}
		
		if ( 'm' == keyToLower ){
			bMouseHidden ? ofShowCursor() : ofHideCursor();
			bMouseHidden = !bMouseHidden;
		}
		
	}
	
}

void animationController::_mousePressed( ofMouseEventArgs &e ){
	// ignore click if imGui intercepted it
	if( ImGui::GetIO().WantCaptureMouse ) return;
}
