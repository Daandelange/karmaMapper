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
        outputColor = texture( pingPongTexture,  (vec2(1,1)* (texCoordVarying.xy + vec2(0, 0)) ) );
        //outputColor *= effectColor;
        //outputColor *= vec4(1,0,0,1); // make this pass red (debugging)
    }
}
