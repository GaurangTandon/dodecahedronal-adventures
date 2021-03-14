#ifndef A0_MEME_H
#define A0_MEME_H

#include "regular_dodec.h"

class Meme {
private:
    float vertices[4][5];
    const char *texturePath = "assets/datboi2.jpg";
    unsigned int vao_id;
    unsigned int vbo_id;
    unsigned int texture_id;

    void initVertices() {
        // CW starting from top-left
        vertices[0][2] = vertices[1][2] = vertices[2][2] = vertices[3][2] = 0;

        // 0.75, -0.75
        // -0.75, -0.75
        // -0.75, 0.75
        // 0.75, 0.75
        vertices[0][0] = vertices[3][0] = 0.75;
        vertices[1][0] = vertices[2][0] = -0.50;

        vertices[0][1] = vertices[1][1] = -0.95;
        vertices[2][1] = vertices[3][1] = 0.25;

        // texture coordinates
        // 0, 1
        // 1, 1
        // 1, 0
        // 0, 0
        vertices[0][3] = vertices[3][3] = 0;
        vertices[1][3] = vertices[2][3] = 1;

        vertices[0][4] = vertices[1][4] = 1;
        vertices[2][4] = vertices[3][4] = 0;
    }

    void initVertexBuffer() {
        glGenBuffers(1, &vbo_id);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
    }

    void initTexture() {
        // ---------
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load image, create texture and generate mipmaps

        stbi_set_flip_vertically_on_load(true);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    void vertexAttribs() {
        glGenVertexArrays(1, &vao_id);
        glBindVertexArray(vao_id);
    }

public:
    Meme(float scale = 1.0f) {
        vertexAttribs();
        initVertices();
        initVertexBuffer();
        initTexture();
    }

    ~Meme() {
        glDeleteVertexArrays(1, &vao_id);
    }

    void draw() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glBindVertexArray(vao_id);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
};

#endif //A0_MEME_H
