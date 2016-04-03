#version 150
 
uniform mat4 modelViewProjectionMatrix;
uniform float size;

in vec4 position;
out vec4 tex;
out vec2 range;

void main(){
	gl_Position = modelViewProjectionMatrix * position;
	tex = position;
	range = vec2(size / 2., size / 2.);
}