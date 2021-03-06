//
//  imageShader.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 06/02/2016.
//
//  - - - -
//
//  Uses a threaded function to load images from file(s)
//	Depends on ofThread and shaderEffect

#include "imageShader.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

imageShader::imageShader(){
	imageShader::reset();
}

imageShader::~imageShader(){
	ofRemoveListener(mirReceiver::mirTempoEvent, this, &imageShader::tempoEventListener);
	ofRemoveListener(liveGrabberOSC::liveGrabberNoteEvent, this, &imageShader::liveGrabberNoteEventListener);
	ofRemoveListener(liveGrabberOSC::liveGrabberFloatEvent, this, &imageShader::liveGrabberFloatEventListener);
	
	alphaFromThread.close();
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool imageShader::initialise(const animationParams& params){
	// init values
	shaderEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	imageShader::reset();
	
	return bInitialised;
}

bool imageShader::render(karmaFboLayer& renderLayer, const animationParams &params){
	
	if(!bDrawAlways && bReactToMusic && musicalAlpha==0) return true;
	
	// change alpha
	float originalAlpha = mainColor[3];
	mainColor[3] *= musicalAlpha;
	
	// render
	if(!shaderEffect::render(renderLayer, params)){
		mainColor[3] = originalAlpha;
		return false;
	}
	mainColor[3] = originalAlpha;
	
	return true;
}

// updates shape data
void imageShader::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	// do basic Effect function
	shaderEffect::update( renderLayer, params );
	
	float newMusicalAlpha;
	bool gotNew = false;
	while( alphaFromThread.tryReceive(newMusicalAlpha)){
		gotNew = true;
	}
	if(gotNew){
		musicalAlpha = newMusicalAlpha;
	}
	else if(musicalAlpha > 0) {
		musicalAlpha -= alphaReducer;// todo: make this time-dependent
	}
//	effectMutex.lock();
//	if(bReactToMusic ){
//		// todo: maincolor has to become a changeable color parameter so we don't have to alter it's original value // nodes
//		mainColor[0] = liveGrabberOSC::liveGrabberAnalysis.lowFollower;
//		mainColor[1] = liveGrabberOSC::liveGrabberAnalysis.midFollower*2;
//		mainColor[2] = liveGrabberOSC::liveGrabberAnalysis.highFollower*2;
//	}
//	effectMutex.unlock();
}

// resets all values
void imageShader::reset(){
	shaderEffect::reset();
	
	// effect type must match with class
	effectType = "imageShader";
	
	ofRemoveListener(mirReceiver::mirTempoEvent, this, &imageShader::tempoEventListener);
	ofAddListener(mirReceiver::mirTempoEvent, this, &imageShader::tempoEventListener);
	
	ofRemoveListener(liveGrabberOSC::liveGrabberNoteEvent, this, &imageShader::liveGrabberNoteEventListener);
	ofAddListener(liveGrabberOSC::liveGrabberNoteEvent, this, &imageShader::liveGrabberNoteEventListener);
	
	ofRemoveListener(liveGrabberOSC::liveGrabberFloatEvent, this, &imageShader::liveGrabberFloatEventListener);
	ofAddListener(liveGrabberOSC::liveGrabberFloatEvent, this, &imageShader::liveGrabberFloatEventListener);
	
	musicalAlpha = 1.f;
	alphaReducer = 0.01f;//
	
	bool tmp = loadShader( effectFolder("videoShader.vert", "videoShader"), effectFolder("videoShader.frag", "videoShader") );
	
	bDrawAlways = true;
	bReactToMusic = false;
	imagePath = "";
	
	bUseTextures = true;
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool imageShader::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUIimageShaderPanel ) ){
		
		ImGui::TextWrapped("Loads an image and renders it.");
		
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Settings :");
		ImGui::Indent();
		if(ImGui::Button("Load image...")){
			ofFileDialogResult d = ofSystemLoadDialog("Choose a video file...");
			if(d.bSuccess){
				loadFromImage( d.getPath() );
			}
		}
		
		ImguiShowTextureMode();
		
		ImGui::Separator();
		ImGui::Checkbox("Draw always", &bDrawAlways);
		
		effectMutex.lock();
		if(ImGui::Checkbox("React to music", &bReactToMusic )){
			// disable this param so this one is visible
			if(bReactToMusic && bDrawAlways) bDrawAlways = false;
		}
		effectMutex.unlock();
		ImGui::Unindent();
		
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Controls");
		
		ImGui::SliderFloat("musical Alpha", &musicalAlpha, 0.f, 1.f);
		ImGui::SliderFloat("alpha reducer", &alphaReducer, 0.f, 0.1f);
		
		ImGui::Indent();
		if(ImGui::Button("Re-draw image")){
			musicalAlpha = 1.f;
		}
		
		ImGui::Separator();
		ImGui::Separator();
		
		if(textures.size()>0 && textures.back().isAllocated()){
			ImGui::LabelText("Loaded image", "%s", imagePath.c_str() );
			ImGui::LabelText("Image width", "%f", textures.back().getWidth() );
			ImGui::LabelText("Image height", "%f", textures.back().getHeight() );
		}
		ImGui::Unindent();
		
		ImGui::Separator();
	}
	
	shaderEffect::printCustomEffectGui();
	
	return true;
}

