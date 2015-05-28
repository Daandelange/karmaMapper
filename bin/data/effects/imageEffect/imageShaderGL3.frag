#version 150

#ifdef GL_ES
precision highp float;
#endif

// incoming variables
// no need to declare these, use this as a list
layout(origin_upper_left) in vec4 gl_FragCoord;// <-- pixel position (layout fixes inverted y)
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;// <-- position of the vertex that created this pixel
// in float gl_FragDepth;// <-- gl_FragCoord.z, if DEPTH enabled

// these are our textures from OF
uniform sampler2DRect tex0;
uniform vec2 tex0Resolution; // w,h
uniform vec2 fboCanvas; // w,h
uniform vec4 shapeBoundingBox; // x,y,w,h
uniform int fitMode; // 0=fit to screen, 1=fit-to-shape, 2=crop-to-shape
uniform float opacity;

// this comes from the vertex shader
in vec2 texCoordVarying;
in vec4 texColor;

// this is the output of the fragment shader
out vec4 outputColor;

void main(){
	
    // get rgb from tex0
    vec2 pos;
    if(fitMode==1) pos = ((gl_FragCoord.xy-shapeBoundingBox.xy) / floor(shapeBoundingBox.zw) ) * tex0Resolution; // fit tex to shape 
    else pos = ( (gl_FragCoord.xy-shapeBoundingBox.xy) / shapeBoundingBox.zw) * tex0Resolution; // fit tex to screen

    outputColor = vec4(texture(tex0, pos).rgb, texColor.a*opacity);
    
    //outputColor = vec4(offset.x, offset.y,0,1);


}
