//
//  shaderEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "basicEffect.h"

class shaderEffect : public basicEffect {
	
public:
	shaderEffect();
	~shaderEffect();
	
	virtual void spawn();
	virtual void render();
	virtual void update();
	virtual void reset();
	virtual void destroy();
	
	bool loadShader(string _vert, string _frag);
	virtual void registerShaderVariables();
	
protected:
	ofShader shader;
	
};
