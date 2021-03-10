#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include <tuple>
#include <iostream>
#include <cassert>
#include <vector>

#include "regular_dodec.h"
#include "hexa_bipyramid.h"
#include "unidecagon.h"
#include "cube.h"
#include "camera.h"

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

bool ROTATING_X = false;
bool ROTATING_Y = false;
bool ROTATING_Z = false;
int CURR_OBJECT = 0;

void frameSizeCallback(GLFWwindow *_window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Shader &shader, Camera &camera) {
#define pressed(x) (glfwGetKey(window, x) == GLFW_PRESS)

    std::vector <std::tuple<int, int, int>> objectMappings = {
            {GLFW_KEY_Q, 0, 1},
            {GLFW_KEY_A, 0, -1},
            {GLFW_KEY_W, 1, 1},
            {GLFW_KEY_S, 1, -1},
            {GLFW_KEY_E, 2, 1},
            {GLFW_KEY_D, 2, -1}
    };

    std::vector <std::tuple<int, int>> cameraMappings = {
            {GLFW_KEY_R, 0},
            {GLFW_KEY_F, 1},
            {GLFW_KEY_T, 2},
            {GLFW_KEY_G, 3},
            {GLFW_KEY_Y, 4},
            {GLFW_KEY_H, 5}
    };

    if (pressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        ROTATING_Z = not ROTATING_Z;
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        ROTATING_X = not ROTATING_X;
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        ROTATING_Y = not ROTATING_Y;
        return;
    }

    // reset everything
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        ROTATING_X = ROTATING_Y = ROTATING_Z = false;
        shader.reset();
//        camera.reset();
        return;
    }

    if (pressed(GLFW_KEY_1)) {
        CURR_OBJECT = 1;
        return;
    }
    if (pressed(GLFW_KEY_2)) {
        CURR_OBJECT = 2;
        return;
    }
    if (pressed(GLFW_KEY_3)) {
        CURR_OBJECT = 3;
        return;
    }
    if (pressed(GLFW_KEY_0)) {
        CURR_OBJECT = 0;
        return;
    }

    for (auto &[key, axis, dir] : objectMappings) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            shader.moveObject(axis, dir);
            return;
        }
    }

    for (auto &[key, dir] : cameraMappings) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            camera.ProcessKeyboard(dir, shader.getTimeDifference());
            return;
        }
    }
}

void updateFrame(GLFWwindow *_window) {
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void calcProjections(Camera &camera, Shader &shader) {
    auto projection = glm::perspective(50.0f, (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 1.0f,
                                       100.0f);
    shader.setMatrix("projection", projection);

    // camera/view transformation
    auto view = camera.GetViewMatrix();
    shader.setMatrix("view", view);
}

void drawObjects(Camera &camera, Shader &shader, Polyhedron &obj) {
    shader.use();
    if (ROTATING_X)
        shader.rotObject(0);
    if (ROTATING_Y)
        shader.rotObject(1);
    if (ROTATING_Z)
        shader.rotObject(2);
    calcProjections(camera, shader);
    obj.draw();
}

void renderLoop(GLFWwindow *window) {
    // toggle for wireframe
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    auto shader = Shader("shader.vs", "shader.fs");
    shader.use();
    shader.initMatrixes();

    float scale = 0.5f;
    RegularDodecahedron reg = RegularDodecahedron(scale);
    HexagonalBipyramid hex = HexagonalBipyramid(scale);
    Unidecagon unid = Unidecagon(scale);
    Cube cube = Cube(scale);

    Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

    long long int previousFrames = 0;
    double prevTime = glfwGetTime();
    long long int frames = 0;

    while (not glfwWindowShouldClose(window)) {
        frames++;

        auto currentTime = glfwGetTime();
        if (currentTime > prevTime + 1) {
            auto rate = frames - previousFrames;
            std::cout << "Frames (per second):" << rate << std::endl;

            previousFrames = frames;
            prevTime = currentTime;
        }

        processInput(window, shader, camera);

        updateFrame(window);

        if (CURR_OBJECT == 0)
            drawObjects(camera, shader, reg);
        else if (CURR_OBJECT == 1)
            drawObjects(camera, shader, hex);
        else if (CURR_OBJECT == 2)
            drawObjects(camera, shader, unid);
        else
            drawObjects(camera, shader, cube);

        // swap the currently computed render buffers with whatever is in the
        // window currently
        glfwSwapBuffers(window);
        // poll for keyboard/mouse/etc.
        glfwPollEvents();
    }
}

int main() {
    srand(42);

    glfwInit();

    // this makes sure the code only runs when the client satisfies these
    // version requirements
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
    if (not window) {
        std::cout << "Oops, no window for you!" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (not gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Oops, no glad for you!" << std::endl;
        glfwTerminate();
        return 2;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, frameSizeCallback);

    glEnable(GL_DEPTH_TEST);

    renderLoop(window);

    // call at the end to free up all allocated resources
    glfwTerminate();

    return 0;
}
