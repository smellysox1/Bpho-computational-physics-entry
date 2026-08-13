#version 330 core
out vec4 colour;

in vec2 fragPos;

const float pi = 3.1415926535897932385;

const float r = 65e-3;
const float d_1 = 0.123e-9;
const float d_2 = 0.213e-9;

const float f = 6.0; // for carbon

uniform float max_N;
uniform float V;

void main() {
    float theta = asin(length(fragPos)) * 0.25;

    float lambda = 1.93915e-11 / sqrt(V); // simplified calculation to reduce imprecision

    float n_1 = 2.0 * d_1 * sin(theta) / lambda;
    float n_2 = 2.0 * d_2 * sin(theta) / lambda;

    float intensity_1 = pow(sin(pi * (n_1 + 0.5)), 16);
    float intensity_2 = pow(sin(pi * (n_2 + 0.5)), 16);

    float intensity = 0.5 * (intensity_1 + intensity_2);

    colour = (n_1 >= max_N && n_2 >= max_N) ? vec4(0.0, 0.0, 0.0, 1.0) :
                            vec4(intensity * 0.5, 1.0, intensity * 0.5, intensity);
}