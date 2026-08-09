#pragma once

#include "shader.hpp"

namespace arrows {

inline float arrowsBlueVertices[] = {
    0.25f, 0.5f, 0.0f, 1.0f,
    0.75f, 0.5f, 1.0f, 1.0f,
    0.75f, -0.5f, 1.0f, 0.0f,
    0.75f, -0.5f, 1.0f, 0.0f,
    0.25f, 0.5f, 0.0f, 1.0f,
    0.25f, -0.5f, 0.0f, 0.0f
};

inline float arrowsGreenVertices[] = {
    -0.75f, 0.5f, 0.0f, 1.0f,
    -0.25f, 0.5f, 1.0f, 1.0f,
    -0.25f, -0.5f, 1.0f, 0.0f,
    -0.25f, -0.5f, 1.0f, 0.0f,
    -0.75f, 0.5f, 0.0f, 1.0f,
    -0.75f, -0.5f, 0.0f, 0.0f
};

inline Shader shader;

inline unsigned int arrowsBlue, arrowsGreen;
inline unsigned int vertexArrayBlue, vertexArrayGreen;
inline unsigned int vertexBufferBlue, vertexBufferGreen;

void initArrows();
void drawArrows(float theta, float phi, int windowWidth, int windowHeight);

}