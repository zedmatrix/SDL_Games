#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

public:
    Camera();

    void update();
    void init(int screenWidth, int screenHeight);
    // Setters
    void setPosition(const glm::vec2& newPosition) { m_position = newPosition; m_needMatrixUpdate = true; }
    void setScale(float newScale) { m_scale = newScale; m_needMatrixUpdate = true; }

    // Getters
    glm::vec2 getPosition() const { return m_position; }
    glm::vec2 getWorldCoords(glm::vec2 screenCoords);
    const glm::mat4& getCameraMatrix() const { return m_cameraMatrix; }
    float getScale() const { return m_scale; }
    // Camera Culling
    bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions);


private:
    int m_screenWidth, m_screenHeight;
    bool m_needMatrixUpdate;
    glm::vec2 m_position;
    glm::mat4 m_cameraMatrix;
    glm::mat4 m_orthoMatrix;
    float m_scale;

};

#endif //CAMERA_HPP
