//
//  shaderEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 10/10/15.
//
//  - - - -
//
//  Applies on vertexShape
//	Distorts it's vertexes

#include "shaderEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

shaderEffect::shaderEffect(){
	
	shaderEffect::reset();
	
	// todo: bind only when bUseShaderVariables is on ?
	ofAddListener( ofEvents().windowResized , this, &shaderEffect::onResizeListener);
}

shaderEffect::~shaderEffect(){
	ofRemoveListener(mirReceiver::mirOnSetEvent, this, &shaderEffect::onSetEventListener);
	
	ofRemoveListener( ofEvents().windowResized , this, &shaderEffect::onResizeListener);
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool shaderEffect::initialise(const animationParams& params){
	// init values
	basicEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	onSetCalls = 0;
	fTimeFactor = 1.0f;
	bUseShadertoyVariables = false;
	bUseMirVariables = false;
	
	// do stuff
	ofEnableAlphaBlending();
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
	
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 8);
	fbo.begin();
	ofClear(0,0,0,0); // clear all, including alpha
	fbo.end();
	
	ofRemoveListener(mirReceiver::mirOnSetEvent, this, &shaderEffect::onSetEventListener);
	ofAddListener(mirReceiver::mirOnSetEvent, this, &shaderEffect::onSetEventListener);
	
	return bInitialised;
}

bool shaderEffect::render(const animationParams &params){
	if(!isReady() || !shader.isLoaded()) return false;
	
	fbo.begin();
	// fade out
	//ofClear(ofFloatColor(0,0,0, 30);
	
//	glEnable(GL_BLEND);
//	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
//	glBlendEquation(GL_FUNC_ADD);

	//ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
//	glEnable(GL_BLEND);
//	glBlendEquation(GL_FUNC_ADD);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	glBlendFunc(GL_ONE, GL_ONE);
	
	
//	glEnable(GL_BLEND);
//	glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_DST_ALPHA);
//	glBlendEquation(GL_FUNC_SUBTRACT);
	
	
	//glBlendColor(0,0,0,30);

	ofSetColor(0.0f, 5.0f*params.seasons.spring + 5.0f*params.seasons.autumn);
	ofFill();
	ofDrawRectangle(0,0, fbo.getWidth(), fbo.getHeight());
	
	glDisable(GL_BLEND);
	
	shader.begin();
	registerShaderVariables(params);
	
	// set drawing environment
	effectMutex.lock();
	ofSetColor( params.varyingColors.main.getLerped(ofColor(0), onSetCalls/10.0f) );
	onSetCalls = 0;
	effectMutex.unlock();
	ofFill();
	
	// tmp
	//ofDrawRectangle(50,50, 200,200);
	
	// draw shape so GPU gets their vertex data
	for(auto it=shapes.begin(); it!=shapes.end(); ++it){
		shader.setUniform4f("shapeBoundingBox", (*it)->getBoundingBox().x, (*it)->getBoundingBox().y, (*it)->getBoundingBox().width, (*it)->getBoundingBox().height );
			//cout << (*it)->getBoundingBox().width << endl;
			(*it)->sendToGPU();
	}
	
	// flush the pipeline! :D
	shader.end();
	fbo.end();
	
	// draw fbo
	fbo.draw(0,0);
	
	return true;
}

