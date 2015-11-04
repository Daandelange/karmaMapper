#version 150

// reference (GLSL, but still useful)
// https://www.opengl.org/wiki/Built-in_Variable_%28GLSL%29

// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 orientationMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor;

// build-in incoming variables
//in int gl_VertexID;// <-- vertex index
//in int gl_InstanceID;// <-- not always set

// build-in variables you can alter
//vec4 gl_Position;// <-- vertex position
//float gl_PointSize;// <-- size/width (px) of current vertex
//float gl_ClipDistance[];// <-- clipping stuf

// from of-bound textures
in vec4 position;
in vec2 texcoord;
in vec4 color;
in vec4 normal;

// texture coordinates are sent to fragment shader
//out vec2 texCoordVarying;
//out vec4 texColor;

void main(){
	// build-in input variables
	// gl_VertexID
	// gl_InstanceID

	// Output variables
	// gl_PointSize
	// gl_Position

	//texColor = globalColor;
    //texCoordVarying = modelViewProjectionMatrix * position;
	gl_Position = modelViewProjectionMatrix * position;
}