#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 color;

out vec4 myColor;

void main(void) {
  myColor = color;
  gl_Position = modelViewProjectionMatrix * position;
}