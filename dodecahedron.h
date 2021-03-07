#ifndef A0_DODECAHEDRON_H
#define A0_DODECAHEDRON_H

#include <vector>
#include <GLFW/glfw3.h>

const int FACE_COUNT = 12;
const int VERT_PER_FACE_COUNT = 5;

class Dodecahedron {
protected:
    float scale;
    float vertices[20][6];
    unsigned int indices[60];
    unsigned int vao_id;
    unsigned int vbo_id;
    unsigned int ebo_id;

protected:
    // Test vertices for a rainbow pentagon
    // set the indices element buffer object appropriately
//@formatter:off
//    float vertices[] = {
//            // vertices, colors
//            0.5f,  0.5f, 0.0f, 1, 0, 0,
//            0.5f, -0.5f, 0.0f, 0, 1, 0,
//            -0.5f, -0.5f, 0.0f, 0, 0, 1,
//            -0.5f,  0.5f, 0.0f, 0, 1, 1,
//            0.0f,  1.0f, 0.0f, 1, 0, 1
//    };
//@formatter:on

    void setupEnableVertexAttribs() {
        // create a VAO so that we can set once
        glGenVertexArrays(1, &vao_id);
        glBindVertexArray(vao_id);
    }

    void setupVertexObjects() {
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
    }

    void setupElmBuffObjects() {
        glGenBuffers(1, &ebo_id);

        // usually we always have to bind the created buffer to some GL buffer
        // and then insert data into that buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void finishedInit() {
        setupVertexObjects();
        setupElmBuffObjects();
    }

public:
    Dodecahedron(float scale = 1.0f) : scale(scale) {
        setupEnableVertexAttribs();
    }

    ~Dodecahedron() {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(1, &vbo_id);
        glDeleteBuffers(1, &ebo_id);
    }

    void draw() {
        glBindVertexArray(vao_id);

        for (int i = 0; i < FACE_COUNT; i++) {
            auto offset = VERT_PER_FACE_COUNT * i * sizeof(unsigned int);
            glDrawElements(GL_TRIANGLE_FAN, VERT_PER_FACE_COUNT, GL_UNSIGNED_INT, (void *) offset);
        }
    }
};


#endif //A0_DODECAHEDRON_H
