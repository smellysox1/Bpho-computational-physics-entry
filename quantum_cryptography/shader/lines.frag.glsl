#version 330 core
out vec4 colour;

in vec2 TexCoord;

uniform sampler2D texture;

void main() {
    colour = texture(texture, TexCoord);
}