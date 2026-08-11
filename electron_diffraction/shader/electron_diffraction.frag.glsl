#version 330 core
out vec4 colour;

in vec2 fragPos;

const float pi = 3.1415926535897932385;

const float r = 65e-3;
const float d = 0.123e-9;

const float f = 6.0; // for carbon

uniform float max_N;
uniform float V;

void main() {
    float theta = asin(length(fragPos)) * 0.25;

    float lambda = 1.93915e-11 / sqrt(V); // simplified calculation to reduce imprecision

    float n = 2.0 * d * sin(theta) / lambda;

    float intensity = pow(sin(pi * (n + 0.5)), 16);

    colour = (n >= max_N) ? vec4(0.0, 0.0, 0.0, 1.0) :
                            vec4(intensity * 0.5, 1.0, intensity * 0.5, intensity);
}