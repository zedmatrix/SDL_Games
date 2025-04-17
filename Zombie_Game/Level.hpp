#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "SpriteBatch.hpp"
#include "ResourceManager.hpp"

const int TILE_WIDTH = 64;

class Level {

public:
    Level(const std::string& fileName);
    void draw();

    glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }
    const std::vector<std::string>& getLevelData() const { return m_levelData; }
    const std::vector<glm::vec2>& getStartZombiePos() const { return m_zombieStartPos; }

    int getWidth() const { return m_levelData[0].size(); }
    int getHeight() const { return m_levelData.size(); }
    int getNumHumans() const { return m_numHumans; }

private:
    std::vector<std::string> m_levelData;
    int m_numHumans;
    SpriteBatch m_spriteBatch;
    glm::vec2 m_startPlayerPos;
    std::vector<glm::vec2> m_zombieStartPos;
};

#endif //LEVEL_HPP
