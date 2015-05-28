#version 150

#ifdef GL_ES
precision highp float;
#endif

// incoming variables
// no need to declare these, use this as a list
layout(pixel_center_integer​) in vec4 gl_FragCoord;// <-- pixel position
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;// <-- position of the vertex that created this pixel
// in float gl_FragDepth;// <-- gl_FragCoord.z, if DEPTH enabled

// these are our textures from OF
uniform float alphaMultiplier;

// this is the output of the vertex shader
in vec4 myColor;

out vec4 outputColor;

void main(){
   	//outputColor.a *= 0.1f;
    //outputColor = vec4(1,0,0,0.99f);
    //outputColor = vec4(myColor.r, myColor.g, myColor.b, 0);
    outputColor = vec4(0,1,0,1);
    //outputColor = ve4(1,0,0,1);
}
