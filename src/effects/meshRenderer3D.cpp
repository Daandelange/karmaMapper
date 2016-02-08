//
//  MeshRenderer3D.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Bla bla bla
//

// tmp disabled
#ifdef KM_PLEASESKIPME

#include "meshRenderer3D.h"

MeshRenderer3D::MeshRenderer3D(){
	basicEffect::basicEffect();
	
	// effectType must match the class name
	effectType = "MeshRenderer3D";
	
	hasError = !grabShape();
	
	ofAddListener(ofx::AbletonLiveSet::EventHandler::noteEvent, this, &MeshRenderer3D::noteEventListener);
	
	// tmp
	hasError = hasError ? true : !loadModel("effects/meshRenderer3D/palm_tree.dae");
}

MeshRenderer3D::~MeshRenderer3D(){
	
	ofRemoveListener(ofx::AbletonLiveSet::EventHandler::noteEvent, this, &MeshRenderer3D::noteEventListener);
}

// update --> animation
// overrule this function with your own
bool MeshRenderer3D::render(){
	if( shapes.size()==0 || !isReady() ) return;
	
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
		
		ofScale( modelScale[i], -modelScale[i], modelScale[i] );
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
	
}

void MeshRenderer3D::update(){
	// you can chose to do the default behaviour too or not by commenting this out
	basicEffect::update();
	
}

// resets all values
// overrule this function with your own.
void MeshRenderer3D::reset(){
	basicEffect::reset();
}

bool MeshRenderer3D::grabShape(){
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

bool MeshRenderer3D::loadModel(string file){
	ofxAssimpModelLoader model;
	mesh.clear();
	
	if( model.loadModel( file ) && model.hasMeshes() ){
		
		// calc boundary
		//model.calculateDimensions();
		//min= model.getSceneMin();
		//max= model.getSceneMax();
		//meshContainer.set( min.+abs(max) );
		
		// grab biggest mesh from 3D file
		int biggestMesh = -1;
		for(int i=0; i<model.getMeshCount(); i++){
			ofMesh tmp = model.getMesh(i);
			
			if(tmp.getNumVertices() > biggestMesh) mesh = tmp;
			
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
		
		return true;
	}
	
	return false;
}

void MeshRenderer3D::noteEventListener(ofx::AbletonLiveSet::LSNoteEvent &noteEvent){
	cout << "ok!" << endl;
}

// END KM_PLEASESKIPME
#endif