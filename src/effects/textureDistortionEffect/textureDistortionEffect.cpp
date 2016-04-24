//
//  textureDistortionEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 14/04/2016.
//
//  - - - -
//
//  Uses a threaded function to load images from file(s)
//	Depends on ofThread and shaderEffect

#include "textureDistortionEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

textureDistortionEffect::textureDistortionEffect(){
	textureDistortionEffect::reset();
}

textureDistortionEffect::~textureDistortionEffect(){
	ofRemoveListener(ofEvents().mousePressed, this, &textureDistortionEffect::mouseClicked );
	ofRemoveListener(ofEvents().mouseReleased, this, &textureDistortionEffect::mouseReleased );
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool textureDistortionEffect::initialise(const animationParams& params){
	// init values
	shaderEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	textureDistortionEffect::reset();
	
	return bInitialised;
}

bool textureDistortionEffect::render(karmaFboLayer& renderLayer, const animationParams &params){
	
	if(!isReady()) return false;
	
	
	if(bIsInEditMode && clicked!=nullptr){
			// draw nothing to reveal the original (underlying) undistorted texture
	}
	else if(shader.isLoaded()){
		
		renderLayer.swap();
		renderLayer.begin();
		
		ofPushStyle();
		
		shader.begin();
		//registerShaderVariables(params);
		
		shader.setUniform1i("kmIsPingPongPass", 1);
		if(bUseCustomFbo){
			shader.setUniformTexture("pingPongTexture", fbo.getTexture(),5);
		}
		else {
			// note: between begin() and end() SRC is DST
			shader.setUniformTexture("pingPongTexture", renderLayer.getDstTexture(),5);
		}
		
		//ofSetColor(255,255,255,255);
		ofSetColor(mainColor[0]*255,mainColor[1]*255,mainColor[2]*255,mainColor[3]*255);
		triangulation.triangleMesh.draw();
		shader.end();
		
		ofPopStyle();
		renderLayer.end();
	}
	// no custom shader rendering...
	else {
		renderLayer.begin();
		ofPushStyle();
		ofSetColor(255,255,255,255);
		//myTex.bind();
		triangulation.triangleMesh.draw();
		//myTex.unbind();
		
		ofPopStyle();
		renderLayer.end();
	}
	
	if(bIsInEditMode){
		renderLayer.begin();
		ofPushStyle();
		if(clicked!=nullptr){
			// just draw nothing to reveal texture
			//myTex.draw(0,0);
			ofSetColor(mainColor[0]*255,mainColor[1]*255,mainColor[2]*255,mainColor[3]*255/2.0f);
			//ofDrawRectangle(0,0, renderLayer.getWidth(), renderLayer.getHeight());
		}
		// draw points
		ofNoFill();
		ofSetColor(255,0,0,255);
		for(auto pt=distortedPoints.begin(); pt!=distortedPoints.end(); ++pt){
			ofVec3f tmpAbsPt(pt->second.texCoordOffset + *(pt->second.shapeCenter) + *(pt->first) );
			ofDrawLine( tmpAbsPt, triangulation.triangleMesh.getVertex(pt->second.texCoordIndex));
			ofDrawCircle(tmpAbsPt, KM_TDE_HANDLE_RADIUS);
		}
		ofPopStyle();
		renderLayer.end();
	}
	
	// draw triangle mesh ?
	if(bDrawTriangulation){
		renderLayer.begin();
		ofPushStyle();
		ofNoFill();
		ofSetColor(mainColor[0]*255,mainColor[1]*255,mainColor[2]*255,mainColor[3]*255);
		//triangulation.triangleMesh.drawVertices();
		triangulation.triangleMesh.drawWireframe();
		ofPopStyle();
		renderLayer.end();
	}
	
	return true;
}

// updates shape data
void textureDistortionEffect::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	// do basic Effect functionality
	shaderEffect::update( renderLayer, params );
	
	// drag/click ?
	if(bIsInEditMode && clicked!=nullptr){
		if(distortedPoints.find(clicked) != distortedPoints.end()){
			//distortedPoints[clicked].texCoordOffset.set(ofGetMouseX(),ofGetMouseY());
			distortedPoints[clicked].texCoordOffset.set(ofVec2f(ofGetMouseX(),ofGetMouseY())- *(distortedPoints[clicked].shapeCenter) - *clicked );
		}
	}
}

