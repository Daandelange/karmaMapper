//
//  meshRenderer3D.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Bla bla bla
//

#include "meshRenderer3D.h"

meshRenderer3D::meshRenderer3D(){
	meshRenderer3D::reset();
}

meshRenderer3D::~meshRenderer3D(){
	
	//ofRemoveListener(ofx::AbletonLiveSet::EventHandler::noteEvent, this, &meshRenderer3D::noteEventListener);
}

// initialises the effect
bool meshRenderer3D::initialise(const animationParams& params){
	// init values
	shaderEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	meshRenderer3D::reset();
	
	return bInitialised;
}

// overrule this function with your own
bool meshRenderer3D::render(karmaFboLayer& renderLayer, const animationParams &params){
	
	if(!isReady()){
		return false;
	}
	
	if( 0 >= shapes.size() || !mesh.hasVertices()){
		return false;
	}
	
	renderLayer.begin();
	// setup 3D drawing environment
	//ofEnableAntiAliasing();
	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	//ofEnableAlphaBlending();
	ofEnableDepthTest();
	glShadeModel(GL_SMOOTH); //some model / light stuff
	
	ofSetSmoothLighting(true);
	
	ofEnableLighting();
	light.enable();
	//ofEnableSeparateSpecularLight();
	ofSetColor(255);
	ofFill();
	
	//glEnable(GL_COLOR_MATERIAL); // call after ofEnableLighting || light.enable();
	
	// rendomize  colors
	vector<ofFloatColor> myColors;
	for(int i=0; i<mesh.getNumVertices(); i++){
		ofFloatColor c;
		// fmod is % operator for floats
		c.setHsb( std::fmod( (.002f*i)+ ofGetElapsedTimef()/4 , 1.f), .8f, .8f);
		myColors.push_back( c );
	}
	mesh.clearColors();
	mesh.addColors(myColors);
	
	int i=0;
	for(vector<basicShape*>::iterator shape=shapes.begin(); shape!=shapes.end(); shape++, i++){
		//(*shape)->render();
		
		ofPushMatrix();

		basicPoint* shapePos = (*shape)->getPositionPtr();
		ofTranslate(shapePos->x, shapePos->y, 0);
		
		ofVec3f meshOffset = min+meshSize/2;
		
		//ofSetColor(255,0,0,190);
		//ofDrawRectangle(-2,-2,-.00001f, 4,4);
		
		ofRotateY(angle);
		
		if(modelScale.size()>i){
			ofScale( modelScale[i], -modelScale[i], modelScale[i] );
		}
		ofTranslate( -meshOffset.x, -meshOffset.y, 0 );
		
		//ofSetColor(0,255,0,90);
		//ofDrawRectangle(min, meshSize.x, meshSize.y);

		mesh.draw( OF_MESH_FILL );
		
		ofPopMatrix();
	}
	
	angle += 0.7f;
	
	//ofDisableSeparateSpecularLight();
	light.disable();
	ofDisableLighting();
	ofDisableDepthTest();
	//ofDisableAlphaBlending();
	//ofDisableBlendMode();
	//ofDisableAntiAliasing();
	renderLayer.end();
}

void meshRenderer3D::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	// do basic Effect functionality
	shaderEffect::update( renderLayer, params );
	
}

// resets all values
// overrule this function with your own.
void meshRenderer3D::reset(){
	
	shaderEffect::reset();
	
	// effect type must match with class
	effectType = "meshRenderer3D";
	
	//loadShader( effectFolder("meshRenderer3D.vert", "meshRenderer3D"), effectFolder("meshRenderer3D.frag", "meshRenderer3D") );
	
	setError(false); // tmp
	// force some shader settings
	//setUsePingPong(true);
	
	meshPath = "";
	bInitialised = loadModel( effectFolder("palm_tree.dae"));
	
	// set this when done
	//bInitialised = true;
	bIsLoading = false;
}

// - - - - - - -
// GUI STUFF
// - - - - - - -

// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool meshRenderer3D::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUImeshRenderer3DEffectPanel, "GUImeshRenderer3DEffectPanel", true, true ) ){
		
		ImGui::TextWrapped("Adds floating 3D models on shapes.");
		ImGui::TextWrapped("Bla bla bla...");
		
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Settings :");
		ImGui::Indent();{
			
			ImGui::Text("Loaded mesh:\t%s", meshPath.c_str());
			
			if(ImGui::Button("Load Mesh...")){
				setError(false); // tmp
			}
			if(meshPath.length()>0){
				ImGui::SameLine();
				if(ImGui::Button("Reload")){
					loadModel(meshPath);
				}
			}
			
			ImGui::Separator();
			ImGui::Separator();
			
			ImGui::TextWrapped("Mesh information:");
			ImGui::TextWrapped("Vertices: %lu", mesh.getNumVertices());
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

bool meshRenderer3D::grabShape(){
	// clear
	shapes.clear();
	shapes.resize(0);
	
	// we just want all vertex shapes
	//shapes = shapesHandler::getInstance().getShapesByType("vertexShape");
	
	// and look for a shapeType id
	for(vector<basicShape*>::reverse_iterator it=shapes.rbegin(); it!=shapes.rend(); it++){
		
		if( (*it)->getGroupID() != 2 && (*it)->getGroupID() != 1 && (*it)->getGroupID() != 4 ){
			it = vector<basicShape*>::reverse_iterator( shapes.erase( it.base() - 1) );
			it--;
		}
	}
	
	return shapes.size() > 0;
}

bool meshRenderer3D::loadModel(string file){
	ofxAssimpModelLoader model;
	mesh.clear();
	
	if( model.loadModel( file ) && model.hasMeshes() ){
		
		// calc boundary
		//model.calculateDimensions();
		//min= model.getSceneMin();
		//max= model.getSceneMax();
		//meshContainer.set( min.+abs(max) );
		
		// grab biggest mesh from 3D file (most detailed)
		int biggestMesh = -1;
		for(int i=0; i<model.getMeshCount(); i++){
			if(biggestMesh >= model.getMesh(i).getNumVertices() ){
				mesh = model.getMesh(i);
			}
		}
		
		// calc mesh boundingbox
		// todo: what if mesh is below origin ?
		min.set(0,0,0);
		max.set(0,0,0);
		for(int i=0; i<mesh.getNumVertices(); i++){
			ofVec3f* v = &mesh.getVerticesPointer()[i];
			if(min.x > v->x) min.x = v->x;
			else if (max.x < v->x) max.x = v->x;
			if(min.y > v->y) min.y = v->y;
			else if (max.y < v->y) max.y = v->y;
			if(min.z > v->z) min.z = v->z;
			else if (max.z < v->z) max.z = v->z;
		}
		meshSize.set( abs(min.x-max.x), abs(min.y-max.y), abs(min.z-max.z) );
		
		// rendomize  colors
		vector<ofFloatColor> myColors;
		for(int i=0; i<mesh.getNumVertices(); i++){
			ofFloatColor c;
			// fmod is % operator for floats
			c.setHsb( std::fmod( (.002f*i), 1.f), .8f, .8f);
			myColors.push_back( c );
		}
		mesh.clearColors();
		mesh.addColors(myColors);
		
		// calc scales
		modelScale.clear();
		for(int i=0; i<shapes.size(); i++){
			float scaleX = shapes[i]->getBoundingBox().width/meshSize.x;
			float scaleY = shapes[i]->getBoundingBox().height/meshSize.y;
			modelScale.push_back( scaleX<scaleY?scaleX:scaleY );
		}
		
		// add glitchy effect
		mesh.setMode(OF_PRIMITIVE_LINE_LOOP );
		
		meshPath = file;
		
		return true;
	}
	
	return false;
}

// - - - - - - -
// LISTENERS
// - - - - - - -

//void meshRenderer3D::noteEventListener(ofx::AbletonLiveSet::LSNoteEvent &noteEvent){
//	cout << "ok!" << endl;
//}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool meshRenderer3D::saveToXML(ofxXmlSettings& xml) const{
	bool ret = shaderEffect::saveToXML(xml);
	
	xml.addValue("meshPath", meshPath);
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool meshRenderer3D::loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene){
	bool ret = shaderEffect::loadFromXML(xml, _scene);
	
	// tmp, don't let shaderEffect create errors
	setError(false);
	
	bUseTextures = false;
	
	if(!loadModel(xml.getValue("meshPath", meshPath))){
		setError(true, "Can't load mesh file!");
		ret = false;
	}
	
	return ret;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool meshRenderer3D::randomizePresets(){
	if(!shaderEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}


// register effect type
EFFECT_REGISTER( meshRenderer3D , "meshRenderer3D" );
