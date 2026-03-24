#pragma once

#include <glm/glm.hpp>

struct Particle {
    glm::vec3 pos{0.0f};
    glm::vec3 vel{0.0f};
    glm::vec4 color{1.0f};
    float life = 0.0f; // remaining life in seconds
};
