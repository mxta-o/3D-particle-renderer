#pragma once

#include <memory>

struct GLFWwindow;

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
