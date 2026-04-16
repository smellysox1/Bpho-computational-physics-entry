#version 330 core
out vec4 FragColor;

const float pi = 3.1415926535897932385;

const float r = 65e-3;
const float d = 0.123e-9;
const float m_e = 9.109383714e-31;
const float e = 1.602176634e-19;
const float h = 6.62607015e-34;
const float V = 1000.0;

const float f = 6.0; // for carbon

uniform float max_N;

const float SCALE_FACTOR = 400.0;
const vec2 ORIGIN_WINDOW_COORDS = vec2(600, 400);

void main() {
    vec2 pixel = (gl_FragCoord.xy - ORIGIN_WINDOW_COORDS) / SCALE_FACTOR;

    float theta = asin(length(pixel)) * 0.25;

    float sinTheta = sin(theta);
    float cos2theta = cos(2.0 * theta);
    float cosTheta = cos(theta);

    float Lp = (1.0 + cos2Theta * cos2Theta) / (8.0 * sinTheta * sinTheta * cosTheta);

    float n = d * sinTheta * 1630754776.0 * sqrt(V);

    float m = 6.0; // multiplicity factor
    float A; // absorption correction
    const float X = 0.5;
    const float cosPiX = cos(pi * X);
    const float absFsquared = f * f * 4.0 * cosPiX * cosPiX;

    float intensity = absFsquared * m * Lp * A;

    if (n >= max_N) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        FragColor = vec4(0.0, 1.0 - intensity / 2000.0, 0.0, 1.0);
    }
}