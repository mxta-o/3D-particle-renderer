#pragma once

#include "sim/Particle.h"
#include <vector>

class ParticleSystem {
public:
    explicit ParticleSystem(size_t maxParticles = 10000);
    ~ParticleSystem();

    // Spawn 'count' new particles at origin with randomized velocity
    void spawn(size_t count);

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
};
