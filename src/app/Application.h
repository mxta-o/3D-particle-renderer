#pragma once

#include <GLFW/glfw3.h>
#include <memory>

namespace render { class Renderer; }

class Application {
public:
    Application(GLFWwindow* window);
    ~Application();
    void run();
private:
    GLFWwindow* window_;
    std::unique_ptr<render::Renderer> renderer_;
};
