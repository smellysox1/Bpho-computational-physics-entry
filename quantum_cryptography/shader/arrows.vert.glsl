#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform ivec2 windowSize;

uniform vec2 centre;
uniform float angle;

void main() {
    vec2 diff = vec2(pos.x * float(windowSize.x), pos.y * float(windowSize.y)) - centre;
    float arg = atan(diff.x, diff.y) + angle;

    vec2 rotatedPos = centre + length(diff) * vec2(cos(arg), sin(arg));

    gl_Position = vec4(rotatedPos.x / float(windowSize.x), rotatedPos.y / float(windowSize.y), 0.0, 1.0);
    TexCoord = texCoord;
}
