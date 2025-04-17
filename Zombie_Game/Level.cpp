#include "Level.hpp"
#include "Error.hpp"
#include "Vertex.hpp"
#include "constants.hpp"

Level::Level(const std::string& fileName) {

    std::ifstream file;
    file.open(fileName);
    if (file.fail()) {
        fatalError("Failed to Open " + fileName);
    }
    std::string tmp;
    file >> tmp >> m_numHumans;

    std::getline(file, tmp);    // throw away first line and fill vector ? leftover whitespace gdb a=1, b=-1
    while (std::getline(file, tmp)) {
        m_levelData.push_back(tmp);
    }

    m_spriteBatch.init();
    m_spriteBatch.begin();
    glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    ColorRGBA8 m_white = ColorRGBA8(255, 255, 255, 255);

    // render the level tiles
    for (size_t y = 0; y < m_levelData.size(); y++) {
        for (size_t x = 0; x < m_levelData[y].size(); x++) {
            char tile = m_levelData[y][x];

            glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
            switch (tile) {
                case 'R':
                    m_spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture(GE_Images::BRICKS).id, 0.0f, m_white);
                    break;
                case 'L':
                case 'W':
                    m_spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture(GE_Images::WOOD).id, 0.0f, m_white);
                    break;
                case 'B':
                case 'S':
                    m_spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture(GE_Images::STEEL).id, 0.0f, m_white);
                    break;
                case 'G':
                    m_spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture(GE_Images::GLASS).id, 0.0f, m_white);
                    break;
                case '@':
                    m_startPlayerPos.x = x * TILE_WIDTH;
                    m_startPlayerPos.y = y * TILE_WIDTH;
                    m_levelData[y][x] = '.';
                    break;
                case 'Z':
                    m_zombieStartPos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
                    m_levelData[y][x] = '.';
                    break;
                case '.':
                    break;
                default:
                    std::printf("Unexpected Symbol %c at (%ld, %ld)\n", tile, x, y);
                    break;
            }
        }
    }
    m_spriteBatch.end();
}

void Level::draw() {
    m_spriteBatch.renderBatch();
}
