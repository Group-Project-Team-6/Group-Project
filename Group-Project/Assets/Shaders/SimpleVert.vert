#version 450 core
layout(binding = 0) uniform uniformBuffer{
 mat4 matris;
} ub;

layout(location = 0) in vec3 position;

layout(location = 0) out vec4 fragColor;


void main() {
    gl_Position = ub.matris * vec4(position, 1.0);
    fragColor = vec4(1,0,1,1);
}