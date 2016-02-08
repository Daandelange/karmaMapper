//
//  videoEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//

// tmp disabled
#ifdef KM_PLEASESKIPME

#pragma once

#include "ofMain.h"
#include "basicEffect.h"
#include "imageEffect.h"

class videoEffect : public imageEffect {
	
public:
	videoEffect();
	~videoEffect();
	
	// global effect functions
	bool initialise();
	bool render();
	void update();
	void reset();
	
	//bool setDirectory(string _dir);
	//bool loadRandomVideo();
	//bool loadVideo(int videoID);
	bool loadVideo(string _videoName);
	
protected:
	
	// video stuff
	//vector<string> videoFiles;
	//string directory;
	ofVideoPlayer stream;
	bool isPlaying();
	//ofShader shader;
	//float textureScale;
	
private:
	
};
// END KM_PLEASESKIPME
#endif