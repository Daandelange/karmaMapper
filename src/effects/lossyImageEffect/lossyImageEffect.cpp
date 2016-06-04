//
//  lossyImageEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 22/04/2016.
//
//  - - - -
//
// Sends the last output frame to CPU which encodes it using lossy images compression then sends it back to the GPU to use for the next frame.

#include "lossyImageEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

lossyImageEffect::lossyImageEffect(){
	lossyImageEffect::reset();
}

lossyImageEffect::~lossyImageEffect(){
	ofRemoveListener( karmaFboLayer::karmaFboLayerBeforeDraw, this, &lossyImageEffect::beforeDraw );
	ofRemoveListener( karmaFboLayer::karmaFboLayerAfterDraw, this, &lossyImageEffect::afterDraw );
	
	toCompress.close();
	receiveCompressed.close();
	waitForThread(true, 1000);
	
	if (handleCompress){
		tjDestroy(handleCompress);
	}
	if (handleDecompress){
		tjDestroy(handleDecompress);
	}
	
	handleCompress = NULL;
	handleDecompress = NULL;
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool lossyImageEffect::initialise(const animationParams& params){
	// init values
	basicEffect::initialise(params);
	ofAddListener( karmaFboLayer::karmaFboLayerBeforeDraw, this, &lossyImageEffect::beforeDraw );
	ofAddListener( karmaFboLayer::karmaFboLayerAfterDraw, this, &lossyImageEffect::afterDraw );
	bIsLoading = true;
	bInitialised = false;
	
	handleCompress = tjInitCompress();
	if (handleCompress == NULL){
		printf("Error in tjInitCompress():\n%s\n", tjGetErrorStr());
	}
	
	handleDecompress = tjInitDecompress();
	if (handleDecompress == NULL){
		printf("Error in tjInitDeCompress():\n%s\n", tjGetErrorStr());
	}
	
	lossyImageEffect::reset();
	
	return bInitialised;
}

bool lossyImageEffect::render(karmaFboLayer& renderLayer, const animationParams &params){
	
	if(!isReady()) return false;
	
//	renderLayer.begin();
//	ofPushStyle();
//	ofSetColor(255,255,255,255);
//	
//	ofPopStyle();
//	renderLayer.end();
	
	// todo: add an option to upload pixels to GPU on layerIndex based position (in render())

	return true;
}

// updates shape data
void lossyImageEffect::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	// do basic Effect functionality
	basicEffect::update( renderLayer, params );
	
	if(!renderLayer.isAllocated()){
		return false;
	}
	
	// upload pixels to GPU
	bool bNewFrame = false;
	static ofPixels tmpPix;
	while(receiveCompressed.tryReceive(tmpPix)){
		bNewFrame = true;
	}
	if(bNewFrame){
		if(!compressedTexture.isAllocated()){
			compressedTexture.allocate(tmpPix);
		}
		compressedTexture.loadData(tmpPix);
	}
}

// resets all values
void lossyImageEffect::reset(){
	basicEffect::reset();
	
	// effect type must match with class
	effectType = "lossyImageEffect";
	
	// use default shader ?
	//loadShader( effectFolder("imageShader.vert", "lossyImageEffect"), effectFolder("textureDistortion.frag", "lossyImageEffect") );
	
	// (re)start thread
	if(isThreadRunning()){
		stopThread();
		//waitForThread();
	}
	startThread();
	
	// tmp, as the shader doesn't load... and is not needed yet...
	setError(false);
	
	// force some shader settings
	setUsePingPong(true);
	
	// init other vars
	fOpacity = 1.0f;
	fJpegQualityMin = 0.1f;
	fJpegQualityMax = 0.2f;
	effectMutex.lock();
	bHasNewFrameThreaded = true;
	effectMutex.unlock();
	
	ofRemoveListener( karmaFboLayer::karmaFboLayerBeforeDraw, this, &lossyImageEffect::afterDraw );
	ofAddListener( karmaFboLayer::karmaFboLayerAfterDraw, this, &lossyImageEffect::afterDraw );
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool lossyImageEffect::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUIlossyImageEffectPanel, "GUIlossyImageEffectPanel", true, true ) ){
		
		ImGui::TextWrapped("Sends the last output frame to CPU which encodes it using lossy images compression then sends it back to the GPU to use for the next frame.");
		
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Settings :");
		ImGui::Indent();{
			
			if(ImGui::Button("Useless Btn")){
				
				setError(false); // tmp
			}
			
			ImGui::Separator();
			
			//ImGui::DragFloat("JPEG Quality", &fJpegQuality, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloatRange2("JPEG Quality", &fJpegQualityMin, &fJpegQualityMax, 0.01f,0,1);
		
			ImGui::Separator();
			ImGui::Separator();
		
		}ImGui::Unindent();
		
		ImGui::Separator();
	}
	
	basicEffect::printCustomEffectGui();
	
	return true;
}

