#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <filesystem>
#include <cmath>
#include <thread>
#include <chrono>

#include "shader.hpp"
#include "planes.hpp"
#include "input.hpp"
#include "constants.hpp"

int windowWidth = 1600, windowHeight = 800;
const auto basePath = std::filesystem::current_path().parent_path();
Shader shader;

int n = 2, l = 1, m = 1, A = 1, Z = 1;
double M = A * u;
double μ = M * m_e / (M + m_e);
double a = 1e10 * eps_0 * h * h / (pi * e * e * μ * Z);
float maxPD = 1.0f;

float size = 1.0f;

glm::mat4 perspective = glm::perspective(2.0f, (float)windowWidth / (float)windowHeight, 0.015625f, 10.0f);

float azimuth = 0.0f, zenith = 0.0f;
float distance = -2.0f;

void limitFPS();

void GLFWErrorCallback(int code, const char* description) {
    std::cerr << "GLFW error code " << code << ": " << description << '\n';
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwSetErrorCallback(GLFWErrorCallback);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Hydrogenic Orbitals", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialise glad\n";
        return 1;
    }

    shader.load(
        (basePath / "hydrogenic_orbitals/shader/orbitals.vert.glsl").string(),
        (basePath / "hydrogenic_orbitals/shader/orbitals.frag.glsl").string()
    );

    shader.use();

    shader.set1i("n", n);
    shader.set1i("l", l);
    shader.set1i("m", m);
    shader.set1i("A", A);
    shader.set1i("Z", Z);
    shader.set1f("an", (float)a * n);
    shader.set1f("size", size);
    shader.set1f("maxPD", maxPD);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    planes::init(128, shader);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = NULL;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4 modelView = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, distance});
        modelView = glm::rotate(modelView, azimuth, glm::vec3(1.0f, 0.0f, 0.0f));
        modelView = glm::rotate(modelView, zenith, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 matrixModelViewProjection = perspective * modelView;

        shader.setMat4("projection", glm::value_ptr(matrixModelViewProjection));

        int cameraPlaneID = (int)std::floor(0.5f * (distance * std::sin(azimuth - pi * 0.5f) + 1.0f) * planes::numPlanes);

        shader.set1i("cameraPlaneID", cameraPlaneID);

        planes::drawPlanes();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize({0.0f, 0.0f});
        ImGui::Begin("Hydrogenic Orbitals");

        if (ImGui::DragFloat("Size", &size, 0.0625f)) {
            size = std::max(size, 0.0625f);
            shader.set1f("size", size);
        }

        if (ImGui::DragFloat("Max Probability Density", &maxPD, 0.015625f)) {
            maxPD = std::max(maxPD, 0.015625f);
            shader.set1f("maxPD", maxPD);
        }

        if (ImGui::DragInt("n", &n, 1.0f, l + 1, 2147483647)) {
            shader.set1i("n", n);
            shader.set1f("an", (float)a * n);
        }

        if (ImGui::DragInt("l", &l, 1.0f, 0, 2147483647))
            shader.set1i("l", l);

        if (ImGui::DragInt("m", &m, 1.0f, -l, l))
            shader.set1i("m", m);

        if (ImGui::DragInt("A", &A, 1.0f, 0)) {
            shader.set1i("A", A);

            M = u * A;
            μ = m_e * M / (M + m_e);
            a = 1e10 * eps_0 * h * h / (pi * e * e * μ * Z);

            shader.set1f("an", (float)a * n);
        }

        if (ImGui::DragInt("Z", &Z, 1.0f, 0)) {
            shader.set1i("Z", Z);

            a = 1e10 * eps_0 * h * h / (pi * e * e * μ * Z);

            shader.set1f("an", (float)a * n);
        }

        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        limitFPS();
    }

    glDeleteVertexArrays(1, &planes::vertexArray);
    glDeleteBuffers(1, &planes::vertexBuffer);
    glDeleteBuffers(1, &planes::elementBuffer);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}

void limitFPS() {
    // 60 frames per second, in nanoseconds
    static const unsigned int targetFrameLength = 16666667;

    static auto frameStartTime = std::chrono::steady_clock::now();

    auto now = std::chrono::steady_clock::now();
    auto diff = now - frameStartTime;

    frameStartTime = now;

    // prevent integer overflow
    if (diff.count() >= targetFrameLength)
        return;

    std::this_thread::sleep_for(std::chrono::nanoseconds(targetFrameLength) - diff);
}