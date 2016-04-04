// fragment shader
 #version 150
uniform vec4 xMin;
uniform vec4 xMax;
uniform vec4 yMin;
uniform vec4 yMax;
uniform float fadeScale;
uniform vec3 size;

out vec4 outputColor;
in vec4 tex;

void main()
{   
	vec2 pt = tex.yx / (size.xy / 2.) + 1.;
	outputColor = mix(
	mix(xMax, xMin, pt.x), 
	mix(yMax, yMin, pt.y), fadeScale);
}