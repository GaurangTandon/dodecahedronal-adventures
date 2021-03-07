#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include <iostream>
#include <cassert>

// clockwise order (?)
float vertices[] = {
        .5f, .5f, 0,
        .5f, -.5f, 0,
        -.5f, -.5f, 0,
        -.5f, .5f, 0
};

unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
};

unsigned int setupEnableVertexAttribs() {
    // create a VAO so that we can set once
    unsigned int vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // TODO: passing vao_id breaks the code; not sure why
    glEnableVertexAttribArray(0);

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

    // This zero corresponds to the zero we mentioned in layout (location=0)
    // still I'm not super clear on this
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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
    glClear(GL_COLOR_BUFFER_BIT);
}

void drawTriangles(unsigned int shader_prog_id, unsigned int vao_id) {
    glUseProgram(shader_prog_id);
    glBindVertexArray(vao_id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void renderLoop(GLFWwindow *window) {
    // toggle for wireframe
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    auto vao_id = setupEnableVertexAttribs();
    auto vbo_id = setupVertexObjects();
    auto ebo_id = setupElmBuffObjects();

    auto shader = Shader("shader.vs", "shader.fs");

    while (not glfwWindowShouldClose(window)) {
        processInput(window);

        updateFrame(window);

        drawTriangles(shader.get_id(), vao_id);

        // swap the currently computed render buffers with whatever is in the
        // window currently
        glfwSwapBuffers(window);
        // poll for keyboard/mouse/etc.
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
    glDeleteBuffers(1, &ebo_id);
    glDeleteProgram(shader.get_id());
}

int main() {
    glfwInit();

    // this makes sure the code only runs when the client satisfies these
    // version requirements
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (not window) {
        std::cout << "Oops, no window for you!" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (not gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Oops, no glad no for you" << std::endl;
        return 2;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, frameSizeCallback);

    renderLoop(window);

    // call at the end to free up all allocated resources
    glfwTerminate();

    return 0;
}