// - - - - - - -
// CUSTOM METHODS
// - - - - - - -
void lossyImageEffect::threadedFunction(){
	// wait until there's a new frame
	// this blocks the thread, so it doesn't use
	// the CPU at all, until a frame arrives.
	// also receive doesn't allocate or make any copies
	ofPixels pixels;
	
	while(toCompress.receive(pixels)){
		
		// don't process empty images
		if(!pixels.isAllocated() || handleCompress == NULL || handleDecompress == NULL || pixels.getWidth()==0 || pixels.getHeight()==0){
			continue;
		}
		
		// COMPRESS IMAGE to JPEG
		//ofBuffer buffer;
		int bpp = pixels.getBytesPerPixel();
		int w = pixels.getWidth();
		int h = pixels.getHeight();
		ofBuffer buffer;
		
		int jpegQuality = ofRandom(fJpegQualityMin, fJpegQualityMax)*100;//round((1.0+sin(ofGetElapsedTimef()*0.5))*49.0);
		jpegQuality = ofClamp(jpegQuality, 0,100);
		//cout << "encoding with Quality = " << jpegQuality << endl;
		
		int pitch = (w*bpp);
		int flags = 0;
		unsigned long size = w*h*bpp;
		
		int jpegsubsamp = ofNoise(ofGetElapsedTimef()*10.0)*4.0;
		
		if ( pixels.getImageType() == OF_IMAGE_COLOR_ALPHA){
			vector<unsigned char> buf; // guaranteed to be destroyed
			buf.resize(size);
			
			unsigned char * output = &buf[0];
			bool compressOK = false;
			
			try {
				int result = -1; // tmp
				result = tjCompress(handleCompress, (unsigned char*)(pixels.getData()), w, pitch, h, bpp, output, &size, jpegsubsamp, jpegQuality, flags);
				
				if(result==0){
					buffer.set((const char*)output, size);
				}
				
				//cout << jpegQuality << "\t" << jpegsubsamp << "\t" << flags << "\t" << result << "\t" << buf.size() << endl;
				compressOK = (result==0);
			}
			catch(std::exception& _e){
				compressOK = false;
				// todo: handle error
			}
			
			if(compressOK){
				// DECOMPRESS IMAGE
				int subsamp;
				int ok = 0;
				try {
					//ok = tjDecompressHeader2(handleDecompress, (unsigned char*)buffer.getData(), buffer.size(), &w, &h, &subsamp);
					ok = tjDecompressHeader2(handleDecompress, (unsigned char*) output, size, &w, &h, &subsamp);
				}
				catch(std::exception& _e){
					// todo: handle error
				}
				//cout << subsamp << endl;
				if (ok != 0)
				{
					printf("Error in tjDecompressHeader2():\n%s\n", tjGetErrorStr());
					//cout << jpegQuality << "\t" << jpegsubsamp << "\t" << flags << "\t" << " " << endl;
					return;
				}
				
				tjDecompress(handleDecompress, (unsigned char*)buffer.getData(), buffer.size(), pixels.getData(), w, 0, h, bpp, 0);
				
				// once processed send the result back to the
				// main thread. in c++11 we can move it to
				// avoid a copy
				//receiveCompressed.empty();
#if __cplusplus>=201103
				receiveCompressed.send(std::move(pixels));
#else
				receiveCompressed.send(pixels);
#endif
				
				//turboJpegLoadedImage.setFromPixels(pixels);
			}
		}
		
	}
}


// - - - - - - -
// LISTENERS
// - - - - - - -
void lossyImageEffect::afterDraw(karmaFboLayerDrawEventArgs& _args ){
	
	if(!isReady()){
		return;
	}
	
	if(!_args.fbo.isAllocated()){
		return;
	}
	
	// send current FBO to CPU, encode + decode it, then upload it back to GPU hoping it will be done before the next frame.
	if(toCompress.empty()){
		// todo: unstatic this
		static ofPixels tmpToCompress;
		if(!tmpToCompress.isAllocated()){
			tmpToCompress.allocate(_args.fbo.getWidth(),_args.fbo.getHeight(), ofGetImageTypeFromGLType(GL_RGBA));
		}
		//_args.fbo.swap();
		//_args.fbo.swap();
		_args.fbo.getSrcTexture().readToPixels(tmpToCompress);
		//_args.fbo.swap();
		
		//renderLayer.getFBO().readToPixels(tmpToCompress);
		toCompress.send(std::move(tmpToCompress));
	}
	
	effectMutex.lock();
	bHasNewFrameThreaded = true;
	effectMutex.unlock();
}

void lossyImageEffect::beforeDraw(karmaFboLayerDrawEventArgs& _args ){
	
	if(!isReady()){
		return;
	}
	
	// render encoded frame to texture
	//effectMutex.lock();
	if(/*bHasNewFrameThreaded &&*/ compressedTexture.isAllocated()){
		if(_args.fbo.isAllocated()){
			_args.fbo.begin();
			ofPushStyle();
			ofSetColor(255,fOpacity*255);
			//ofSetColor(mainColor[0],mainColor[1],mainColor[2],mainColor[3]);
			compressedTexture.draw(0,0);
			ofPopStyle();
			_args.fbo.end();
			//_args.fbo.swap();
		}
	}
	//effectMutex.unlock();
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool lossyImageEffect::saveToXML(ofxXmlSettings& xml) const{
	bool ret = basicEffect::saveToXML(xml);
	
	xml.addValue("fJpegQualityMin", fJpegQualityMin);
	xml.addValue("fJpegQualityMax", fJpegQualityMax);
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool lossyImageEffect::loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene){
	bool ret = basicEffect::loadFromXML(xml, _scene);
	
	fJpegQualityMin = xml.getValue("fJpegQualityMin", fJpegQualityMin);
	fJpegQualityMax = xml.getValue("fJpegQualityMax", fJpegQualityMax);
	
	//ofLoadImage(threadedTexture, ofToDataPath("ressources/images/collection1/Stronium-Tree-highres.jpg"));
	
	// tmp, as the shader doesn't load... and is not needed yet...
	setError(false);
	
	return ret;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool lossyImageEffect::randomizePresets(){
	if(!basicEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}

// register effect type
EFFECT_REGISTER( lossyImageEffect , "lossyImageEffect" );
