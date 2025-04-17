#include "Agent.hpp"
#include "ResourceManager.hpp"
#include "Error.hpp"
Agent::Agent() {
    //Empty
}

void Agent::draw(SpriteBatch& m_spriteBatch) {
    if (textureID != 0) {
        const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        glm::vec4 destRect;
        destRect.x = m_position.x;
        destRect.y = m_position.y;
        destRect.z = AGENT_WIDTH;
        destRect.w = AGENT_WIDTH;

        m_spriteBatch.draw(destRect, uvRect, textureID, 0.0f, m_color);
    } else {
        fatalError("textureID not set");
    }
}

bool Agent::applyDamage(float damage) {
    m_health -= damage;
    if (m_health <= 0) {
        return true;
    }
    return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& m_levelData,
                              std::vector<glm::vec2>& collideTilePosition, float x, float y) {

    if (std::isnan(x) || std::isnan(y)) {
        // x or y is NaN and offscreen ?
        return;
    }
    glm::vec2 cornerPos = glm::vec2(std::floor(x / (float)TILE_WIDTH),
                                      std::floor(y / (float)TILE_WIDTH));

    // outside world check
    if (cornerPos.x < 0 || cornerPos.x >= m_levelData[0].length() || cornerPos.y < 0 || cornerPos.y >= m_levelData.size()) return;

    if (m_levelData[cornerPos.y][cornerPos.x] != '.') {
        collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
    }
}

bool Agent::collideWithLevel(const std::vector<std::string>& m_levelData) {
    if (std::isnan(m_position.x) || std::isnan(m_position.y)) {
        return false;
    }

    std::vector<glm::vec2> collideTilePosition;
    checkTilePosition(m_levelData, collideTilePosition, m_position.x, m_position.y);
    checkTilePosition(m_levelData, collideTilePosition, m_position.x + AGENT_WIDTH, m_position.y);
    checkTilePosition(m_levelData, collideTilePosition, m_position.x, m_position.y + AGENT_WIDTH);
    checkTilePosition(m_levelData, collideTilePosition, m_position.x + AGENT_WIDTH, m_position.y + AGENT_WIDTH);
    if (collideTilePosition.size() == 0) {
        return false;
    }
    for (size_t i = 0; i < collideTilePosition.size(); i++) {
        collideWithTile(collideTilePosition[i]);
    }
    return true;
}
bool Agent::collideWithAgent(Agent* agent) {
    if (!agent) {
        std::cerr << "Error: collideWithAgent called with nullptr!" << std::endl;
        return false;
    }
    // Minimum distance before there is a collision
    const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

    // Center position of this agent and parameter agent
    glm::vec2 centerPosA = m_position + glm::vec2(AGENT_RADIUS);
    glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

    // Distance vector between the two agents
    glm::vec2 distVec = centerPosA - centerPosB;

    float distance = glm::length(distVec);
    float collisionDepth = MIN_DISTANCE - distance;

    if (collisionDepth > 0) {
        glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
        m_position += collisionDepthVec / 2.0f;
        agent->m_position -= collisionDepthVec / 2.0f;

        return true;
    }
    return false;
}

void Agent::collideWithTile(glm::vec2 tilePos) {

    const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
    glm::vec2 centerPlayerPos = m_position + glm::vec2(AGENT_RADIUS);
    glm::vec2 distVec = centerPlayerPos - tilePos;

    float xdepth = MIN_DISTANCE - std::abs(distVec.x);
    float ydepth = MIN_DISTANCE - std::abs(distVec.y);

    /// collision
    if (xdepth > 0 && ydepth > 0) {
        if (std::max(xdepth, 0.0f) < std::max(ydepth, 0.0f)) {
            if (distVec.x < 0) {
                m_position.x -= xdepth;
            } else {
                m_position.x += xdepth;
            }
        } else {
            if (distVec.y < 0) {
                m_position.y -= ydepth;
            } else {
                m_position.y += ydepth;
            }
        }
    }
}
