#version 150

#ifdef GL_ES
precision highp float;
#endif

// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 orientationMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor;

// set by karmaMapper
uniform vec2 	fboCanvas; // w,h
uniform vec4 	shapeBoundingBox; // x,y,w,h
uniform vec2 	shapeCenter; // x,y
uniform int 	textureMode; // 0=scale to shape, 1=cover, 2=fill (clamed edge), 3=fill (repeat)
uniform vec4 	globalTextureTransform; // position(x,y) scale(z, w)
uniform vec2    resolution;
uniform vec2    textureResolution;
uniform vec4    effectColor;
uniform int     kmIsPingPongPass;

// shadertoy variables
// ### karmaMapper request shaderToyVariables
uniform vec3      		iResolution;           // viewport resolution (in pixels)
uniform float     		iGlobalTime;           // shader playback time (in seconds)
uniform float     		iTimeDelta;            // render time (in seconds)
uniform int       		iFrame;                // shader playback frame
uniform float     		iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      		iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4     		iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform sampler2DRect 	iChannel0;             // input channel. XX = 2D/Cube
uniform vec4      		iDate;                 // (year, month, day, time in seconds)
uniform float     		iSampleRate;           // sound sample rate (i.e., 44100)

in vec2 texCoordVarying;
in vec4 gl_FragCoord;

out vec4 outputColor;

// ### karmaMapper dont request pingPong
uniform sampler2DRect pingPongTexture;

void main()
{
    // do a ping-pong pass ?
    if( kmIsPingPongPass == 1 ){
        outputColor = texture( pingPongTexture,  (vec2(1,1)* (gl_FragCoord.xy + vec2(0, 0)) ) );
        //outputColor *= effectColor;
        //outputColor *= vec4(1,0,0,1); // make this pass red (debugging)
    }
    else {

    	float textureScale = 1.0;
    	//outputColor = vec4( texture(iChannel0, gl_FragCoord.xy/iResolution.xy ).rgb, 1);
    	//outPutColor = vec4(0.5, gl_FragCoord.x, gl_FragCoord.y);
    	//vec2 q = gl_FragCoord.xy*iChannelResolution[0].xy/iResolution.xy;
    	//outputColor = vec4( texture( iChannel0, vec2(q.x,1.0-q.y) ).rgb, 1);

    	// get rgb from tex0
    	vec2 offset = (shapeBoundingBox.xy+shapeBoundingBox.zw*vec2(0.5)) - shapeCenter;
        vec2 pos;
        if( textureMode == 0 ){ // stretch texture to fill
        	pos = (((texCoordVarying.xy-offset+globalTextureTransform.xy))/shapeBoundingBox.zw )+vec2(1)*vec2(0.5); // from 0 to 1 instead of -1 to 1
        }
        else if( textureMode== 1){ // cover, fill entire shape without distorsion
        	pos = (((texCoordVarying.xy-offset+globalTextureTransform.xy))/shapeBoundingBox.zw );

        	// dont distort
        	vec2 ratio = vec2(shapeBoundingBox.zw/iChannelResolution[0].xy);
        	vec2 scale = vec2(iChannelResolution[0].x/shapeBoundingBox.z, iChannelResolution[0].y/shapeBoundingBox.w);
        	if(scale.x>scale.y){
        		scale.x=scale.y;
        	}
        	else{
        		scale.y=scale.x;
        	}
        	scale *= globalTextureTransform.zw;
        	pos *= ratio*(scale);

        	pos = pos+vec2(1)*vec2(0.5);
        }
        else if( textureMode== 2 || textureMode== 3){ // fit, show entire image without distorsion
        	pos = (((texCoordVarying.xy-offset+globalTextureTransform.xy))/shapeBoundingBox.zw );
        	vec2 ratio = vec2(shapeBoundingBox.zw/iChannelResolution[0].xy);
        	vec2 scale = vec2(iChannelResolution[0].x/shapeBoundingBox.z, iChannelResolution[0].y/shapeBoundingBox.w);
        	if(scale.x<scale.y){
        		scale.x=scale.y;
        	}
        	else{
        		scale.y=scale.x;
        	}
        	scale *= globalTextureTransform.zw;
        	pos *= ratio*(scale);

        	if(textureMode==3){ // FIT with REPEAT
        		pos = mod(pos+vec2(1)*vec2(0.5), 1.0);
        	}
        	else { // CLAMP EDGE
        		pos = pos+vec2(1)*vec2(0.5);
        	}
        	
        }
        else pos = ( ((shapeBoundingBox.zw*vec2(0.5)+texCoordVarying)) / shapeBoundingBox.zw) * iChannelResolution[0].xy; // fit tex to screen
        //pos.y = -100.0f+pos.y;
        //outputColor = vec4( texture( iChannel0, pos ).rgb, 1);
        
        //outputColor *= vec4( mod( ((texCoordVarying.xy+offset)+vec2(shapeBoundingBox.zw*vec2(0.5)-shapeBoundingBox.xy))/shapeBoundingBox.zw, 1.0)*vec2(1,1), 0, 1 );
    	outputColor = vec4( texture( iChannel0, pos*iChannelResolution[0].xy ).rgb, 1);
        outputColor *= effectColor;
        //outputColor *= vec4(0,1,0,1);  // make this pass green (debugging)
    	//outputColor *= vec4( pos, 0, 1 );
    	//outputColor = vec4( texture(iChannel0, ( (texCoordVarying-shapeCenterOffset)/iResolution.xy*iChannelResolution[0].xy*textureScale) ).rgb, 1);//outputColor.a);
    	//outputColor.r += 0.5f; // debugging
    	//outputColor = vec4(1, 0, 0, 1);
    }
}