// resets all values
void textureDistortionEffect::reset(){
	shaderEffect::reset();
	
	// effect type must match with class
	effectType = "textureDistortionEffect";
	
	loadShader( effectFolder("textureDistortion.vert", "textureDistortionEffect"), effectFolder("textureDistortion.frag", "textureDistortionEffect") );
	
	// tmp, as the shader doesn't load... and is not needed yet...
	setError(false);
	
	// force some shader settings
	setUsePingPong(true);
	
	clicked = nullptr;
	//vbo.clear();
	bIsInEditMode = false;
	distortedPoints.clear();
	
	// set this when done
	bInitialised = true;
	bIsLoading = false;
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool textureDistortionEffect::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUItextureDistortionEffectPanel, "GUItextureDistortionEffectPanel", true, true ) ){
		
		ImGui::TextWrapped("Lets you match specific regions of a texture by creating a mesh distortion grid on its bound shapes.");
		ImGui::TextWrapped("Use your mouse to edit the mesh.");
		ImGui::TextWrapped("Note: loading & saving only works when the bound shapes don't change in between.");
		
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Settings :");
		ImGui::Indent();{
			
			if(ImGui::Button("Generate new grid from shapes")){
				generateGridFromShapes();
				setError(false); // tmp
			}
			
			if(ImGui::Checkbox("Edit Mesh Grid", &bIsInEditMode)){
				if(bIsInEditMode){
					
					// re-sync points with
					if(shapes.size() > 0){
						ofAddListener(ofEvents().mousePressed, this, &textureDistortionEffect::mouseClicked );
						ofAddListener(ofEvents().mouseReleased, this, &textureDistortionEffect::mouseReleased );
					}
					else {
						setError(true, "No Shapes to perform a wrap!");
					}
					
				}
				else {
					// we dont need this anymore...
					ofRemoveListener(ofEvents().mousePressed, this, &textureDistortionEffect::mouseClicked );
					ofRemoveListener(ofEvents().mouseReleased, this, &textureDistortionEffect::mouseReleased );
				}
			}
			
			ImGui::Checkbox("Draw Triangulation", &bDrawTriangulation);
		
			ImGui::Separator();
			ImGui::Separator();
		
		}ImGui::Unindent();
		
		ImGui::Separator();
	}
	
	shaderEffect::printCustomEffectGui();
	
	return true;
}

// - - - - - - -
// CUSTOM METHODS
// - - - - - - -

// re-syncs
bool textureDistortionEffect::generateGridFromShapes(){
	//vbo.clear();
	distortedPoints.clear();
	triangulation.reset();

	if(shapes.size()>0){
		
		bool foundOne = false;
		
		for( auto sh=shapes.begin(); sh!=shapes.end(); ++sh ){
			// restrict to vertex shapes
			if( (*sh)->isType("vertexShape") ){
				vertexShape* tmpShape = static_cast<vertexShape*>(*sh);
				auto& points = tmpShape->getPoints();
				
				if(points.size()<1) continue;
				
				int ptOffset = 0;
				for( auto pt=points.begin(); pt!=points.end(); ++pt ){
					distortionPoint point;
					point.texCoordOffset = ofPoint(0,0);
					point.texCoordIndex = 0;
					point.shapeName = tmpShape->getName();
					point.shapeVertIndex = ptOffset;
					point.shapeCenter = tmpShape->getPositionUnaltered();
					
					distortedPoints.emplace(
						make_pair(&(*pt), point )
					);
					
					ptOffset++;
					
					//triangulation.addPoint(distortedPoints[point]);
				}
			}
			
		}
		
		reTriangulateFromPoints();
		
		return true;
	}
	else {
		setError(true, "No shapes to bind...");
		ofLogNotice("textureDistortionEffect::bindDistortionWithShapes") << "No vertexShapes to bind...";
	}
	return false;
}

void textureDistortionEffect::reTriangulateFromPoints(){
	
	triangulation.reset();
	
	vector<ofVec2f> textureCoords;
	textureCoords.clear();
	
	// add screen coordinates
	float res = 5.0f; // at least 1 or above, defines number of parts pers screen-side. (cleaner texture distorsion)
	float h = ofGetHeight();
	float w = ofGetWidth();
	
	unsigned int numScreenPoints = 0;
	
	for(int i=0; i<res; i++){
		ofVec2f pt((w/res)*i ,0);
		triangulation.addPoint(pt);
		textureCoords.push_back(pt);
		numScreenPoints++;
	}
	for(int i=0; i<res; i++){
		ofVec2f pt(w,(h/res)*i);
		triangulation.addPoint(pt);
		textureCoords.push_back(pt);
		numScreenPoints++;
	}
	for(int i=0; i<res; i++){
		ofVec2f pt((w/res)*(res-i), h);
		triangulation.addPoint(pt);
		textureCoords.push_back(pt);
		numScreenPoints++;
	}
	for(int i=0; i<res; i++){
		ofVec2f pt(0, (h/res)*(res-i));
		triangulation.addPoint(pt);
		textureCoords.push_back(pt);
		numScreenPoints++;
	}

	int i=0;
	for(auto pt=distortedPoints.begin();pt!=distortedPoints.end();++pt){
		triangulation.addPoint( *pt->first + *pt->second.shapeCenter );
		textureCoords.push_back(basicPoint(pt->second.texCoordOffset) + *pt->second.shapeCenter + *pt->first);
		pt->second.texCoordIndex = i+numScreenPoints;
		i++;
	}
	
	triangulation.triangulate();
	
	// set texture coords
	triangulation.triangleMesh.addTexCoords(textureCoords);
}

