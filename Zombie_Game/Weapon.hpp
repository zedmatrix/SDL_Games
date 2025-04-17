#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <random>
#include <memory>
#include <ctime>
#include "SpriteBatch.hpp"
#include "AudioEngine.hpp"

class Agent;
class Human;
class Zombie;
// Bullet Class

const int BULLET_RADIUS = 6;
class Bullet {

public:
    Bullet(const glm::vec2& position, const glm::vec2& direction, float damage, float speed);

    //When update returns true delete bullet
    bool update(const std::vector<std::string>& m_levelData, float deltaTime);

    void draw(SpriteBatch& m_spriteBatch);

    bool collideWithAgent(Agent* agent);
    float getDamage() const { return m_damage; }


private:

    bool collideWithWorld(const std::vector<std::string>& m_levelData);
    glm::vec2 m_position;
    glm::vec2 m_direction;
    float m_damage;
    float m_speed;

};

// Gun class
class Gun {

public:
    Gun(std::string name, int rate, int bps, int damage, float speed, float spread, SoundEffect* fireEffect);
    void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime);


private:
    void fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);
    std::string m_name;
    int m_fireRate;
    int m_bulletsPerShot;
    int m_damage;
    float m_speed;
    float m_spread;
    glm::vec2 m_direction;

    float m_frameCounter;
    SoundEffect* m_fireEffect;
};

#endif //WEAPON_HPP
