#include "app/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

Camera::Camera(float fov_deg, float aspect)
    : fov_(glm::radians(fov_deg)), aspect_(aspect) {}

void Camera::setAspect(float aspect) {
    aspect_ = aspect;
}

void Camera::onMouseButton(int button, int action, double x, double y) {
    (void)button;
    if (action == 1) { // press
        rotating_ = true;
        lastX_ = x; lastY_ = y;
    } else { // release
        rotating_ = false;
    }
}

void Camera::onCursorPos(double x, double y) {
    if (!rotating_) return;
    double dx = x - lastX_;
    double dy = y - lastY_;
    const float sens = 0.005f;
    yaw_   += (float)(-dx * sens);
    pitch_ += (float)(-dy * sens);
    // clamp pitch
    const float maxP = glm::half_pi<float>() - 0.01f;
    if (pitch_ > maxP) pitch_ = maxP;
    if (pitch_ < -maxP) pitch_ = -maxP;
    lastX_ = x; lastY_ = y;
}

void Camera::onScroll(double yoffset) {
    const float zoomSpeed = 0.5f;
    distance_ *= (float)glm::pow(0.9f, (float)yoffset * zoomSpeed);
    if (distance_ < 0.1f) distance_ = 0.1f;
}

void Camera::setTarget(const glm::vec3& t) {
    target_ = t;
}

glm::mat4 Camera::getViewProjection() const {
    // spherical to Cartesian
    float x = distance_ * cosf(pitch_) * sinf(yaw_);
    float y = distance_ * sinf(pitch_);
    float z = distance_ * cosf(pitch_) * cosf(yaw_);
    glm::vec3 pos = target_ + glm::vec3(x, y, z);

    glm::mat4 view = glm::lookAt(pos, target_, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(fov_, aspect_, 0.1f, 100.0f);
    return proj * view;
}
