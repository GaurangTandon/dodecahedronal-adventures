#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include <tuple>
#include <iostream>
#include <cassert>
#include <vector>

#include "regular_dodec.h"
#include "hexa_bipyramid.h"
#include "cube.h"
#include "camera.h"

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

bool ROTATING_X = false;
bool ROTATING_Y = false;
bool ROTATING_Z = false;

void frameSizeCallback(GLFWwindow *_window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Shader &shader) {
    std::vector <std::tuple<int, int, int>> objectMappings = {
            {GLFW_KEY_Q, 0, 1},
            {GLFW_KEY_A, 0, -1},
            {GLFW_KEY_W, 1, 1},
            {GLFW_KEY_S, 1, -1},
            {GLFW_KEY_E, 2, 1},
            {GLFW_KEY_D, 2, -1}
    };

    std::vector <std::tuple<int, int, int>> cameraMappings = {
            {GLFW_KEY_R, 0, 1},
            {GLFW_KEY_F, 0, -1},
            {GLFW_KEY_T, 1, 1},
            {GLFW_KEY_G, 1, -1},
            {GLFW_KEY_Y, 2, 1},
            {GLFW_KEY_H, 2, -1}
    };

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
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

    // reset all rotations :P
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        ROTATING_X = ROTATING_Y = ROTATING_Z = false;
        return;
    }

    for (auto &[key, axis, dir] : objectMappings) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            shader.moveObject(axis, dir);
            return;
        }
    }

    for (auto &[key, axis, dir] : cameraMappings) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            shader.moveCamera(axis, dir);
            return;
        }
    }
}

void updateFrame(GLFWwindow *_window) {
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void calcProjections(Camera &camera, Shader &shader) {
    // pass projection matrix to shader (note that in this case it could change every frame)
    auto projection = glm::perspective(glm::radians(camera.Zoom), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f,
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

    auto object = HexagonalBipyramid(0.5f);

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    while (not glfwWindowShouldClose(window)) {
        processInput(window, shader);

        updateFrame(window);

        drawObjects(camera, shader, object);

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
