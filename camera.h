#ifndef CAMERA_H
#define CAMERA_H
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:

    static constexpr float yaw_default =            -90.0f;
    static constexpr float pitch_default =          0.0f;
    static constexpr float speed_default =          2.5f;
    static constexpr float sensitivity_default =    0.1f;
    static constexpr float zoom_default =           45.0f;

    Camera();

    Camera(glm::vec3 position);

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    glm::mat4 getView();
    float getFieldOfView();
    glm::vec3 getPosition();
    glm::vec3 getDirection();

    void processKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();

    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler Angles
    float yaw;
    float pitch;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

};

#endif // CAMERA_H
