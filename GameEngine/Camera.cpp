#include "Camera.hpp"

Camera::Camera()
    : m_needMatrixUpdate(true),
    m_position(0.0f, 0.0f),
    m_cameraMatrix(1.0f),
    m_orthoMatrix(1.0f),
    m_scale(1.0f) {}

void Camera::init(int screenWidth, int screenHeight) {
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_screenWidth),
                              0.0f, static_cast<float>(m_screenHeight));

}

void Camera::update() {

    if (m_needMatrixUpdate) {

        // Camera Translation
        glm::vec3 translate(-m_position.x + m_screenWidth / 2, -m_position.y + m_screenHeight / 2, 0.0f);
        m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

        // Camera Scale
        glm::vec3 scale(m_scale, m_scale, 0.0f);
        m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

        m_needMatrixUpdate = false;
    }
}

glm::vec2 Camera::getWorldCoords(glm::vec2 screenCoords) {
    //Invert y coordinates
    screenCoords.y = m_screenHeight - screenCoords.y;

    // Make the center of screen ZERO
    screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2);

    // Scale the coordinates
    screenCoords /= m_scale;

    //Translate to camera position
    screenCoords += m_position;
    return screenCoords;
}
bool Camera::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions) {
    // Scaled Screen Coords
    glm::vec2 scaledScreenDimensions = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;

    const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
    const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

    // Center Position of object and of Camera
    glm::vec2 centerPos = position + dimensions / 2.0f;
    glm::vec2 centerCameraPos = m_position;

    // Vector from CameraPosition to Object Position
    glm::vec2 distVec = centerPos - centerCameraPos;

    float xdepth = MIN_DISTANCE_X - std::abs(distVec.x);
    float ydepth = MIN_DISTANCE_Y - std::abs(distVec.y);

    /// collision
    if (xdepth > 0 && ydepth > 0) {
        /// collision
        return true;
    }
    return false;
}
