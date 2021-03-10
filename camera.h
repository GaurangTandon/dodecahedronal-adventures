#ifndef A0_CAMERA_H
#define A0_CAMERA_H

// demoss

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
private:
    void updateCameraEulerAngles() {
        glm::vec3 front;
        front.x = cosf(glm::radians(YAW)) * cosf(glm::radians(PITCH));
        front.y = sinf(glm::radians(PITCH));
        front.z = sinf(glm::radians(YAW)) * cosf(glm::radians(PITCH));

        // normalization
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }

public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    Camera(glm::vec3 position = POSITION, glm::vec3 up = UP) : Front(FRONT), Position(position), WorldUp(up) {
        updateCameraEulerAngles();
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(int direction, float deltaTime) {
        float velocity = SPEED * deltaTime;
        glm::vec3 delta;

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

    void reset() {
        Front = FRONT;
        Position = POSITION;
        WorldUp = UP;

        updateCameraEulerAngles();
    }

};

#endif //A0_CAMERA_H
