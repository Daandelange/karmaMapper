#version 330

#ifdef GL_ES
precision highp float;
#endif

uniform sampler2DRect tex0; // video layer
uniform vec2 resolution;
uniform vec2 textureResolution;
uniform vec2 shapeCenterOffset;
uniform float textureScale;

in vec2 vertexTexCoord;
in vec4 gl_FragCoord;

out vec4 outputColor;

void main()
{
	//outputColor = vec4( texture(tex0, gl_FragCoord.xy/resolution.xy ).rgb, 1);
	outputColor = vec4( texture(tex0, mod( (vertexTexCoord-shapeCenterOffset)/resolution*textureResolution*textureScale, textureResolution) ).rgb, 1);//outputColor.a);
	//outputColor.r += 0.15f; // debugging
}