// - - - - - - -
// LISTENERS
// - - - - - - -
void textureDistortionEffect::mouseClicked(ofMouseEventArgs &args){
	
	if(!isReady() || !bIsInEditMode){
		return;
	}
	
	// resets click
	clicked = nullptr;
	
	if(ImGui::IsAnyItemHovered()){
		return;
	}
	
	if(distortedPoints.size()>0){
		for(auto pt=distortedPoints.begin(); pt!=distortedPoints.end(); ++pt){
			if( args.distance( pt->second.texCoordOffset + *pt->second.shapeCenter + *pt->first ) < KM_TDE_HANDLE_RADIUS){
				clicked = pt->first;
				return;
			}
		}
	}
}

void textureDistortionEffect::mouseReleased(ofMouseEventArgs &args){
	
	if(!isReady() || !bIsInEditMode){
		return;
	}
	
	if(clicked==nullptr){
		return;
	}
	
	// save position
	if(distortedPoints.find(clicked) != distortedPoints.end()){
		
		distortedPoints[clicked].texCoordOffset = ofVec2f(args) - ( (basicPoint) *(distortedPoints[clicked].shapeCenter ) + *clicked);
		
		triangulation.triangleMesh.setTexCoord(distortedPoints[clicked].texCoordIndex, args);
	}
	
	clicked = nullptr;
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool textureDistortionEffect::saveToXML(ofxXmlSettings& xml) const{
	bool ret = shaderEffect::saveToXML(xml);
	
	xml.addValue("bDrawTriangulation", bDrawTriangulation);
	
	if(distortedPoints.size()>0){
		xml.addTag("distortionPoints");
		if( xml.pushTag("distortionPoints") ){
			for(auto it=distortedPoints.begin(); it!=distortedPoints.end(); ++it){
				if(xml.pushTag("point", xml.addTag("point"))){
					xml.addValue("shapeName", it->second.shapeName);
					xml.addValue("shapeVertIndex", it->second.shapeVertIndex);
					xml.addValue("offsetX", it->second.texCoordOffset.x);
					xml.addValue("offsetY", it->second.texCoordOffset.y);
					xml.popTag();
				}
			}
			
			xml.popTag();
		}
	}
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool textureDistortionEffect::loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene){
	bool ret = shaderEffect::loadFromXML(xml, _scene);
	
	// force shader
	loadShader( effectFolder("textureDistortion.vert", "textureDistortionEffect"), effectFolder("textureDistortion.frag", "textureDistortionEffect") );
	
	// tmp, as the shader doesn't load... and is not needed yet...
	setError(false);
	
	setUsePingPong(true);
	bUseTextures = false;
	
	bDrawTriangulation = xml.getValue("bDrawTriangulation", bDrawTriangulation);
	
	if(xml.pushTag("distortionPoints")){
		if( generateGridFromShapes() ){
			
			int numPoints = xml.getNumTags("point");
			
			// check if the amount of points matches with save file
			if( numPoints==distortedPoints.size() ){
				for(int ptIndex=0; ptIndex<numPoints; ++ptIndex){
					if(xml.pushTag("point", ptIndex)){
						int targetIndex = xml.getValue("shapeVertIndex",-1);
						if(targetIndex<0){
							xml.popTag();
							continue;
						}
						auto targetShape = _scene.getShapeByName(xml.getValue("shapeName",""));
						if(targetShape==nullptr){
							// skip point on unknow shape
							continue;
						}
						if(!targetShape->isType("vertexShape")){
							// skip, cannot be a vertex shape
							continue;
						}
						
						auto ptIt = static_cast<vertexShape*>(targetShape)->getPoints().begin();
						std::advance(ptIt, targetIndex);
						
						if(ptIt==static_cast<vertexShape*>(targetShape)->getPoints().end()){
							// point doesnt exist on shape ?
							continue;
						}
						
						basicPoint* pt = &(*ptIt);
						if(distortedPoints.find(pt) != distortedPoints.end()){
							basicPoint ptCoord(xml.getValue("offsetX",0.f),xml.getValue("offsetY",0.f));
							distortedPoints[pt].texCoordOffset = ptCoord;
							triangulation.triangleMesh.setTexCoord(distortedPoints[pt].texCoordIndex, ptCoord +*distortedPoints[pt].shapeCenter + *pt );
							
						}
						
						xml.popTag();
					}
				}
			}
			
			else {
				ofLogNotice("textureDistortionEffect::loadFromXML()") << "Looks like the bound shapes have changed since last save... could not import texture distortion offsets with current bound shapes.";
			}
		}
		else {
			ofLogNotice("textureDistortionEffect::loadFromXML()") << "Unable to create grid from shapes while loading from savefile...";
		}
		xml.popTag(); // pop distortionPoints
	}
	
	return ret;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool textureDistortionEffect::randomizePresets(){
	if(!shaderEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}

// register effect type
EFFECT_REGISTER( textureDistortionEffect , "textureDistortionEffect" );
