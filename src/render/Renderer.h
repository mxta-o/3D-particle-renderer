#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "render/Shader.h"
#include "sim/Particle.h"

namespace render {
    class Renderer {
    public:
        Renderer();
        ~Renderer();
        void render(const glm::mat4& mvp);
        // particle rendering API
        void initParticleBuffers(size_t maxParticles);
        void updateParticleBuffer(const std::vector<Particle>& particles, size_t count);
        void renderParticles(const glm::mat4& mvp, size_t count);
    private:
        // particle buffers
        unsigned int pVAO_ = 0, pVBO_ = 0;
        std::unique_ptr<Shader> particleShader_;
        size_t particleCapacity_ = 0;
    };
}
