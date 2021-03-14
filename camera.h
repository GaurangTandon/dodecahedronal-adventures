#ifndef A0_CAMERA_H
#define A0_CAMERA_H

// demoss

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

const float SPEED = 2.5f;

const auto POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const auto UP = glm::vec3(0.0f, 1.0f, 0.0f);
const auto FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 WORLD_UP;

class Camera {
private:
    void updateCameraUpRightVectors() {
        // right and up are derived from Front and an absolute WORLD_UP
        Right = glm::normalize(glm::cross(Front, WORLD_UP));
        Up = glm::normalize(glm::cross(Right, Front));
        // this makes sense to me
    }

public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 initialPosition;

    Camera(glm::vec3 position = POSITION) : Position(position),
                                            Front(FRONT), Up(UP),
                                            initialPosition(position) {
        WORLD_UP = Up;
        updateCameraUpRightVectors();
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void translate(int direction, float deltaTime) {
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
        else
            assert(false);

        Position += delta;
    }

    void rotate() {
        auto theta = glm::radians(5.0f);

        auto magnitude = glm::length(Position);

        // rotate front vector in xz plane
        auto x = Front.x, z = Front.z;
        Front.x = cos(theta) * x - sin(theta) * z;
        Front.z = sin(theta) * x + cos(theta) * z;

        // Update position vector in line with the new front vector
        Position = -magnitude * Front;

        updateCameraUpRightVectors();
    }

    void reset() {
        Front = FRONT;
        Position = initialPosition;

        updateCameraUpRightVectors();
    }

    void predefinedJump(int pred_position, const glm::vec3 &center_position) {
        auto magnitude = glm::length(Position);

        auto s2 = sqrtf(2.0f);
        auto nis2 = -1 / s2;

        switch (pred_position) {
            case 0:
                Front = FRONT;
                break;
            case 1:
                Front = -FRONT;
                break;
            default:
                Front = glm::vec3(nis2, 0, nis2); // 1/root2, 0, 1/root2
        }

//        Front -= glm::normalize(center_position);
//        Front = glm::normalize(Front);
        Position = center_position - magnitude * Front;

        updateCameraUpRightVectors();
    }
};

#endif //A0_CAMERA_H
