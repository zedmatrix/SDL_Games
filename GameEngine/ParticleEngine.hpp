#ifndef PARTICLEENGINE_HPP
#define PARTICLEENGINE_HPP

#include <glm/glm.hpp>
#include <vector>
#include "Vertex.hpp"
#include "SpriteBatch.hpp"
#include "GLTexture.hpp"

/// Particle's'
class Particle {
public:
    friend class ParticleBatch;
    void update(/*float deltaTime*/);

private:
    glm::vec2 m_position = glm::vec2(0.0);
    glm::vec2 m_velocity = glm::vec2(0.0);
    float m_life = 0.0f;
    float m_width = 0.0f;
    ColorRGBA8 m_color;

};

/// ParticleBatch
class ParticleBatch {

public:

    ParticleBatch();
    ~ParticleBatch();
    void init(int maxParticles, float decayRate, GLTexture texture);
    void update(/*float deltaTime*/);
    void draw(SpriteBatch* spriteBatch);
    void addParticle(const glm::vec2& position, glm::vec2 velocity, float width, const ColorRGBA8& color);

private:
    int findFreeParticle();
    float m_decayRate = 0.1f;
    Particle* m_particles = nullptr;
    int m_maxParticles = 0;
    int m_lastFreeParticle = 0;
    GLTexture m_texture;


};


/// Main ParticleEngine
class ParticleEngine {

public:
    ParticleEngine();
    ~ParticleEngine();

    void addParticleBatch(ParticleBatch* particleBatch);
    void update(/*float deltaTime*/);
    void draw(SpriteBatch *spriteBatch);

private:
    std::vector<ParticleBatch*> m_batches;

};

#endif //PARTICLEENGINE_HPP
