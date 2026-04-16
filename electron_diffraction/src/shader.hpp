#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <optional>
#include <iostream>

class Shader {
public:
    ~Shader();
    void load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
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
    std::optional<unsigned int> createShader(unsigned int shaderType, const std::string& path);
    std::string fetch(const std::string& path);

private:
    GLuint shaderProgram;
};