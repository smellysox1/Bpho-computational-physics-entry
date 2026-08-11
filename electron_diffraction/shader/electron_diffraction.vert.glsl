#version 330 core
layout (location = 0) in vec3 pos;

out vec2 fragPos;

void main() {
    fragPos = pos.xy;
    gl_Position = vec4(pos, 1.0);
}