//
//  cpuWebcamAnalyser.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 27/04/16.
//
//  - - - -
//
//  Displays a texture on shape

#include "cpuWebcamAnalyser.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

cpuWebcamAnalyser::cpuWebcamAnalyser(){
	cpuWebcamAnalyser::reset();
}

cpuWebcamAnalyser::~cpuWebcamAnalyser(){
	
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool cpuWebcamAnalyser::initialise(const animationParams& params){
	// init values
	videoShader::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	cpuWebcamAnalyser::reset();
	
	faceTracker.setup();
	
	return bInitialised;
}

bool cpuWebcamAnalyser::render(karmaFboLayer& renderLayer, const animationParams &params){
	//if( !videoShader::render(renderLayer, params) ) return false;
	if(!isReady()){
		return false;
	}
	
	bool tookShot = false;
	
	//renderLayer.end();
	//renderLayer.swap();
	renderLayer.begin();
	
	ofPushStyle();
	ofFill();
	ofSetColor(255);
	
//	if(textures.size()>0 ){
//		textures[0].draw(0,0);
//	}
	
	if(videoMode==VIDEO_MODE_UVC_WEBCAM && analysisMode == WEBCAM_ANALYSIS_FACEDETECT){
		
		if( faceTracker.getFound() ){
			ofVec3f orientation = faceTracker.getOrientation();
			float orientationAngle = orientation.angle(ofVec3f(1,0,0).getNormalized());
			
//			ofPushMatrix();
//			ofTranslate(renderLayer.getWidth()/2.f, renderLayer.getHeight()/2.f);
			//faceTracker.getMeanObjectMesh().draw();
			
//			ofNoFill();
//			ofDrawLine(ofVec3f(0), orientation*100 );
//			ofFill();
//			ofPopMatrix();
			
			//ofSetColor( abs(orientationAngle) );
			ofMesh faceMesh = faceTracker.getImageMesh();
			//ofMesh faceMesh = faceTracker.getObjectMesh();
			
			//faceMesh.draw();

			if(abs(orientationAngle) < fFaceTrackerOrientationTolerance){
				
				ofVec2f camPosition = faceTracker.getPosition();
				float camScale = faceTracker.getScale();
				//ofMesh reference = faceTracker.getImageMesh();
				int meshRectPts[4] = {ofGetWidth(),ofGetHeight(),0,0};
				for(int i = 0; i < faceMesh.getNumVertices(); i++) {
					ofVec2f point = faceMesh.getVertices()[i];
					if( point.x < meshRectPts[0] ) meshRectPts[0] = point.x;
					if( point.x > meshRectPts[2] ) meshRectPts[2] = point.x;
					if( point.y < meshRectPts[1] ) meshRectPts[1] = point.y;
					if( point.y > meshRectPts[3] ) meshRectPts[3] = point.y;
				}
				
				ofRectangle meshRect(meshRectPts[0],meshRectPts[1],meshRectPts[2]-meshRectPts[0],meshRectPts[3]-meshRectPts[1]);
				
				//ofDrawRectangle(meshRect);
				ofPoint leftEye = faceTracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getCentroid2D();
				ofPoint rightEye = faceTracker.getImageFeature(ofxFaceTracker::RIGHT_EYE).getCentroid2D();
				ofPoint thirdEye = leftEye+(rightEye-leftEye)/2.0f;
				
				float maskScale = leftEye.distance(rightEye)/(float)targetDistanceBetweenEyes;
				ofPoint thirdEyeTargetPos = ofPoint(renderLayer.getWidth()/2.f, renderLayer.getHeight()/2.f);
				
//				ofSetColor(255,255,0);
//				ofDrawCircle(thirdEyeTargetPos, 10);
//				ofSetColor(255);
				
				ofPushMatrix();
				
				//ofTranslate(thirdEyeTargetPos);
				ofTranslate((thirdEyeTargetPos-thirdEye));
				
				
				ofTranslate(thirdEye);
				ofScale(1.f/maskScale);
				ofRotateZ( (rightEye-leftEye).angle( ofPoint(1,0,0) ) );
				ofTranslate(-thirdEye);
				
				if(bDebugFaceTracking){
					// draw webcam image
					if(textures.size()>0 ){
						textures[0].draw(0,0);
					}
					
					//faceMesh.draw();
					
					ofSetColor(255,0,0);
					ofDrawCircle(thirdEye, 10);
					ofSetColor(255);
					
					ofNoFill();
					ofDrawLine(leftEye, rightEye );
					
					ofNoFill();
					ofDrawRectangle(meshRect);
				}
				
				
				if(lastShotTime+minTimeBetweenShots < ofGetElapsedTimef()){
					canTakeShot++;
				}
				
				if( 200 < canTakeShot && abs(orientationAngle) < fFaceTrackerOrientationTolerance/2.f){
					
					//ofSetColor(255); // FLASH
					//ofDrawRectangle(0, 0, renderLayer.getWidth(), renderLayer.getHeight());
					
					// draw webcam image for the screenshot...
					ofFill();
					if(textures.size()>0 && textures[0].isAllocated() ){
						textures[0].draw(0,0);
						tookShot = true;
					}
					
					canTakeShot = 0;
					lastShotTime = ofGetElapsedTimef();
				}
				else if(20 < canTakeShot){
					ofSetColor(255,128);
					faceMesh.drawWireframe();
				}
				
				ofPopMatrix();
				
				//
			}
			else {
				if(canTakeShot>0) canTakeShot--;
			}
		}
		else {
			if(canTakeShot>0) canTakeShot--;
		}
	}
	
	ofPopStyle();
	renderLayer.end();
	
	if(tookShot){
		//cout << "took screenshot" << endl;
		// register screen
		lastShot = renderLayer.getSrcTexture();
		
		ofPixels tmpPx;
		lastShot.readToPixels(tmpPx);
		ofSaveImage(tmpPx, ofToDataPath("visages/"+ofToString(ofGetFrameNum())+".png") );
	}
	
	return true;
}

// updates shape data
void cpuWebcamAnalyser::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	if(!isReady()){
		return false;
	}
	
	// call the shaderEffect update, not the videoEffect which will upload the webcam image to the GPU (not needed)
	shaderEffect::update( renderLayer, params );
	
	if( textures.size()>0 ){
		
		if(videoMode==VIDEO_MODE_UVC_WEBCAM && analysisMode == WEBCAM_ANALYSIS_FACEDETECT ){
			
			if( UVCWebcam.isReady() ){
				
				UVCWebcam.update();
				
				if(UVCWebcam.isFrameNew()){
					webcamFPSCounter.begin();
					if(ofGetFrameNum()%4==0) webcamFPSCounter.tick("newframe");
					webcamFPSCounter.end();
					
					faceTracker.update(ofxCv::toCv(UVCWebcam));
					
					//tex.loadData(vidGrabber.getPixels());
					//UVCWebcam.getPixels();
					textures[0].loadData( UVCWebcam.getPixels());
				}
				for(int i=0; i<WEBCAM_FPS_HISTORY_SIZE-1; ++i){
					webcamFPSHistory[i]=webcamFPSHistory[i+1];
				}
				webcamFPSHistory[WEBCAM_FPS_HISTORY_SIZE-1] = webcamFPSCounter.getFps();
				
			}
		}
	} // endif has textures
}

