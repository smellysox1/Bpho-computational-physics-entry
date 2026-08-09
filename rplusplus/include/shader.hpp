#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <optional>
#include <iostream>

namespace rpp {
class Shader {
public:
    ~Shader();
    void load();
    void use();

    inline void set1f(const char* name, float v0) {
        glUniform1f(glGetUniformLocation(shaderProgram, name), v0);
    }

    inline void set2f(const char* name, float v0, float v1) {
        glUniform2f(glGetUniformLocation(shaderProgram, name), v0, v1);
    }

    inline void set4f(const char* name, float v0, float v1, float v2, float v3) {
        glUniform4f(glGetUniformLocation(shaderProgram, name), v0, v1, v2, v3);
    }

    inline GLuint getShaderProgram() const {
        return shaderProgram;
    }

private:
    std::optional<unsigned int> createShader(unsigned int shaderType);

private:
    GLuint shaderProgram;

    const char* vertexSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"

"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"

"void main()\n"
"{\n"
	"gl_Position = vec4(aPos, 1.0);\n"
	"ourColor = aColor;\n"
	"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}";

    const char* fragmentSource = "#version 330 core\n"
"out vec4 FragColor;\n"

"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"

"void main()\n"
"{\n"
	"FragColor = texture(texture1, TexCoord);\n"
"}\n";
};
}