#ifndef HUMAN_HPP
#define HUMAN_HPP
#include "Agent.hpp"


class Human : public Agent {

public:
    Human();

    void init(float speed, glm::vec2 pos);
    virtual void update(const std::vector<std::string>& m_levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies,
                        float deltaTime);

private:
    glm::vec2 m_direction;

};

#endif //HUMAN_HPP
