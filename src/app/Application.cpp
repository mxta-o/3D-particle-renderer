#include "app/Application.h"
#include "render/Renderer.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "app/Camera.h"
#include "sim/ParticleSystem.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

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
    // ImGui initialization (requires GL context and glad)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    // configure continuous fountain emitter (start higher)
    emitterY_ = 3.5f;
    emissionRateUI_ = 800.0f;
    restitutionUI_ = 0.5f;
    groundYUI_ = 0.0f;
    particleSystem_->setEmitterPos(glm::vec3(0.0f, emitterY_, 0.0f));
    particleSystem_->setEmissionRate(emissionRateUI_); // particles per second
    particleSystem_->setRestitution(restitutionUI_);
    particleSystem_->setGroundHeight(groundYUI_);
    if (camera_) camera_->setTarget(glm::vec3(0.0f, emitterY_, 0.0f));
    particleSystem_->start();
    glfwSetWindowUserPointer(window_, this);
    glfwSetMouseButtonCallback(window_, glfw_mouse_button_cb);
    glfwSetCursorPosCallback(window_, glfw_cursor_pos_cb);
    glfwSetScrollCallback(window_, glfw_scroll_cb);
}

// input handlers (implemented below)

// shutdown ImGui in destructor
Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

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

        // start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // HUD: top-left performance panel
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;
        ImGui::SetNextWindowPos(ImVec2(10,10), ImGuiCond_Always);
        ImGui::Begin("HUD", nullptr, flags);
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("FPS: %.1f", io.Framerate);
        if (particleSystem_) {
            ImGui::Text("Particles: %zu", particleSystem_->aliveCount());
        }
        // Emitter controls
        if (ImGui::CollapsingHeader("Emitter")) {
            if (ImGui::SliderFloat("Emitter Y", &emitterY_, 0.0f, 20.0f)) {
                particleSystem_->setEmitterPos(glm::vec3(0.0f, emitterY_, 0.0f));
                if (camera_) camera_->setTarget(glm::vec3(0.0f, emitterY_, 0.0f));
            }
            if (ImGui::SliderFloat("Emission Rate", &emissionRateUI_, 0.0f, 5000.0f)) {
                particleSystem_->setEmissionRate(emissionRateUI_);
            }
            if (ImGui::SliderFloat("Restitution", &restitutionUI_, 0.0f, 1.0f)) {
                particleSystem_->setRestitution(restitutionUI_);
            }
            if (ImGui::SliderFloat("Ground Y", &groundYUI_, -5.0f, 5.0f)) {
                particleSystem_->setGroundHeight(groundYUI_);
            }
            if (ImGui::Button("Start/Stop")) {
                static bool running = true;
                running = !running;
                if (running) particleSystem_->start(); else particleSystem_->stop();
            }
        }
        ImGui::End();

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 mvp = camera_ ? camera_->getViewProjection() : glm::mat4(1.0f);
        // draw grid (space-time visualization)
        renderer_->renderGrid(mvp, (float)now);
        if (particleSystem_) {
            auto& ps = particleSystem_->particles();
            size_t n = particleSystem_->aliveCount();
            renderer_->updateParticleBuffer(ps, n);
            renderer_->renderParticles(mvp, n);
        }

        // render ImGui on top
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window_);
    }
}

void Application::handleMouseButton(int button, int action, double x, double y) {
    if (camera_) camera_->onMouseButton(button, action, x, y);
    // (click no longer spawns; continuous emitter active)
}

void Application::handleCursorPos(double x, double y) {
    if (camera_) camera_->onCursorPos(x, y);
}

void Application::handleScroll(double yoffset) {
    if (camera_) camera_->onScroll(yoffset);
}
