#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight, glm::vec3 position) : _screenWidth(screenWidth), _screenHeight(screenHeight)
{
    //Ustawiamy pocz¹tkowe wartoœci parametrów kamery
    _position = position;
    _up = glm::vec3(0.0f, 1.0f, 0.0f);
    _front = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));
    _yaw = -90.0f;
    _pitch = 0.0f;
    _fov = 45.0f;
    _mouseSensitivity = 0.2f;
    _movementSpeed = 2.0f;
    _aspectRatio = (float)_screenWidth / (float)_screenWidth;
    _worldUp = _up;
    updateCameraVectors();
}

void Camera::setMouseMovement(float xOffset, float yOffset, float sensitivity)
{
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    _yaw += xOffset;
    _pitch += yOffset;

    //Ograniczenie zakresu ruchu kamery
    if (_pitch > 89.0f) {
        _pitch = 89.0f;
    }

    if (_pitch < -89.0f) {
        _pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::setKeyboardMovement(Direction direction, float deltaTime)
{
    float velocity = _movementSpeed * deltaTime;
    switch (direction) {
    case Direction::Forward:
        _position += _front * velocity;
        break;
    case Direction::Backward:
        _position -= _front * velocity;
        break;
    case Direction::Left:
        _position -= _right * velocity;
        break;
    case Direction::Right:
        _position += _right * velocity;
        break;
    }
}

void Camera::setMouseScroll(float yOffset)
{
    if (_fov >= 1.0f && _fov <= 45.0f) {
        _fov -= yOffset;
    }

    //Ograniczenie zakresu FOV (field of view)
    if (_fov <= 1.0f) {
        _fov = 1.0f;
    }

    if (_fov >= 45.0f) {
        _fov = 45.0f;
    }
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(_position, _position + _front, _up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(_fov), _aspectRatio, 0.1f, 100.0f);
}

glm::vec3 Camera::getCameraPosition() const
{
    return _position;
}


void Camera::updateCameraVectors()
{
    //nowy wektor kierunku dla obliczen
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    //Normalizacja wektora w celu naprawienia movmentu w sytuacji patrzenia w gore i dol
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}