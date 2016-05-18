//
//  animationControllerEvents.h
//  karmaMapper
//
//  Created by Daan on 23/02/16.
//
//

#pragma once

#include "animationParamsServer.h"
//#include "karmaFboLayer.h"

// forward declaration ?
struct animationParams;
//class karmaFboLayer;

enum karmaControllerDrawEventStage {
	DRAW_EVENT_BEFORE_DRAW,
	DRAW_EVENT_AFTER_DRAW
};

struct karmaControllerDrawEventArgs {
	animationParams& params;
	enum karmaControllerDrawEventStage stage;
    //karmaFboLayer& fbo;
	
	karmaControllerDrawEventArgs(/*karmaFboLayer& _fbo,*/ animationParams& _p) : params(_p), stage(DRAW_EVENT_BEFORE_DRAW) /*, fbo(_fbo)*/ {
		
	}
};