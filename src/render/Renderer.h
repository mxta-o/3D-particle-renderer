#pragma once

#include <memory>

namespace render {
    class Shader;
    class Renderer {
    public:
        Renderer();
        ~Renderer();
        void render();
    private:
        unsigned int VAO_, VBO_;
        std::unique_ptr<Shader> shader_;
    };
}
