#include "ParticleEngine.hpp"
/// Particle


void Particle::update(/*float deltaTime*/) {
    m_position += m_velocity; /* * deltaTime*/

}
/// Particle Batch
ParticleBatch::ParticleBatch() {
// empty
}

ParticleBatch::~ParticleBatch() {
    delete[] m_particles;
}

void ParticleBatch::init(int maxParticles, float decayRate, GLTexture texture) {
    m_maxParticles = maxParticles;
    m_particles = new Particle[m_maxParticles];
    m_decayRate = decayRate;
    m_texture = texture;

}
void ParticleBatch::update(/*float deltaTime*/) {
    for (int i = 0; i < m_maxParticles; i++) {

        if (m_particles[i].m_life > 0.0f) {
            m_particles[i].update(/*deltaTime*/);
            m_particles[i].m_life -= m_decayRate; /* * deltaTime*/
        }
    }
}
void ParticleBatch::draw(SpriteBatch* spriteBatch) {
    glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    for (int i = 0; i < m_maxParticles; i++) {
        auto& p = m_particles[i];
        if (p.m_life > 0.0f) {
            glm::vec4 destRect(p.m_position.x, p.m_position.y, p.m_width, p.m_width);
            spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, p.m_color);
        }
    }
}
int ParticleBatch::findFreeParticle() {
    for (int i = m_lastFreeParticle; i < m_maxParticles; i++) {
        if (m_particles[i].m_life <= 0.0f) {
            m_lastFreeParticle = i;
            return i;
        }
    }
    for (int i = 0; i < m_maxParticles; i++) {
        if (m_particles[i].m_life <= 0.0f) {
            m_lastFreeParticle = i;
            return i;
        }
    }
    return 0;
}

void ParticleBatch::addParticle(const glm::vec2& position, glm::vec2 velocity, float width, const ColorRGBA8& color) {
    int particleIndex = findFreeParticle();
    auto& p = m_particles[particleIndex];
    p.m_life = 1.0f;
    p.m_position = position;
    p.m_velocity = velocity;
    p.m_color = color;
    p.m_width = width;
}

/// Main Particle Engine
ParticleEngine::ParticleEngine() {
    //Empty
}
ParticleEngine::~ParticleEngine() {
    for (auto& b : m_batches) {
        delete b;
    }
}

void ParticleEngine::addParticleBatch(ParticleBatch* particleBatch) {
    m_batches.push_back(particleBatch);
}
void ParticleEngine::update(/*float deltaTime*/) {
    for (auto& b : m_batches) {
        b->update(/*deltaTime*/);
    }
}
void ParticleEngine::draw(SpriteBatch *spriteBatch) {

    for (auto& b : m_batches) {
        spriteBatch->begin();

        b->draw(spriteBatch);

        spriteBatch->end();

        spriteBatch->renderBatch();
    }

}
