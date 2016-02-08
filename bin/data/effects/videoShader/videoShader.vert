#version 330

// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 orientationMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor;

//in vec2 texcoord; // This is provide by openFrameworks
in vec4 position;
out vec2 texCoordVarying;

void main()
{
	// Absolute window position: gl_FragCoord.x / windowWidth
	//gl_Position.xy;//	

    texCoordVarying = position.xy;
	gl_Position = modelViewProjectionMatrix * position;
}
