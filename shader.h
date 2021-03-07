#ifndef A0_SHADER_H
#define A0_SHADER_H

// TODO: include only as much as required
#include<glad/glad.h>

#include <glm/glm.hpp>
#include <fstream>
#include <sstream>

class Shader {
    int id;

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

    Shader(const char *vertShaderPath, const char *fragShaderPath) {
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
};


#endif //A0_SHADER_H
