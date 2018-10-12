#include "camera.h"

Camera::Camera() :
    position{glm::vec3(0.0f, 0.0f, 0.0f)},
    worldUp{glm::vec3(0.0f, 1.0f, 0.0f)},
    yaw{yaw_default},
    pitch{pitch_default},
    movementSpeed{speed_default},
    mouseSensitivity{sensitivity_default},
    zoom{zoom_default}
{
    updateCameraVectors();
}


Camera::Camera(glm::vec3 position) :
position{position},
worldUp{glm::vec3(0.0f, 1.0f, 0.0f)},
yaw{yaw_default},
pitch{pitch_default},
movementSpeed{speed_default},
mouseSensitivity{sensitivity_default},
zoom{zoom_default}
{
    updateCameraVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
position{position},
worldUp{up},
yaw{yaw},
pitch{pitch},
movementSpeed{speed_default},
mouseSensitivity{sensitivity_default},
zoom{zoom_default}
{
    updateCameraVectors();
}

glm::mat4 Camera::getView()
{
    return glm::lookAt(position, position + front, up);
}

float Camera::getFieldOfView()
{
    return glm::radians(zoom);
}

glm::vec3 Camera::getPosition()
{
    return position;
}

glm::vec3 Camera::getDirection()
{
    return front;
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        position += front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        position -= front * velocity;
    if (direction == Camera_Movement::LEFT)
        position -= right * velocity;
    if (direction == Camera_Movement::RIGHT)
        position += right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    yaw   += xoffset * mouseSensitivity;
    pitch += yoffset * mouseSensitivity;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    yaw = glm::mod( yaw + xoffset, 360.0f );

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset;
    if (zoom <= 1.0f)
        zoom = 1.0f;
    if (zoom >= 45.0f)
        zoom = 45.0f;
}


void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    front = glm::normalize( glm::vec3(
                    cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                    sin(glm::radians(pitch)),
                    sin(glm::radians(yaw)) * cos(glm::radians(pitch))
                          ));

    // Also re-calculate the Right and Up vector
    // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}
