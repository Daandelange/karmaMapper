//
//  videoEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

#pragma once

#include "ofMain.h"
#include "basicEffect.h"

class videoEffect : public basicEffect {
	
public:
	videoEffect();
	~videoEffect();
	
	virtual void spawn();
	virtual void render();
	virtual void update();
	virtual void reset();
	virtual void destroy();
	
	bool setDirectory(string _dir);
	bool loadRandomVideo();
	bool loadVideo(int videoID);
	bool loadVideo(string videoName);
	
protected:
	vector<string> videoFiles;
	string directory;
	ofVideoPlayer stream;
	ofShader shader;
};
