#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Weapon.hpp"
#include "ResourceManager.hpp"
#include "Vertex.hpp"
#include "Agent.hpp"
#include "Level.hpp"
#include "constants.hpp"

Gun::Gun(std::string name, int rate, int bps, int damage, float speed, float spread, SoundEffect* fireEffect) :
    m_name(name),
    m_fireRate(rate),
    m_bulletsPerShot(bps),
    m_damage(damage),
    m_speed(speed),
    m_spread(spread),
    m_frameCounter(0) {

    m_fireEffect = fireEffect;
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime) {
    m_frameCounter += 1.0f * deltaTime;
    if (m_frameCounter >= m_fireRate && isMouseDown) {
        fire(position, direction, bullets);
        m_frameCounter = 0;
    }

}
void Gun::fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets) {
    static std::mt19937 randomEngine(time(nullptr));
    std::uniform_real_distribution<float> randRotate(-glm::radians(m_spread), glm::radians(m_spread));

    m_fireEffect->play();
    for (int i = 0; i < m_bulletsPerShot; i++) {
        float angleOffset = randRotate(randomEngine);
        glm::vec2 newDirection = glm::rotate(direction, angleOffset);

        bullets.emplace_back(position, newDirection, m_damage, m_speed);
    }
}

/// Bullet Class
Bullet::Bullet(const glm::vec2& position, const glm::vec2& direction, float damage, float speed) :
    m_position(position),
    m_direction(direction),
    m_damage(damage),
    m_speed(speed) {

}
bool Bullet::update(const std::vector<std::string>& m_levelData, float deltaTime) {
    m_position += m_direction * m_speed * deltaTime;
    return collideWithWorld(m_levelData);
}

bool Bullet::collideWithWorld(const std::vector<std::string>& m_levelData) {
    glm::vec2 gridPosition;
    gridPosition.x = floor(m_position.x / (float)TILE_WIDTH);
    gridPosition.y = floor(m_position.y / (float)TILE_WIDTH);

    // If we are outside the world, just return
    if (gridPosition.x < 0 || gridPosition.x >= m_levelData[0].size() ||
        gridPosition.y < 0 || gridPosition.y >= m_levelData.size()) {
        return true;
    }

    return (m_levelData[gridPosition.y][gridPosition.x] != '.');
}

void Bullet::draw(SpriteBatch& m_spriteBatch) {
    glm::vec4 destRect(m_position.x + BULLET_RADIUS, m_position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
    glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    ColorRGBA8 m_greyColor = ColorRGBA8(128, 128, 128, 128);
    m_spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture(GE_Images::BULLET).id, 0.0f, m_greyColor);
}
bool Bullet::collideWithAgent(Agent* agent) {
    const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

    glm::vec2 centerPosA = m_position + glm::vec2(BULLET_RADIUS);
    glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

    glm::vec2 distVec = centerPosA - centerPosB;

    float distance = glm::length(distVec);

    float collisionDepth = MIN_DISTANCE - distance;
    if (collisionDepth > 0) {
        return true;
    }
    return false;

}
