#pragma once

#include <memory>
#include "render/Shader.h"

namespace render {
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