// resets all values
void cpuWebcamAnalyser::reset(){
	videoShader::reset();
	
	// effect type must match with class
	effectType = "cpuWebcamAnalyser";
	
	// over-ride videoShader's reset
	bUseShadertoyVariables = false;
	bUseTextures = true;
	setUsePingPong(false);
	fFaceTrackerOrientationTolerance = 60.0f;
	minTimeBetweenShots = 10;
	targetDistanceBetweenEyes = 200;
	canTakeShot = 0;
	lastShotTime = ofGetElapsedTimef();
	bDebugFaceTracking = false;
	setVideoMode(VIDEO_MODE_UVC_WEBCAM);
	setAnalysisMode(WEBCAM_ANALYSIS_FACEDETECT);
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
	
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool cpuWebcamAnalyser::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUIcpuWebcamAnalyserPanel, "GUIcpuWebcamAnalyserPanel", true, true ) ){
		
		ImGui::TextWrapped("Analyses webcam video.");
		
		if (ImGui::ListBoxHeader("Analysis Mode", 2)){
			
			// manually add new modes here
			if ( ImGui::Selectable("WEBCAM_ANALYSIS_FACEDETECT", analysisMode==WEBCAM_ANALYSIS_FACEDETECT)) {
				setAnalysisMode(WEBCAM_ANALYSIS_FACEDETECT);
			}
			ImGui::ListBoxFooter();
		}
		
		ImGui::Separator();
		
		if(analysisMode==WEBCAM_ANALYSIS_FACEDETECT){
			if(ImGui::TreeNode("Face Detection Settings")){
				bool detectedFace = faceTracker.getFound();
				ImGui::Text("Detects face: %d", detectedFace);
				ImGui::Checkbox("bDebugFaceTracking", &bDebugFaceTracking);
				
				ImGui::DragInt("Face orientation Tolerance (degrees)", &fFaceTrackerOrientationTolerance);
				
				ImGui::DragInt("Wait time between shots (sec)", &minTimeBetweenShots);
				
				ImGui::DragInt("targetDistanceBetweenEyes", &targetDistanceBetweenEyes);
				
				
				
				ImGui::TreePop(); // pop facedetect analysis
			}
		}
		
		ImGui::Separator();
	}
	
	videoShader::printCustomEffectGui();
	
	return true;
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool cpuWebcamAnalyser::saveToXML(ofxXmlSettings& xml) const{
	bool ret = videoShader::saveToXML(xml);
	
	xml.addValue("analysisMode", analysisMode);
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool cpuWebcamAnalyser::loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene){
	bool ret = videoShader::loadFromXML(xml, _scene);

	// over-ride videoShader's reset
	bUseShadertoyVariables = false;
	bUseTextures = true;
	setUsePingPong(false);
	setVideoMode(VIDEO_MODE_UVC_WEBCAM);
	ret *= loadShader( effectFolder("videoShader.vert", "videoShader"), effectFolder("videoShader.frag", "videoShader") );
	
	return ret;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool cpuWebcamAnalyser::randomizePresets(){
	if(!videoShader::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}

// - - - - - - -
// cpuWebcamAnalyser FUNCTIONS
// - - - - - - -
void cpuWebcamAnalyser::setAnalysisMode(const enum webcamAnalysisMode& mode){
	
	if(mode==WEBCAM_ANALYSIS_FACEDETECT){
		analysisMode = WEBCAM_ANALYSIS_FACEDETECT;
	}
	
	else {
		setError( true, "Unsupported analysis mode in cpuWebcamAnalyser.");
		ofLogWarning("cpuWebcamAnalyser::setAnalysisMode") << "UnSupported analysis mode: " << mode;
	}
}


void cpuWebcamAnalyser::threadedFunction(){

#ifdef KARMAMAPPER_DEBUG
	//ofThread::setThreadName("cpuWebcamAnalyser::threadedFunction() " + ofToString(ofThread::getThreadId()) );
#endif
	
	while (isThreadRunning()) {
		sleep(14); // limits CPU usage, todo: could be revised #dirty
	}
	stopThread();
}

// register effect type
EFFECT_REGISTER( cpuWebcamAnalyser , "cpuWebcamAnalyser" );
