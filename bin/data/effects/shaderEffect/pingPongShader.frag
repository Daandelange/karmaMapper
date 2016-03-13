#version 150

layout(origin_upper_left) in vec4 gl_FragCoord;// <-- pixel position (layout fixes inverted y)

uniform sampler2DRect tex0;
uniform vec2 tex0Resolution; // w,h
// uniform sampler2DRect tex1;
// uniform sampler2DRect tex2;
// uniform sampler2DRect imageMask;

in vec2 texCoordVarying;

// set by karmaMapper
uniform vec2 	fboCanvas; // w,h
// uniform vec4 	shapeBoundingBox; // x,y,w,h
// uniform vec2 	shapeCenter; // x,y
// uniform int 	textureMode; // 0=scale to shape, 1=cover, 2=fill (clamed edge), 3=fill (repeat)
// uniform vec4 	globalTextureTransform; // position(x,y) scale(z, w)
//uniform vec2	resolution;
// uniform vec2	textureResolution;
uniform int		kmIsPingPongPass;
uniform vec4    effectColor;

out vec4 outputColor;

// ### karmaMapper request pingPong
uniform sampler2DRect pingPongTexture;

void main()
{
    //vec2 pos = (texCoordVarying.xy/resolution.xy )+vec2(1)*vec2(0.5); // from 0 to 1 instead of -1 to 1

	//outputColor = texture( tex0, pos );
	//outputColor = texture( pingPongTexture, texCoordVarying ); // OK with fbo.texture.draw()
	//outputColor = texture( pingPongTexture, vec2(0, fboCanvas.y) + vec2(1,-1)* gl_FragCoord.xy ); // quite ok with drawRect() + uniform.texture
	outputColor = texture( pingPongTexture,  (vec2(1,-1)* (gl_FragCoord.xy - vec2(0, fboCanvas.y)) ) );
	//outputColor = vec4(gl_FragCoord.x/fboCanvas.x, gl_FragCoord.y/fboCanvas.y, 0, 1);
    //outputColor = vec4( texture( pingPongTexture, pos*resolution ).rgb, 1);
	//outputColor += vec4(-.3,1,.5,0);
	//outputColor = vec4(1,1,1,outputColor.a*2)-outputColor; // invert colors
	//outputColor = vec4(1,0,0,outputColor.a);
	//outputColor *= vec4(1,1,1,1.5);
	//outputColor = vec4(1,0,0,1);
}