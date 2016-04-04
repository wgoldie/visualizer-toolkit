#version 150
 
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
out vec4 tex;

void main(){
	gl_Position = modelViewProjectionMatrix * position;
	tex = position;
}