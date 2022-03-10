#version 400 core

uniform sampler2D 	hitTex;
uniform vec3		hitPos;

in Vertex
{
	vec2 texCoord;
	vec3 position;
} IN; 

out vec4 fragColor;

void main(void)
{
	vec4 colour = texture(hitTex, IN.texCoord);
	colour.a = 0.0f;
	if(distance(hitPos,IN.position)<0.2f)  colour.a = 1.0f;
	fragColor = vec4(0,0,0,1);//colour;
}