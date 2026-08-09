#include "shader.hpp"

#include <iostream>

rpp::Shader::~Shader() {
    glDeleteProgram(shaderProgram);
}

void rpp::Shader::load() {
    glDeleteProgram(shaderProgram);

    auto vertexShader = createShader(GL_VERTEX_SHADER);
    auto fragmentShader = createShader(GL_FRAGMENT_SHADER);

    if (!vertexShader.has_value() || !fragmentShader.has_value()) {
        std::cerr << "one of shader has no value\n";
        return;
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader.value());
    glAttachShader(shaderProgram, fragmentShader.value());
    glLinkProgram(shaderProgram);

    int programSuccess;
    char programInfoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);

    if (!programSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, programInfoLog);
        std::cerr << "Error: Shader could not be loaded: " << programInfoLog << '\n';
    }

    glDeleteShader(vertexShader.value());
    glDeleteShader(fragmentShader.value());
}

void rpp::Shader::use() {
    glUseProgram(shaderProgram);
}

std::optional<unsigned int> rpp::Shader::createShader(unsigned int shaderType) {
    unsigned int shader = glCreateShader(shaderType);

    if (shaderType == GL_FRAGMENT_SHADER)
        glShaderSource(shader, 1, &fragmentSource, NULL);
    else
        glShaderSource(shader, 1, &vertexSource, NULL);

    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Shader could not be compiled\n";
        std::cerr << infoLog << '\n';

        return {};
    }

    return shader;
}