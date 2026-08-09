#pragma once

#include "shader.hpp"

namespace planes {
    inline unsigned int numPlanes;

    inline float vertices[] = {
        1.0f, 1.0f,
        1.0f, -1.0f,
        -1.0f, -1.0f,
        -1.0f, 1.0f
    };

    inline unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    inline unsigned int vertexArray;
    inline unsigned int vertexBuffer;
    inline unsigned int elementBuffer;

    void init(unsigned int numPlanes, Shader& shader);

    void drawPlanes();
}