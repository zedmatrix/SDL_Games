#ifndef AGENT_HPP
#define AGENT_HPP
#include <glm/glm.hpp>
#include <cmath>
#include <memory>
#include <algorithm>
#include "SpriteBatch.hpp"
#include "Vertex.hpp"
#include "Level.hpp"

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2;
const float TILE_RADIUS = TILE_WIDTH / 2;

class Zombie;
class Human;

class Agent {

public:
    Agent();
    virtual void update(const std::vector<std::string>& m_levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies, float deltaTime) = 0;

    bool collideWithLevel(const std::vector<std::string>& m_levelData);
    bool collideWithAgent(Agent* agent);

    void setTexture(const std::string& texturePath) {
        textureID = ResourceManager::getTexture(texturePath).id;
    }
    void draw(SpriteBatch& m_spriteBatch);
    bool applyDamage(float damage);

    glm::vec2 getPosition() const { return m_position; }

protected:
    void checkTilePosition(const std::vector<std::string>& m_levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y);
    void collideWithTile(glm::vec2 tilePos);
    glm::vec2 m_position;
    float m_speed;
    float m_health;
    ColorRGBA8 m_color;

private:
    GLuint textureID;

};

#endif //AGENT_HPP
