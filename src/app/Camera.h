#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera(float fov_deg = 45.0f, float aspect = 4.0f/3.0f);
    void setAspect(float aspect);
    void onMouseButton(int button, int action, double x, double y);
    void onCursorPos(double x, double y);
    void onScroll(double yoffset);
    glm::mat4 getViewProjection() const;
    void setTarget(const glm::vec3& t);
private:
    glm::vec3 target_ = glm::vec3(0.0f);
    float distance_ = 3.0f;
    float yaw_ = 0.0f;   // radians
    float pitch_ = 0.0f; // radians
    float fov_;
    float aspect_;
    bool rotating_ = false;
    double lastX_ = 0.0, lastY_ = 0.0;
};
