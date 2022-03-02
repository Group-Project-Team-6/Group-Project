#version 450 core

layout(location = 0) in vec3 position;

layout(location = 0) out vec4 fragColor;


void main() {
    gl_Position = vec4(position, 1.0);
    fragColor = vec4(1,0,1,1);
}