#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>

// clockwise order (?)
float vertices[] = {
        .0f, .5f, 0,
        .5f, -.5f, 0,
        -.5f, -.5f, 0
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

unsigned int setupShaders() {
    // vertex shader: create and compile
    unsigned int vertex_shader_id;
    {
        const char *vertexShaderGLSL = "#version 330 core\n"
                                       "layout (location = 0) in vec3 inpvec;\n"
                                       "void main() {\n"
                                       "    gl_Position = vec4(inpvec, 1.0);\n"
                                       "}\n";

        vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_id, 1, &vertexShaderGLSL, NULL);
        glCompileShader(vertex_shader_id);

        int success;
        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
        assert(success);
    }

    // fragment shader: create and compile
    unsigned int frag_shader_id;
    {
        const char *fragmentShaderGLSL = "#version 330 core\n"
                                         "out vec4 frag_colour;\n"
                                         "void main() {\n"
                                         "    frag_colour = vec4(0.5, 0.5, 0.5, 1.0);\n"
                                         "}\n";

        frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader_id, 1, &fragmentShaderGLSL, NULL);
        glCompileShader(frag_shader_id);
        int success;
        glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &success);
        assert(success);
    }

    // shader program: create and link
    unsigned int shader_program_id;
    {
        shader_program_id = glCreateProgram();
        glAttachShader(shader_program_id, vertex_shader_id);
        glAttachShader(shader_program_id, frag_shader_id);
        glLinkProgram(shader_program_id);
        int success;
        glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
        assert(success);
    }

    // delete after linking
    glDeleteShader(vertex_shader_id);
    glDeleteShader(frag_shader_id);

    return shader_program_id;
}

void frameSizeCallback(GLFWwindow *_window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void updateFrame(GLFWwindow *window) {
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void drawTriangles(unsigned int shader_prog_id, unsigned int vao_id) {
    glUseProgram(shader_prog_id);
    glBindVertexArray(vao_id);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void renderLoop(GLFWwindow *window) {
    auto vao_id = setupEnableVertexAttribs();
    auto vbo_id = setupVertexObjects();
    auto shader_prog_id = setupShaders();

    while (not glfwWindowShouldClose(window)) {
        processInput(window);

        updateFrame(window);

        drawTriangles(shader_prog_id, vao_id);

        // swap the currently computed render buffers with whatever is in the
        // window currently
        glfwSwapBuffers(window);
        // poll for keyboard/mouse/etc.
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
    glDeleteProgram(shader_prog_id);
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
