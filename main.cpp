#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include <iostream>
#include <cassert>

#include "regular_dodec.h"

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

//@formatter:off
float vertices[] = {
        // vertices, colors
         0.5f,  0.5f, 0.0f, 1, 0, 0,
         0.5f, -0.5f, 0.0f, 0, 1, 0,
        -0.5f, -0.5f, 0.0f, 0, 0, 1,
        -0.5f,  0.5f, 0.0f, 0, 1, 1,
         0.0f,  1.0f, 0.0f, 1, 0, 1
};
//@formatter:on

unsigned int indices[] = {
        0, 1, 2, 3, 4
};

unsigned int setupEnableVertexAttribs() {
    // create a VAO so that we can set once
    unsigned int vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    return vao_id;
}

unsigned int setupVertexObjects() {
    // creating VBO
    unsigned int vbo_id;
    glGenBuffers(1, &vbo_id);
    // now we bind our vbo buffer into array_buffer
    // so when we operate on the array buffer, this vbo will be used
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    // copy buffer data from memory into array buffer for static drawing into the
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // The first argument (0/1) corresponds to the index we mentioned in layout (location=0/1) in the vertex shader
    // The second argument is size of vector
    // last two are stride and starting location respectively
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

    // we just initiated two vertex attrib arrays above, we now initialize them below
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    return vbo_id;
}

unsigned int setupElmBuffObjects() {
    unsigned int ebo_id;
    glGenBuffers(1, &ebo_id);

    // usually we always have to bind the created buffer to some GL buffer
    // and then insert data into that buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return ebo_id;
}

void frameSizeCallback(GLFWwindow *_window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void updateFrame(GLFWwindow *_window) {
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void drawTriangles(Shader &shader, unsigned int vao_id) {
    shader.use();

    glBindVertexArray(vao_id);

    glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, 0);
}

void renderLoop(GLFWwindow *window) {
    // toggle for wireframe
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    auto vao_id = setupEnableVertexAttribs();
    auto vbo_id = setupVertexObjects();
    auto ebo_id = setupElmBuffObjects();

    auto shader = Shader("shader.vs", "shader.fs");
    shader.use();
    shader.initMatrixes();

    while (not glfwWindowShouldClose(window)) {
        processInput(window);

        updateFrame(window);

        drawTriangles(shader, vao_id);

        // swap the currently computed render buffers with whatever is in the
        // window currently
        glfwSwapBuffers(window);
        // poll for keyboard/mouse/etc.
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
    glDeleteBuffers(1, &ebo_id);
}

int main() {
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

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glfwSetFramebufferSizeCallback(window, frameSizeCallback);

    renderLoop(window);

    // call at the end to free up all allocated resources
    glfwTerminate();

    return 0;
}
