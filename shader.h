#ifndef A0_SHADER_H
#define A0_SHADER_H

#include<glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>

class Shader {
    int id;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 projection;

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
        glUseProgram(id);
    }

    void initMatrixes() {
        setMatrix("model", model);
        setMatrix("view", view);
        setMatrix("projection", projection);
    }
};


#endif //A0_SHADER_H