#ifndef A0_CAMERA_H
#define A0_CAMERA_H

// TODO: demoss, copied from learnopengl.com

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

const auto POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const auto UP = glm::vec3(0.0f, 1.0f, 0.0f);
const auto FRONT = glm::vec3(0.0f, 0.0f, -1.0f);

class Camera {
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(glm::vec3 position = POSITION, glm::vec3 up = UP, float yaw = YAW, float pitch = PITCH) : Front(FRONT),
                                                                                                     MovementSpeed(
                                                                                                             SPEED),
                                                                                                     MouseSensitivity(
                                                                                                             SENSITIVITY),
                                                                                                     Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(int direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        glm::vec3 delta;
        std::cout << velocity << std::endl;

        if (direction == 0)
            delta = -Front * velocity;
        else if (direction == 1)
            delta = Front * velocity;
        else if (direction == 2)
            delta = -Right * velocity;
        else if (direction == 3)
            delta = Right * velocity;
        else if (direction == 4)
            delta = -Up * velocity;
        else if (direction == 5)
            delta = Up * velocity;
        else assert(false);

        Position += delta;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cosf(glm::radians(Yaw)) * cosf(glm::radians(Pitch));
        front.y = sinf(glm::radians(Pitch));
        front.z = sinf(glm::radians(Yaw)) * cosf(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front,
                                          WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

#endif //A0_CAMERA_H
