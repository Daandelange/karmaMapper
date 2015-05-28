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
	
	virtual bool render();
	virtual void update();
	virtual void reset();
	
	bool setDirectory(string _dir);
	bool loadRandomVideo();
	bool loadVideo(int videoID);
	bool loadVideo(string videoName);
	
protected:
	
	// video stuff
	vector<string> videoFiles;
	string directory;
	ofVideoPlayer stream;
	ofShader shader;
	float textureScale;
	
private:
	
};
