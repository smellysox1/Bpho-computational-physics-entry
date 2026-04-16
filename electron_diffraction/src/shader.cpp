#include "shader.hpp"

#include <iostream>

Shader::~Shader() {
    glDeleteProgram(shaderProgram);
}

void Shader::load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    glDeleteProgram(shaderProgram);

    auto vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderPath);
    auto fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

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

void Shader::use() {
    glUseProgram(shaderProgram);
}

std::optional<unsigned int> Shader::createShader(unsigned int shaderType, const std::string& path) {
    unsigned int shader = glCreateShader(shaderType);

    std::string shaderSource = fetch(path);
    const char* shaderSourceCString = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourceCString, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Shader '" << path << "' could not be compiled\n";
        std::cerr << infoLog << '\n';

        return {};
    }

    return shader;
}

std::string Shader::fetch(const std::string& path) {
    std::ifstream file(path.data());
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}