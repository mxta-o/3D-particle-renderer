#include "sim/ParticleSystem.h"
#include <random>

ParticleSystem::ParticleSystem(size_t maxParticles)
    : particles_(maxParticles), maxParticles_(maxParticles), aliveCount_(0) {}

ParticleSystem::~ParticleSystem() = default;

void ParticleSystem::spawn(size_t count) {
    if (count == 0) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> ang(0.0f, 2.0f * 3.14159265359f);
    std::uniform_real_distribution<float> speed(0.5f, 3.0f);
    std::uniform_real_distribution<float> life(1.0f, 4.0f);

    for (size_t i = 0; i < count; ++i) {
        if (aliveCount_ >= maxParticles_) break;
        Particle& p = particles_[aliveCount_++];
        float a = ang(gen);
        float s = speed(gen);
        p.pos = emitterPos_;
        p.vel = glm::vec3(cosf(a) * s, (speed(gen) * 0.5f), sinf(a) * s);
        p.color = glm::vec4(1.0f, 0.6f + 0.4f * (s / 3.0f), 0.2f, 1.0f);
        p.life = life(gen);
    }
}

void ParticleSystem::update(float dt) {
    // handle continuous emission
    if (emitting_ && emissionRate_ > 0.0f) {
        float want = emissionRate_ * dt + emissionAccumulator_;
        size_t toSpawn = (size_t)want;
        emissionAccumulator_ = want - float(toSpawn);
        if (toSpawn > 0) spawn(toSpawn);
    }

    if (aliveCount_ == 0) return;

    const glm::vec3 gravity(0.0f, -9.81f, 0.0f);
    size_t write = 0;
    for (size_t i = 0; i < aliveCount_; ++i) {
        Particle& p = particles_[i];
        p.vel += gravity * dt;
        p.pos += p.vel * dt;
        p.life -= dt;
        if (p.life > 0.0f) {
            particles_[write++] = p;
        }
    }
    aliveCount_ = write;
}

void ParticleSystem::clear() {
    aliveCount_ = 0;
}

void ParticleSystem::setEmissionRate(float particlesPerSecond) {
    emissionRate_ = particlesPerSecond;
}

void ParticleSystem::setEmitterPos(const glm::vec3& pos) {
    emitterPos_ = pos;
}

void ParticleSystem::start() {
    emitting_ = true;
}

void ParticleSystem::stop() {
    emitting_ = false;
}
