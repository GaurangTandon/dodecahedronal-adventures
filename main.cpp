#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include "shader.h"
#include <tuple>
#include <iostream>
#include <vector>

#include "regular_dodec.h"
#include "hexa_bipyramid.h"
#include "unidecagon.h"
#include "cube.h"
#include "camera.h"
#include "meme.h"

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

bool ROTATING_X = false;
bool ROTATING_Y = false;
bool ROTATING_Z = false;
int CURR_OBJECT = 0;
bool MEME_ON = false;

bool ROTATE_CAM = false;

void frameSizeCallback(GLFWwindow *_window, int width, int height) {
    glViewport(0, 0, width, height);
}

void resetShader(Shader &shader) {
    shader.reset();
    ROTATING_X = ROTATING_Y = ROTATING_Z = false;
}

void resetCamera(Camera &camera) {
    ROTATE_CAM = false;
    camera.reset();
}

struct myShaders {
    Shader meme, obj;

    myShaders(Shader &o, Shader &m) : obj(o), meme(m) {
    }
};

void resetAll(myShaders &shaders, Camera &camera) {
    resetShader(shaders.meme);
    resetShader(shaders.obj);

    resetCamera(camera);
    MEME_ON = false;
    CURR_OBJECT = 0;
}

void initMeme() {
    MEME_ON = true;
}

void processInput(GLFWwindow *window, myShaders &shaders, Camera &camera, const glm::vec4 &centroid) {
#define pressed(x) (glfwGetKey(window, x) == GLFW_PRESS)

    std::vector<std::tuple<int, int, int>> objectMappings = {
            {GLFW_KEY_Q, 0, 1},
            {GLFW_KEY_A, 0, -1},
            {GLFW_KEY_W, 1, 1},
            {GLFW_KEY_S, 1, -1},
            {GLFW_KEY_E, 2, 1},
            {GLFW_KEY_D, 2, -1}
    };

    std::vector<std::tuple<int, int>> cameraMappings = {
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

    if (pressed(GLFW_KEY_Z)) {
        ROTATING_Z = not ROTATING_Z;
        return;
    }

    if (pressed(GLFW_KEY_X)) {
        ROTATING_X = not ROTATING_X;
        return;
    }

    if (pressed(GLFW_KEY_C)) {
        ROTATING_Y = not ROTATING_Y;
        return;
    }

    // reset everything
    if (pressed(GLFW_KEY_V)) {
        resetAll(shaders, camera);
        return;
    }

    if (pressed(GLFW_KEY_O)) {
//        resetCamera(camera);
        ROTATE_CAM = not ROTATE_CAM;

        return;
    }

    for (int i = 0; i < 3; i++) {
        if (pressed(GLFW_KEY_4 + i)) {
            resetCamera(camera);
            camera.predefinedJump(i, shaders.obj.positionCentroid(centroid));
            return;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (pressed(GLFW_KEY_0 + i)) {
            CURR_OBJECT = i;
            return;
        }
    }

    for (auto &[key, axis, dir] : objectMappings) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            if (MEME_ON) {
                shaders.obj.moveObject(axis, dir);
                shaders.meme.moveObject(axis, dir);
            } else {
                shaders.obj.moveObject(axis, dir);
            }
            return;
        }
    }

    for (auto &[key, dir] : cameraMappings) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            camera.translate(dir, shaders.obj.getTimeDifference());
            return;
        }
    }

    if (pressed(GLFW_KEY_P)) {
        resetAll(shaders, camera);
        initMeme();
        return;
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

void initCamera(Camera &camera, Shader &shader) {
    if (ROTATE_CAM) camera.rotate();

    calcProjections(camera, shader);
}

void initShader(Shader &shader, bool rot = true) {
    shader.use();

    if (rot) {
        if (ROTATING_X)
            shader.rotObject(0);

        if (ROTATING_Y)
            shader.rotObject(1);

        if (ROTATING_Z)
            shader.rotObject(2);
    }
}

void renderLoop(GLFWwindow *window) {
    // toggle for wireframe
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    auto objShader = Shader("shader.vs", "shader.fs");
    auto memeShader = Shader("meme_shader.vs", "meme_shader.fs");
    myShaders mss(objShader, memeShader);
    mss.obj.use();
    mss.obj.initMatrixes();
    mss.meme.use();
    mss.meme.initMatrixes();
    mss.meme.setTexture("ourTexture");

    float scale = 0.5f;
    mss.obj.use();
    RegularDodecahedron reg = RegularDodecahedron(scale);
    RegularDodecahedron reg_meme = RegularDodecahedron(scale / 2.75);
    HexagonalBipyramid hex = HexagonalBipyramid(scale);
    Unidecagon unid = Unidecagon(scale);
    Cube cube = Cube(scale);

    mss.meme.use();
    Meme meme = Meme(scale);

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

        glm::vec4 centroid;
        if (CURR_OBJECT == 0)
            centroid = reg.centroid;
        else if (CURR_OBJECT == 1)
            centroid = hex.centroid;
        else if (CURR_OBJECT == 2)
            centroid = unid.centroid;
        else
            centroid = cube.centroid;

        processInput(window, mss, camera, centroid);

        updateFrame(window);

        if (MEME_ON) {
            initShader(mss.meme, false);
            initCamera(camera, mss.meme);

            meme.draw();

            initShader(mss.obj);
            initCamera(camera, mss.obj);

            ROTATING_Z = true;
//            mss.obj.moveObject(1, -1, 10.0f);
            reg_meme.draw();
        } else {
            initShader(mss.obj);
            initCamera(camera, mss.obj);

            if (CURR_OBJECT == 0)
                reg.draw();
            else if (CURR_OBJECT == 1)
                hex.draw();
            else if (CURR_OBJECT == 2)
                unid.draw();
            else
                cube.draw();
        }

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

    auto window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment Zero", NULL, NULL);
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
