// fragment shader
 #version 150
uniform vec4 xMin;
uniform vec4 xMax;
uniform vec4 yMin;
uniform vec4 yMax;
uniform float fadeScale;

out vec4 outputColor;
in vec4 tex;
in vec2 range;

void main()
{   
	vec2 pt = tex.yx / range + 1.;
	outputColor = mix(
	mix(xMax, xMin, pt.x), 
	mix(yMax, yMin, pt.y), fadeScale);
}