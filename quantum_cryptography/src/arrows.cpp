#include "arrows.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>

#include "base_path.hpp"

void arrows::initArrows() {
    //stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load((basePath / "quantum_cryptography/resources/arrows_blue.png").string().c_str(), &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "Failed to load texture";
        return;
    }

    glGenTextures(1, &arrowsBlue);
    glBindTexture(GL_TEXTURE_2D, arrowsBlue);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    data = stbi_load((basePath / "quantum_cryptography/resources/arrows_green.png").string().c_str(), &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "Failed to load texture";
        return;
    }

    glGenTextures(1, &arrowsGreen);
    glBindTexture(GL_TEXTURE_2D, arrowsGreen);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glGenBuffers(1, &vertexBufferBlue);
    glGenVertexArrays(1, &vertexArrayBlue);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferBlue);
    glBindVertexArray(vertexArrayBlue);

    glBufferData(GL_ARRAY_BUFFER, sizeof(arrowsBlueVertices), arrowsBlueVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &vertexBufferGreen);
    glGenVertexArrays(1, &vertexArrayGreen);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferGreen);
    glBindVertexArray(vertexArrayGreen);

    glBufferData(GL_ARRAY_BUFFER, sizeof(arrowsGreenVertices), arrowsGreenVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shader.load((basePath / "quantum_cryptography/shader/arrows.vert.glsl").string(), (basePath / "quantum_cryptography/shader/arrows.frag.glsl").string());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void arrows::drawArrows(float theta, float phi, int windowWidth, int windowHeight) {
    shader.use();
    shader.set2i("windowSize", windowWidth, windowHeight);

    shader.set1i("texture", 0);

    shader.set2f("centre", -0.5f * (float)windowWidth, 0.0f);
    shader.set1f("angle", -theta);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, arrowsGreen);
    glBindVertexArray(vertexArrayGreen);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    shader.set2f("centre", 0.5f * (float)windowWidth, 0.0f);
    shader.set1f("angle", -phi);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, arrowsBlue);
    glBindVertexArray(vertexArrayBlue);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}