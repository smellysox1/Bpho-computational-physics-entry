#include "logger.hpp"


std::string logger::pad(int n) {
    if (n < 10) return "0" + std::to_string(n);
    else return std::to_string(n);
}

void logger::GLFWErrorCallback(int code, const char* description) {
    WC2_ERROR("GLFW error code ", code, ": ", description, "\n");
}