#include "render/Renderer.h"
#include "render/Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

using namespace render;

static const float triangleVertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

Renderer::Renderer() {
    shader_ = std::make_unique<Shader>("src/render/shaders/basic.vert", "src/render/shaders/basic.frag");

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}

void Renderer::render(const glm::mat4& mvp) {
    shader_->use();
    shader_->setMat4("u_MVP", glm::value_ptr(mvp));
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
