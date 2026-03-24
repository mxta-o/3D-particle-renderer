#include "app/Application.h"
#include "render/Renderer.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

Application::Application(GLFWwindow* window)
    : window_(window) {
    renderer_ = std::make_unique<render::Renderer>();
}

Application::~Application() = default;

void Application::run() {
    while (!glfwWindowShouldClose(window_)) {
        int w, h;
        glfwGetFramebufferSize(window_, &w, &h);
        glViewport(0, 0, w, h);

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer_->render();

        glfwPollEvents();
        glfwSwapBuffers(window_);
    }
}
