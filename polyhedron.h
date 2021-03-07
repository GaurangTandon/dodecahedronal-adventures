#ifndef A0_Polyhedron_H
#define A0_Polyhedron_H

#include <GLFW/glfw3.h>

//#define TEST

//float COLORS[20][3] = {
//        {242, 171, 195},
//        {47,  233, 204},
//        {34,  154, 14},
//        {153, 116, 202},
//        {219, 8,   62},
//        {83,  15,  236},
//        {49,  177, 168},
//        {236, 183, 123},
//        {81,  92,  82},
//        {114, 232, 177},
//        {96,  86,  3},
//        {51,  95,  95},
//        {131, 195, 180},
//        {240, 85,  219},
//        {116, 213, 201},
//        {114, 156, 154},
//        {13,  213, 195},
//        {85,  113, 146},
//        {88,  59,  225},
//        {101, 100, 178}
//};

float COLORS[20][3] = {
        {255, 0,   0},
        {0,   255, 0},
        {0,   0,   255},

        {255, 0,   255},
        {0,   255, 255},
        {255, 255, 0},

        {125, 0,   0},
        {0,   125, 0},
        {0,   0,   125},

        {0,   125, 125},
        {125, 0,   125},
        {125, 125, 0},

        {40,  120, 200},
        {120, 40,  200},
        {120, 200, 40},

        {90,  120, 150},
        {120, 90,  150},
        {120, 150, 90},
        {90,  150, 120},
        {150, 90,  120}
};

class Polyhedron {
private:
    int vertCountPerFace;
    int faceCount;

protected:
    float scale;

#ifndef TEST
    float vertices[20][6];
    unsigned int indices[60];
#endif
    unsigned int vao_id;
    unsigned int vbo_id;
    unsigned int ebo_id;

protected:
//@formatter:off
#ifdef TEST
    // Test vertices for a rainbow pentagon
    float vertices[5][6] = {
            // vertices, colors
            0.5f,  0.5f, 0.0f, 1, 0, 0,
            0.5f, -0.5f, 0.0f, 0, 1, 0,
            -0.5f, -0.5f, 0.0f, 0, 0, 1,
            -0.5f,  0.5f, 0.0f, 0, 1, 1,
            0.0f,  1.0f, 0.0f, 1, 0, 1
    };
    unsigned int indices[5] = {
            0, 1, 2, 3, 4
    };
#endif
//@formatter:on

    void setupVertexAttribs() {
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

    void assignColor(int ind) {
        vertices[ind][3] = COLORS[ind][0] / 255;
        vertices[ind][4] = COLORS[ind][1] / 255;
        vertices[ind][5] = COLORS[ind][2] / 255;
    }

public:
    Polyhedron(float scale_arg = 1.0f, int vertCount = 5, int faces = 12) : scale(scale_arg),
                                                                            vertCountPerFace(vertCount),
                                                                            faceCount(faces) {
        setupVertexAttribs();
    }

    ~Polyhedron() {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(1, &vbo_id);
        glDeleteBuffers(1, &ebo_id);
    }

    void draw() {
        glBindVertexArray(vao_id);

#ifdef TEST
        glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, (void *) 0);
#else
        const int TRIANGLE_SIDES = 3;

        for (int i = 0; i < faceCount; i++) {
            auto offset = vertCountPerFace * i * sizeof(unsigned int);

            glDrawElements(GL_TRIANGLE_FAN, vertCountPerFace, GL_UNSIGNED_INT, (void *) offset);
        }
#endif
    }
};


#endif //A0_Polyhedron_H
