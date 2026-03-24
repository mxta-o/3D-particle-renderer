#include "render/Renderer.h"
#include "render/Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

using namespace render;

Renderer::Renderer() {
    // triangle rendering removed; particle buffers are initialized via initParticleBuffers()
}

Renderer::~Renderer() {
    if (pVAO_ != 0) glDeleteVertexArrays(1, &pVAO_);
    if (pVBO_ != 0) glDeleteBuffers(1, &pVBO_);
}

void Renderer::render(const glm::mat4& mvp) {
    // no-op: triangle drawing removed. Particles are drawn via renderParticles().
}

void Renderer::initParticleBuffers(size_t maxParticles) {
    particleCapacity_ = maxParticles;
    particleShader_ = std::make_unique<Shader>("src/render/shaders/particle.vert", "src/render/shaders/particle.frag");

    glGenVertexArrays(1, &pVAO_);
    glGenBuffers(1, &pVBO_);

    glBindVertexArray(pVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, pVBO_);
    // each particle: vec3 pos + vec4 color
    glBufferData(GL_ARRAY_BUFFER, particleCapacity_ * (3 + 4) * sizeof(float), nullptr, GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 4) * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (3 + 4) * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::updateParticleBuffer(const std::vector<Particle>& particles, size_t count) {
    if (count == 0 || pVBO_ == 0) return;
    size_t stride = 7; // pos(3) + color(4)
    std::vector<float> data(count * stride);
    for (size_t i = 0; i < count; ++i) {
        const Particle& p = particles[i];
        size_t base = i * stride;
        data[base + 0] = p.pos.x;
        data[base + 1] = p.pos.y;
        data[base + 2] = p.pos.z;
        data[base + 3] = p.color.r;
        data[base + 4] = p.color.g;
        data[base + 5] = p.color.b;
        data[base + 6] = p.color.a;
    }

    glBindBuffer(GL_ARRAY_BUFFER, pVBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float), data.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::renderParticles(const glm::mat4& mvp, size_t count) {
    if (count == 0 || pVAO_ == 0) return;
    particleShader_->use();
    particleShader_->setMat4("u_MVP", glm::value_ptr(mvp));
    glBindVertexArray(pVAO_);
    glDrawArrays(GL_POINTS, 0, (GLsizei)count);
    glBindVertexArray(0);
}
