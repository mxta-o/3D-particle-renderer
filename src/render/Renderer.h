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
        // grid rendering (public so Application can invoke it)
        void renderGrid(const glm::mat4& mvp, float time);
    private:
        // particle buffers
        unsigned int pVAO_ = 0, pVBO_ = 0;
        std::unique_ptr<Shader> particleShader_;
        size_t particleCapacity_ = 0;
        // grid (space-time) visualization
        unsigned int gVAO_ = 0, gVBO_ = 0;
        std::unique_ptr<Shader> gridShader_;
        size_t gridVertexCount_ = 0;
        void initGrid(float extent = 10.0f, float spacing = 1.0f);
    };
}
