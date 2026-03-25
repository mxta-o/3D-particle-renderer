#pragma once

#include "sim/Particle.h"
#include <vector>
#include <glm/glm.hpp>

class ParticleSystem {
public:
    explicit ParticleSystem(size_t maxParticles = 10000);
    ~ParticleSystem();

    // Spawn 'count' new particles at origin with randomized velocity
    void spawn(size_t count);

    // Continuous emission controls
    void setEmissionRate(float particlesPerSecond);
    void setEmitterPos(const glm::vec3& pos);
    void start();
    void stop();
    // ground / collision
    void setGroundHeight(float y);
    void setRestitution(float r);

    // Advance simulation by dt seconds (simple Euler integrator)
    void update(float dt);

    // Access particles for rendering / inspection
    const std::vector<Particle>& particles() const { return particles_; }

    // Number of alive particles (compact prefix of particles_)
    size_t aliveCount() const { return aliveCount_; }

    // Clear all particles
    void clear();

private:
    std::vector<Particle> particles_;
    size_t maxParticles_;
    size_t aliveCount_ = 0;
    // emission state
    float emissionRate_ = 0.0f; // particles per second
    glm::vec3 emitterPos_{0.0f, 0.0f, 0.0f};
    float emissionAccumulator_ = 0.0f;
    bool emitting_ = false;
    // ground collision
    float groundY_ = 0.0f;
    float restitution_ = 0.5f; // bounce multiplier
};
