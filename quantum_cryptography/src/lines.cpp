#include "lines.hpp"

#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>

#include "base_path.hpp"

void lines::initLines() {
    //stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load((basePath / "quantum_cryptography/resources/line.png").string().c_str(), &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "Failed to load texture";
        return;
    }

    glGenTextures(1, &lines);
    glBindTexture(GL_TEXTURE_2D, lines);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glGenBuffers(1, &vertexBuffer1);
    glGenVertexArrays(1, &vertexArray1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer1);
    glBindVertexArray(vertexArray1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(lines1Vertices), lines1Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &vertexBuffer2);
    glGenVertexArrays(1, &vertexArray2);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2);
    glBindVertexArray(vertexArray2);

    glBufferData(GL_ARRAY_BUFFER, sizeof(lines2Vertices), lines2Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shader.load((basePath / "quantum_cryptography/shader/lines.vert.glsl").string(), (basePath / "quantum_cryptography/shader/lines.frag.glsl").string());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void lines::drawLines() {
    shader.use();
    shader.set1i("texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, lines);
    glBindVertexArray(vertexArray1);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(vertexArray2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}