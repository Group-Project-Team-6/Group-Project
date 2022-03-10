#version 400 core

uniform sampler2D 	hitTex;
uniform vec2		hitUV;

in Vertex
{
	vec2 texCoord;
} IN; 

out vec4 fragColor;

void main(void)
{
	vec4 colour = texture(hitTex, IN.texCoord);
	colour.a = 0.0f;
	if(distance(IN.texCoord,hitUV)<0.5f) colour.a = 1.0f;
	fragColor = colour;
}