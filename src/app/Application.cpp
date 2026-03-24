#include "app/Application.h"
#include "render/Renderer.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "app/Camera.h"
#include "sim/ParticleSystem.h"

static void glfw_mouse_button_cb(GLFWwindow* w, int button, int action, int mods) {
    (void)mods;
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(w));
    if (!app) return;
    double x, y; glfwGetCursorPos(w, &x, &y);
    app->handleMouseButton(button, action, x, y);
}

static void glfw_cursor_pos_cb(GLFWwindow* w, double x, double y) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(w));
    if (!app) return;
    app->handleCursorPos(x, y);
}

static void glfw_scroll_cb(GLFWwindow* w, double x, double y) {
    (void)x;
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(w));
    if (!app) return;
    app->handleScroll(y);
}

Application::Application(GLFWwindow* window)
    : window_(window) {
    renderer_ = std::make_unique<render::Renderer>();
    camera_ = std::make_unique<Camera>(45.0f, 800.0f/600.0f);
    particleSystem_ = std::make_unique<ParticleSystem>(20000);
    renderer_->initParticleBuffers(20000);
    glfwSetWindowUserPointer(window_, this);
    glfwSetMouseButtonCallback(window_, glfw_mouse_button_cb);
    glfwSetCursorPosCallback(window_, glfw_cursor_pos_cb);
    glfwSetScrollCallback(window_, glfw_scroll_cb);
}

// input handlers (implemented below)

Application::~Application() = default;

void Application::run() {
    while (!glfwWindowShouldClose(window_)) {
        int w, h;
        glfwGetFramebufferSize(window_, &w, &h);
        glViewport(0, 0, w, h);
        if (camera_) camera_->setAspect(w / float(h == 0 ? 1 : h));
        static double lastTime = glfwGetTime();
        double now = glfwGetTime();
        float dt = float(now - lastTime);
        lastTime = now;

        if (particleSystem_) particleSystem_->update(dt);

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 mvp = camera_ ? camera_->getViewProjection() : glm::mat4(1.0f);
        renderer_->render(mvp);
        if (particleSystem_) {
            auto& ps = particleSystem_->particles();
            size_t n = particleSystem_->aliveCount();
            renderer_->updateParticleBuffer(ps, n);
            renderer_->renderParticles(mvp, n);
        }

        glfwPollEvents();
        glfwSwapBuffers(window_);
    }
}

void Application::handleMouseButton(int button, int action, double x, double y) {
    if (camera_) camera_->onMouseButton(button, action, x, y);
    // spawn particles on left-click
    if (particleSystem_ && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        particleSystem_->spawn(200);
    }
}

void Application::handleCursorPos(double x, double y) {
    if (camera_) camera_->onCursorPos(x, y);
}

void Application::handleScroll(double yoffset) {
    if (camera_) camera_->onScroll(yoffset);
}
