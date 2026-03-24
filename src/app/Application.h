#pragma once

#include <memory>

struct GLFWwindow;

namespace render { class Renderer; }

class Camera;
class Application {
public:
    Application(GLFWwindow* window);
    ~Application();
    void run();
    // input forwarders used by GLFW callbacks
    void handleMouseButton(int button, int action, double x, double y);
    void handleCursorPos(double x, double y);
    void handleScroll(double yoffset);
private:
    GLFWwindow* window_;
    std::unique_ptr<render::Renderer> renderer_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<class ParticleSystem> particleSystem_;
};
