#define GLM_ENABLE_EXPERIMENTAL
#include "Zombie.hpp"
#include "Human.hpp"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Zombie::Zombie() {

}

void Zombie::update(const std::vector<std::string>& m_levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies, float deltaTime) {

    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randRotate(-2.0f, 2.0f);
    Human* closestHuman = getNearestHuman(humans);

    if (closestHuman != nullptr) {
        glm::vec2 m_direction = glm::normalize(closestHuman->getPosition() - m_position);
        m_position += m_direction * m_speed * deltaTime;
    }

    if (collideWithLevel(m_levelData) ) {
        m_direction = glm::rotate(m_direction, randRotate(randomEngine));
    }
}

void Zombie::init(float speed, glm::vec2 pos) {
    m_speed = speed;
    m_position = pos;
    m_color = ColorRGBA8(32, 255, 32, 255);
    m_health = 300;

    std::mt19937 randomEngine(time(nullptr));
    std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

    m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
    if (m_direction.length() == 0) m_direction = glm::vec2(1.0, 0.0f);

    m_direction = glm::normalize(m_direction);
}

Human* Zombie::getNearestHuman(std::vector<std::unique_ptr<Human>>& humans) {
    Human* closestHuman = nullptr;
    float smallestDistance = 9999;

    for (size_t i = 0; i < humans.size(); i++) {
        glm::vec2 distVec = humans[i]->getPosition() - m_position;
        float distance = glm::length(distVec);
        if (distance < smallestDistance) {
            smallestDistance = distance;
            closestHuman = humans[i].get();
        }

    }
    return closestHuman;
}