// updates shape data
void shaderEffect::update(const animationParams& params){
	
	// do basic Effect function
	basicEffect::update( params );
	
	ofScopedLock lock(effectMutex);
	
	// update shaderToyArgs
	if(bUseShadertoyVariables){
		shaderToyArgs.iMouse[0] = ofGetMouseX();
		shaderToyArgs.iMouse[1] = ofGetMouseY();
		shaderToyArgs.iMouse[2] = ofGetMousePressed( OF_MOUSE_BUTTON_1 );
		shaderToyArgs.iMouse[3] = ofGetMousePressed( OF_MOUSE_BUTTON_2 );
		
		shaderToyArgs.iFrame = ofGetFrameNum();
		
		// todo: should only be updated on change
		shaderToyArgs.iResolution[0] = ofGetWindowWidth();
		shaderToyArgs.iResolution[1] = ofGetWindowHeight();
		shaderToyArgs.iResolution[2] = ofGetWindowMode();
		
		shaderToyArgs.iGlobalTime = ofGetElapsedTimef()*shaderToyArgs.iGlobalTimeScale;
		
		shaderToyArgs.iDate[0] = ofGetYear();
		shaderToyArgs.iDate[1] = ofGetMonth();
		shaderToyArgs.iDate[2] = ofGetDay();
		shaderToyArgs.iDate[3] = ofGetElapsedTimef();
	}
	
	if(bUseMirVariables){
		// todo
	}
	
	if( !isReady() ) return;
	
//	
//	for(auto it=shapes.begin(); it!=shapes.end(); it++){
//		// todo: basicEffect should have a nice downcast getter function like .asType<vertexShape>()
//		if( (*it)->isType("vertexShape") ){
//			list<basicPoint>& pts = ((vertexShape*)(*it))->getPoints(POINT_POSITION_RELATIVE);
////			int offset = 0;
////			for(auto itb=pts.begin(); itb!=pts.end(); ++itb){
////				(*itb) += basicPoint(
////					sin(params.seasons.summer*TWO_PI+offset*params.seasons.summer)*50,
////					cos(params.seasons.summer*TWO_PI+offset*params.seasons.summer)*50
////				);
////				offset++;
////			}
////			(*it)->onShapeModified();
//		}
//	}
	// start shader
	
}

