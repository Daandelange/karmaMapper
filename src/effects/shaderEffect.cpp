//
//  shaderEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

#include "shaderEffect.h"

shaderEffect::shaderEffect(){
	basicEffect::basicEffect();
}

shaderEffect::~shaderEffect(){
	basicEffect::~basicEffect();
}

// spawns the effect @ the scene so it starts rendering (setup)
// overrule this function with your own.
void shaderEffect::spawn(){
	basicEffect::spawn();
}

// update --> animation
// overrule this function with your own.
void shaderEffect::render(){
	
	if( !shader.isLoaded() ){
		ofLogNotice("shaderEffect::registerShaderVariables() --> shader not loaded");
		pShape->hasError = true;
		basicEffect::render();
		return;
	}
	else pShape->hasError = false;
	
	// load shader
	shader.begin();
	
	registerShaderVariables();
	
	// draw shape so GPU gets their vertex data
	pShape->render();
	
	// flush the pipeline! :D
	shader.end();
}

void shaderEffect::update(){
	basicEffect::update();
}

// resets all values
// overrule this function with your own.
void shaderEffect::reset(){
	basicEffect::reset();
}

// called just before removal
void shaderEffect::destroy(){
	basicEffect::destroy();
}

void shaderEffect::registerShaderVariables(){
	
	if( !shader.isLoaded() ){
		ofLogWarning("shaderEffect::registerShaderVariables() --> shader not loaded or linked");
		return;
	}
	
	//if(shader==NULL) return;
	
	 shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
	 shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
	 
	 shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(), 0);
	 shader.setUniform4f("iMouse", ofGetMouseX(), ofGetMouseY(), 0, 0 );
	 shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
	 shader.setUniform1i("tex", 0);
}

bool shaderEffect::loadShader(string _vert, string _frag){
	shader.load(_vert, _frag);
	//shader.setupShaderFromFile(GL_FRAGMENT_SHADER, _frag);
	
	return true;
	
	//cout << "OK:" << shader.linkProgram() << endl;
	//cout << shader.isLoaded() << shader.linkProgram() << endl;
	//shader.checkProgramInfoLog( shader.getProgram() );
}


