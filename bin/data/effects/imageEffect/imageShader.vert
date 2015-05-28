#version 120
 
// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
 
attribute vec4 position;
attribute vec4 color;
attribute vec4 normal;
attribute vec2 texcoord;
// this is the end of the default functionality
 
// this is something we're creating for this shader
varying vec2 varyingtexcoord;
 
// this is coming from our C++ code
uniform float mouseX;
 
void main()
{
	// vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

    // here we move the texture coordinates
    varyingtexcoord = texcoord;//vec2(gl_MultiTexCoord0.x, gl_MultiTexCoord0.y);
 
    // send the vertices to the fragment shader
    //gl_Position = modelViewProjectionMatrix * position;
    //gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
}