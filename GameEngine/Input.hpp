#ifndef INPUT_HPP
#define INPUT_HPP
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <cstdint>
#include <unordered_map>
class Input {

public:

    Input() : m_mouseCoords(0) {}

    void update();
    void pressKey(uint32_t keyID);
    void releaseKey(uint32_t keyID);
    bool isKeyDown(uint32_t keyID);
    bool isKeyPressed(uint32_t keyID);
    void setMouseCoords(float x, float y);
    glm::vec2 getMouseCoords() const { return m_mouseCoords; }
private:
    bool wasKeyDown(uint32_t keyID);
    std::unordered_map<uint32_t, bool> m_prevKeyMap;  ///< the previous key map
    std::unordered_map<uint32_t, bool> m_keyMap;      ///< the current key map
    glm::vec2 m_mouseCoords;

};

#endif //INPUT_HPP
