#version 330

uniform mat4 modelViewProjectionMatrix; // This is provide by openFrameworks
in vec2 texcoord; // This is provide by openFrameworks
in vec4 position;
out vec2 vertexTexCoord;

void main()
{
	// Absolute window position: gl_FragCoord.x / windowWidth
	//gl_Position.xy;//	
	vertexTexCoord = position.xy;
    gl_Position = modelViewProjectionMatrix * position;
}
