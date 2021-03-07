#ifndef A0_SHADER_H
#define A0_SHADER_H

#include<glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    int id;
    glm::mat4 model;
    glm::mat4 projection;
    glm::mat4 view;
    float previousFrameTime;
    float currentFrameTime;

    int compiler(GLenum shaderType, const char *path) {
        std::ifstream shader(path);
        std::stringstream buffer;
        buffer << shader.rdbuf();
        const auto stro = buffer.str();
        const auto str = stro.c_str();

        int shader_id = glCreateShader(shaderType);
        glShaderSource(shader_id, 1, &str, NULL);
        glCompileShader(shader_id);

        int success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

        if (!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shader_id, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderType << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }

        assert(success);

        return shader_id;
    }

public:
    int get_id() {
        return id;
    }

    void setMatrix(const char *name, const glm::mat4 &value) {
        auto loc = glGetUniformLocation(id, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    Shader(const char *vertShaderPath, const char *fragShaderPath) : model(glm::mat4(1.0f)),
                                                                     projection(glm::mat4(1.0f)),
                                                                     view(glm::mat4(1.0f)) {
        int vertShaderId = compiler(GL_VERTEX_SHADER, vertShaderPath);
        int fragShaderId = compiler(GL_FRAGMENT_SHADER, fragShaderPath);

        // link the shaders
        {
            id = glCreateProgram();
            glAttachShader(id, vertShaderId);
            glAttachShader(id, fragShaderId);
            glLinkProgram(id);
            int success;
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            assert(success);
        }

        // delete the shaders
        {
            glDeleteShader(vertShaderId);
            glDeleteShader(fragShaderId);
        }
    }

    ~Shader() {
        glDeleteProgram(id);
    }

    void use() {
        previousFrameTime = currentFrameTime;
        currentFrameTime = glfwGetTime();
        glUseProgram(id);
    }

    void initMatrixes() {
        setMatrix("model", model);
        setMatrix("view", view);
        setMatrix("projection", projection);
    }

    glm::vec3 getTranslation(int axis, int dir) {
        float speed = 0.5f * (currentFrameTime - previousFrameTime);

        glm::vec3 trans;
        trans[axis] = dir * speed;

        return trans;
    }

    void moveObject(int axis, int dir) {
        auto trans = getTranslation(axis, dir);
        model = glm::translate(model, trans);
        setMatrix("model", model);
    }

    void moveCamera(int axis, int dir) {
        auto trans = getTranslation(axis, dir);
        view = glm::translate(view, trans);
        setMatrix("view", view);
    }

    void rotObject(int axis_ind) {
        auto axis = glm::vec3(0.0f, 0.0f, 0.0f);
        axis[axis_ind] = 1;

        auto deltaDegrees = 100 * (currentFrameTime - previousFrameTime);
        model = glm::rotate(model, glm::radians(deltaDegrees), axis);

        setMatrix("model", model);
    }
};


#endif //A0_SHADER_H
