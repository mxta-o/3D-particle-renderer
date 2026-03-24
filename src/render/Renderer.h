#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "render/Shader.h"

namespace render {
    class Renderer {
    public:
        Renderer();
        ~Renderer();
        void render(const glm::mat4& mvp);
    private:
        unsigned int VAO_, VBO_;
        std::unique_ptr<Shader> shader_;
    };
}