// resets all values
void shaderEffect::reset(){
	basicEffect::reset();
	
	ofScopedLock lock(effectMutex);
	
	// effect type must match with class
	effectType = "shaderEffect";
	vertexShader = effectFolder(ShaderEffectDefaultVert);
	fragmentShader = effectFolder(ShaderEffectDefaultFrag);
	
	bUseShadertoyVariables = false;
	bUseMirVariables = false;
	shaderToyArgs = shaderToyVariables();
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool shaderEffect::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUIShaderPanel, "GUIShaderPanel", true, true ) ){
		
		ImGui::TextWrapped("This effect loads shader files and animates them feeding it parameters.");
		ImGui::TextWrapped("You can enable some default variables or create your own."); // todo
		
		ImGui::Separator();
		
		ImGui::LabelText("Vertex Shader", "%s", vertexShader.c_str() );
		if(ImGui::Button("Load .vert...")){
			ofFileDialogResult d = ofSystemLoadDialog("Select vertex shader...");
			if(d.bSuccess){
				ofFile file( d.getPath() );
				if(file.exists()){
					vertexShader = file.getAbsolutePath();
					loadShader(vertexShader, fragmentShader);
				}
				else {
					// notify 404 ?
				}
			}
		}
		ImGui::SameLine();
		if(ImGui::Button("Clear Vertex")){
			loadShader("", fragmentShader);
		}
		
		ImGui::Separator();
		
		ImGui::LabelText("Fragment Shader", "%s", fragmentShader.c_str() );
		if(ImGui::Button("Load Fragment...")){
			ofFileDialogResult d = ofSystemLoadDialog("Select fragment shader...");
			if(d.bSuccess){
				ofFile file( d.getPath() );
				if(file.exists()){
					fragmentShader = file.getAbsolutePath();
					loadShader(vertexShader, fragmentShader);
				}
				else {
					// notify 404 ?
					
				}
			}
		}
		ImGui::SameLine();
		if(ImGui::Button("Clear Fragment")){
			loadShader("", vertexShader);
		}
		
		ImGui::Separator();
		
		if( !shader.isLoaded() ){
			ImGui::TextWrapped("Shader not loaded...");
		}
		else {
			ImGui::TextWrapped("Shader(s) loaded! :)");
		}
		ImGui::SameLine();
		if(ImGui::Button("Reload Shader(s)")){
			loadShader(vertexShader, fragmentShader);
		}
		
		ImGui::Separator();
		
		if( ImGui::CollapsingHeader( GUIShaderVariablesPanel, "GUIShaderVariablesPanel", true, true ) ){
			
			ImGui::TextWrapped("Here, you can enable/disable some standard shader variables, and (soon) create your own ones.");
			
			ImGui::Separator();
			
			ImGui::Checkbox("Inject shadertoy variables.", &bUseShadertoyVariables );
			if(bUseShadertoyVariables){
				ImGui::Indent();
				ImGui::SliderFloat("Time scale", &shaderToyArgs.iGlobalTimeScale, 0, 10);
				ImGui::Separator();
				ImGui::Text("(These are not editable...)");
				//ImGui::TextDisabled("iResoltion = %ix%ipx (%f)", (int)( shaderToyArgs.iResolution[0] ), (int)(shaderToyArgs.iResolution[1]), shaderToyArgs.iResolution[2] );
				ImGui::InputFloat3("iResolution", shaderToyArgs.iResolution );
				ImGui::InputFloat("iGlobalTime", &shaderToyArgs.iGlobalTime );
				//ImGui::TextDisabled("iGlobalTime = %f", shaderToyArgs.iGlobalTime );
				ImGui::InputFloat("iTimeDelta", &shaderToyArgs.iTimeDelta );
				ImGui::InputInt("iFrame", &shaderToyArgs.iFrame );
				ImGui::InputFloat4("iMouse", shaderToyArgs.iMouse );
				ImGui::InputFloat4("iDate", shaderToyArgs.iDate );
				ImGui::Separator();
				
				ImGui::Text("Textures (not yet)"); // todo
				ImGui::InputFloat4("iChannelTime", shaderToyArgs.iChannelTime );
				
				ImGui::InputFloat3("iChannelResolution[0]", shaderToyArgs.iChannelResolution[0] );
				ImGui::InputFloat3("iChannelResolution[1]", shaderToyArgs.iChannelResolution[1] );
				ImGui::InputFloat3("iChannelResolution[2]", shaderToyArgs.iChannelResolution[2] );
				ImGui::InputFloat3("iChannelResolution[3]", shaderToyArgs.iChannelResolution[3] );
				
				ImGui::Unindent();
			} // end shadertoy variables
			
			ImGui::Checkbox("Inject mir variables.", &bUseMirVariables );
			if(bUseMirVariables){
				ImGui::Indent();
				ImGui::TextWrapped("Forwards karmaSoundAnalyser audio into the shader.");
				
				ImGui::Separator();
				ImGui::Unindent();
				
			} // end mir variables
		}
		
		
	}
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool shaderEffect::saveToXML(ofxXmlSettings& xml) const{
	
 	bool ret = basicEffect::saveToXML(xml);
	
	xml.addValue("vertexShader", vertexShader );
	xml.addValue("fragmentShader", fragmentShader );
	
	xml.addValue("useShadertoyVariables", bUseShadertoyVariables );
	
	xml.addTag("shadertoyVariables");
	if( xml.pushTag("shadertoyVariables") ){
		
		xml.addValue("iGlobalTimeScale", shaderToyArgs.iGlobalTimeScale );
		
		xml.popTag();
	}
	
	xml.addValue("bUseMirVariables", bUseMirVariables );
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool shaderEffect::loadFromXML(ofxXmlSettings& xml){
	bool ret = basicEffect::loadFromXML(xml);
	
	loadShader( xml.getValue("vertexShader",""), xml.getValue("fragmentShader","") );
	
	bUseShadertoyVariables = xml.getValue( "useShadertoyVariables", false );
	
	if( xml.pushTag("shadertoyVariables") ){
		shaderToyArgs.iGlobalTimeScale = xml.getValue("iGlobalTimeScale", 1.0f );
		
		xml.popTag();
	}
	
	bUseMirVariables = xml.getValue( "bUseMirVariables", false );
	
	return shader.isLoaded();
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool shaderEffect::randomizePresets(){
	if(!basicEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}

// - - - - -
// shaderEffect Methods
// - - - - -
void shaderEffect::registerShaderVariables(const animationParams& params){
	
	
	effectMutex.lock();
	
	if( !shader.isLoaded() ){
		ofLogWarning("shaderEffect::registerShaderVariables() --> shader not loaded or linked!");
		return;
	}
	
	//if(shader==NULL) return;
	
	shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
	shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
	
	shader.setUniform2f("fboCanvas", ofGetWidth(), ofGetHeight() );
	
	shader.setUniform1i("tex", 0);
	
	effectMutex.unlock();
	
	
	if( bUseMirVariables ) registerMirVariables();
	
	if( bUseShadertoyVariables ) registerShaderToyVariables();
}

// registers shadertoy variables
void shaderEffect::registerShaderToyVariables(){
	ofScopedLock( effectMutex );
	shader.setUniform1f("iResolution", ofGetElapsedTimef() * fTimeFactor );
	
	
// These variables will be available in your shader :)
	
//	uniform vec3      iResolution;           // viewport resolution (in pixels)
//	uniform float     iGlobalTime;           // shader playback time (in seconds)
//	uniform float     iTimeDelta;            // render time (in seconds)
//	uniform int       iFrame;                // shader playback frame
//	uniform float     iChannelTime[4];       // channel playback time (in seconds)
//	uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
//	uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
//	uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
//	uniform vec4      iDate;                 // (year, month, day, time in seconds)
//	uniform float     iSampleRate;           // sound sample rate (i.e., 44100)
	
}

void shaderEffect::registerMirVariables() {
	
	ofScopedLock(effectMutex);
	
	shader.setUniform1f("mirZeroCrossings", mirReceiver::mirCache.zcr );
	shader.setUniform1f("mirZcr", mirReceiver::mirCache.zcr);
	shader.setUniform1f("mirPitch", mirReceiver::mirCache.pitch);
	shader.setUniform1f("mirBpm", mirReceiver::mirCache.bpm);
	shader.setUniform1f("mirBalance", mirReceiver::mirCache.balance);
	shader.setUniform1f("mirVolume", mirReceiver::mirCache.volumeMono);
	shader.setUniform1i("mirSilence", mirReceiver::mirCache.silence);
	shader.setUniform1f("mirOnSetCalls", onSetCalls );
}

bool shaderEffect::loadShader(string _vert, string _frag){
	;
	//shader.setupShaderFromFile(GL_FRAGMENT_SHADER, _frag);
	// todo: lock effectMutex here ?
	
	if( shader.isLoaded() ){
		shader.unload();
		fragmentShader = "";
		vertexShader = "";
	}
	
	if( shader.load(_vert, _frag) ){
		if( shader.isLoaded() ){
			bHasError = false;
			fragmentShader = _frag;
			vertexShader = _vert;
		}
		else {
			bHasError = true;
			shortStatus = "Shader not loaded (but it compiled)";
		}
	}
	else{
		ofLogNotice("shaderEffect::loadShader() --> shader not loaded");
		shortStatus = "Shader not loaded!";
		
		// todo: trigger shader not found errors here
		bHasError = true;
	}
	
	return bHasError;
	
	
	//cout << "OK:" << shader.linkProgram() << endl;
	//cout << shader.isLoaded() << shader.linkProgram() << endl;
	//shader.checkProgramInfoLog( shader.getProgram() );
}

void shaderEffect::onSetEventListener(mirOnSetEventArgs &_args){
	ofScopedLock lock(effectMutex);
	
	if(shapes.size()<=0) return;
	
	if(_args.source.compare("aubioOnSet")==0){
		onSetCalls++;
	}
	
	//else if(_args.source.compare("")==0){}
}

void shaderEffect::onResizeListener(ofResizeEventArgs &resize){
	
	if( bUseShadertoyVariables ){
		shaderToyArgs.iResolution[0] = resize.width;
		shaderToyArgs.iResolution[1] = resize.height;
		shaderToyArgs.iResolution[2] = ofGetWindowMode();
	}
}


// register effect type
EFFECT_REGISTER( shaderEffect , "shaderEffect" );
