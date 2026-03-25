#include "render/Renderer.h"
#include "render/Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

using namespace render;

Renderer::Renderer() {
    // triangle rendering removed; particle buffers are initialized via initParticleBuffers()
    // initialize a grid visualization
    initGrid(12.0f, 1.0f, 5);
}

Renderer::~Renderer() {
    if (pVAO_ != 0) glDeleteVertexArrays(1, &pVAO_);
    if (pVBO_ != 0) glDeleteBuffers(1, &pVBO_);
    if (gVAO_minor_ != 0) glDeleteVertexArrays(1, &gVAO_minor_);
    if (gVBO_minor_ != 0) glDeleteBuffers(1, &gVBO_minor_);
    if (gVAO_major_ != 0) glDeleteVertexArrays(1, &gVAO_major_);
    if (gVBO_major_ != 0) glDeleteBuffers(1, &gVBO_major_);
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

void Renderer::initGrid(float extent, float spacing, int majorEvery) {
    gridShader_ = std::make_unique<Shader>("src/render/shaders/grid.vert", "src/render/shaders/grid.frag");

    std::vector<float> vertsMinor;
    std::vector<float> vertsMajor;
    int lines = (int)std::floor((extent * 2.0f) / spacing) + 1;
    float start = -extent;
    // lines along X (varying Z)
    for (int i = 0; i < lines; ++i) {
        float z = start + i * spacing;
        bool isMajor = (i % majorEvery) == 0;
        if (isMajor) {
            vertsMajor.push_back(-extent); vertsMajor.push_back(0.0f); vertsMajor.push_back(z);
            vertsMajor.push_back(extent); vertsMajor.push_back(0.0f); vertsMajor.push_back(z);
        } else {
            vertsMinor.push_back(-extent); vertsMinor.push_back(0.0f); vertsMinor.push_back(z);
            vertsMinor.push_back(extent); vertsMinor.push_back(0.0f); vertsMinor.push_back(z);
        }
    }
    // lines along Z (varying X)
    for (int i = 0; i < lines; ++i) {
        float x = start + i * spacing;
        bool isMajor = (i % majorEvery) == 0;
        if (isMajor) {
            vertsMajor.push_back(x); vertsMajor.push_back(0.0f); vertsMajor.push_back(-extent);
            vertsMajor.push_back(x); vertsMajor.push_back(0.0f); vertsMajor.push_back(extent);
        } else {
            vertsMinor.push_back(x); vertsMinor.push_back(0.0f); vertsMinor.push_back(-extent);
            vertsMinor.push_back(x); vertsMinor.push_back(0.0f); vertsMinor.push_back(extent);
        }
    }

    gridCountMinor_ = vertsMinor.size() / 3;
    gridCountMajor_ = vertsMajor.size() / 3;

    // minor
    glGenVertexArrays(1, &gVAO_minor_);
    glGenBuffers(1, &gVBO_minor_);
    glBindVertexArray(gVAO_minor_);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO_minor_);
    glBufferData(GL_ARRAY_BUFFER, vertsMinor.size() * sizeof(float), vertsMinor.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // major
    glGenVertexArrays(1, &gVAO_major_);
    glGenBuffers(1, &gVBO_major_);
    glBindVertexArray(gVAO_major_);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO_major_);
    glBufferData(GL_ARRAY_BUFFER, vertsMajor.size() * sizeof(float), vertsMajor.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::renderGrid(const glm::mat4& mvp, float time) {
    if (!gridShader_) return;
    gridShader_->use();
    gridShader_->setMat4("u_MVP", glm::value_ptr(mvp));
    gridShader_->setFloat("u_time", time);

    // minor lines: thicker
    if (gVAO_minor_ != 0 && gridCountMinor_ > 0) {
        glLineWidth(2.0f);
        gridShader_->setVec3("u_color", 1.0f, 1.0f, 1.0f);
        glBindVertexArray(gVAO_minor_);
        glDrawArrays(GL_LINES, 0, (GLsizei)gridCountMinor_);
    }

    // major lines: much thicker and bright white
    if (gVAO_major_ != 0 && gridCountMajor_ > 0) {
        glLineWidth(6.0f);
        gridShader_->setVec3("u_color", 1.0f, 1.0f, 1.0f);
        glBindVertexArray(gVAO_major_);
        glDrawArrays(GL_LINES, 0, (GLsizei)gridCountMajor_);
    }

    glBindVertexArray(0);
    glLineWidth(1.0f);
}
void Renderer::renderParticles(const glm::mat4& mvp, size_t count) {
    if (count == 0 || pVAO_ == 0) return;
    particleShader_->use();
    particleShader_->setMat4("u_MVP", glm::value_ptr(mvp));
    glBindVertexArray(pVAO_);
    glDrawArrays(GL_POINTS, 0, (GLsizei)count);
    glBindVertexArray(0);
}