void imageShader::ImguiShowTextureMode(){
	if( ImGui::ListBoxHeader("Texture Mode...", 2) ){
		if(ImGui::Selectable("Fill Shape", textureMode==0)){
			setTextureMode(0);
		}
		if(ImGui::Selectable("Cover", textureMode==1)){
			setTextureMode(1);
		}
		if(ImGui::Selectable("Fit (clamped)", textureMode==2)){
			setTextureMode(2);
		}
		if(ImGui::Selectable("Fit (repeat)", textureMode==3)){
			setTextureMode(3);
		}
		if(ImGui::Selectable("Multiple shapes", textureMode==4)){
			setTextureMode(4);
		}
		ImGui::ListBoxFooter();
	}
}

// - - - - - - -
// CUSTOM METHODS
// - - - - - - -

bool imageShader::loadFromImage(string _imagePath){
	ofFile file( _imagePath );
	if( file.exists() && file ){
		imagePath = file.getAbsolutePath();
		
		ofImage tmpImage;
		if( tmpImage.load( file.getAbsolutePath() ) ){
			textures.clear();
			textures.push_back( ofTexture() );
			textures.back().allocate(tmpImage.getWidth(), tmpImage.getHeight(), GL_RGB);
			textures.back().loadData( tmpImage.getPixels() );
			
			shaderToyArgs.iChannelResolution[0*3+0] = tmpImage.getWidth();
			shaderToyArgs.iChannelResolution[0*3+1] = tmpImage.getHeight();
			shaderToyArgs.iChannelResolution[0*3+2] = tmpImage.getWidth() / tmpImage.getHeight();
			shaderToyArgs.iChannelTime[0]=0.f;
			
			imagePath = file.getAbsolutePath();
			
			ofLogNotice("imageShader::loadVideoFile") << "Loaded "<< imagePath << ".";
		}
		else {
			// not in video mode
		}
		return true;
	}
	else {
		ofLogNotice("videoShader::loadVideoFile") << "Invalid movie file. Not loading...";
	}
	return false;
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool imageShader::saveToXML(ofxXmlSettings& xml) const{
	bool ret = shaderEffect::saveToXML(xml);

	xml.addValue("bReactToMusic", bReactToMusic);
	xml.addValue("bDrawAlways", bDrawAlways);
	xml.addValue("imagePath", imagePath);
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool imageShader::loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene){
	bool ret = shaderEffect::loadFromXML(xml, _scene);
	
	bReactToMusic = xml.getValue("bReactToMusic",bReactToMusic);
	bDrawAlways = xml.getValue("bDrawAlways", bDrawAlways);
	
	ret *= loadFromImage( xml.getValue("imagePath", imagePath ) );
	
	return ret;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

// return false if effect should remain a little before deletion
bool imageShader::disableSoonIsNow() {
	//cout << (particles.getParticleCount() >= 0) << endl;
	return ( (bReactToMusic && musicalAlpha==0) || !bReactToMusic);
}

bool imageShader::randomizePresets(){
	if(!shaderEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}

// - - - - -
// imageShader Methods
// - - - - -
void imageShader::tempoEventListener(mirTempoEventArgs &_args){
	ofScopedLock lock(effectMutex);
	
	if(!bReactToMusic) return;
	
	if(shapes.size()<=0) return;
	
	if(_args.isTempoBis){
		//bReDrawNextFrame = true;
//		for(auto s=shapes.begin(); s!=shapes.end(); ++s){
//			if( (*s)->isType("vertexShape") ){
//				
//			}
//		}
		alphaFromThread.send(std::move(1.f));
	}
}

void imageShader::liveGrabberNoteEventListener(liveGrabberNoteEventArgs &_args){
	ofScopedLock lock(effectMutex);
	
	if(!bReactToMusic) return;
	
	if(shapes.size()<=0) return;
	
	if(_args.key.compare("A0")==0){
		//bReDrawNextFrame = true;
		alphaFromThread.send(std::move(1.f));
	}
}

void imageShader::liveGrabberFloatEventListener(liveGrabberFloatEventArgs &_args){
	//ofScopedLock lock(effectMutex);
	
	if(!isReady()) return;
	
	if(!bReactToMusic) return;
	
	if(shapes.size()<=0) return;
	
	if(_args.what.compare("criquet")==0){
		float value = _args.value;
		alphaFromThread.send(std::move(value));
	}
}


// register effect type
EFFECT_REGISTER( imageShader , "imageShader" );
