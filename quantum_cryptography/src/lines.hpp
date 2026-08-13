#pragma once

#include "shader.hpp"

namespace lines {

inline float lines1Vertices[] = {
    0.25f, 0.5f, 0.0f, 1.0f,
    0.75f, 0.5f, 1.0f, 1.0f,
    0.75f, -0.5f, 1.0f, 0.0f,
    0.75f, -0.5f, 1.0f, 0.0f,
    0.25f, 0.5f, 0.0f, 1.0f,
    0.25f, -0.5f, 0.0f, 0.0f
};

inline float lines2Vertices[] = {
    -0.75f, 0.5f, 0.0f, 1.0f,
    -0.25f, 0.5f, 1.0f, 1.0f,
    -0.25f, -0.5f, 1.0f, 0.0f,
    -0.25f, -0.5f, 1.0f, 0.0f,
    -0.75f, 0.5f, 0.0f, 1.0f,
    -0.75f, -0.5f, 0.0f, 0.0f
};

inline Shader shader;

inline unsigned int lines;
inline unsigned int vertexArray1, vertexArray2;
inline unsigned int vertexBuffer1, vertexBuffer2;

void initLines();
void drawLines();

}