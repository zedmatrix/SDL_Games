#include "Input.hpp"

void Input::update() {
    /// Copies keymap to previous key map
    for (auto& it : m_keyMap) {
        m_prevKeyMap[it.first] = it.second;
    }
}
void Input::pressKey(uint32_t keyID) {
    m_keyMap[keyID] = true;
}

void Input::releaseKey(uint32_t keyID) {
    m_keyMap[keyID] = false;
}
bool Input::isKeyDown(uint32_t keyID) {
    auto it = m_keyMap.find(keyID);
    if (it != m_keyMap.end()) {
        return it->second;
    }
    return false;
}

bool Input::isKeyPressed(uint32_t keyID) {
    if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false) {
        return true;
    }
    return false;
}
bool Input::wasKeyDown(uint32_t keyID) {
    auto it = m_prevKeyMap.find(keyID);
    if (it != m_prevKeyMap.end()) {
        return it->second;
    }
    return false;
}

void Input::setMouseCoords(float x, float y) {
    m_mouseCoords.x = x;
    m_mouseCoords.y = y;
}
