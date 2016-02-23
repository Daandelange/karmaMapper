//
//  animationControllerEvents.h
//  karmaMapper
//
//  Created by Daan on 23/02/16.
//
//

#pragma once

#include "animationParamsServer.h"

enum karmaControllerDrawEventStage {
	DRAW_EVENT_BEFORE_DRAW,
	DRAW_EVENT_AFTER_DRAW
};

struct karmaControllerDrawEventArgs {
	animationParams& params;
	enum karmaControllerDrawEventStage stage;
	
	karmaControllerDrawEventArgs(animationParams& _p) : params(_p), stage(DRAW_EVENT_BEFORE_DRAW) {
		
	}
};