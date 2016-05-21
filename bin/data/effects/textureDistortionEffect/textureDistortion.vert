#version 150

// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 orientationMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor;
uniform int  kmIsPingPongPass;

//in vec2 texcoord; // This is provided by openFrameworks
in vec4 position;
out vec2 texCoordVarying;
in vec2 texcoord;

void main()
{
	// Absolute window position: gl_FragCoord.x / windowWidth
	//gl_Position.xy;//	
	if(kmIsPingPongPass == 1){
		texCoordVarying = texcoord.xy;
		//texCoordVarying = position.xy;
	}
	else {
		texCoordVarying = position.xy;
	}

	gl_Position = modelViewProjectionMatrix * position;
}
