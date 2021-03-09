#ifndef A0_Polyhedron_H
#define A0_Polyhedron_H

#include <GLFW/glfw3.h>
#include <vector>

float COLORS[20][3] = {
        {74, 78, 77},
        {14, 154, 167},
        {61, 164, 171},
        {246, 205, 97},

        {254, 138, 113},
        {253, 244, 152},
        {238, 64, 53},
        {3, 146, 207},

        {123, 192, 67},
        {133, 68, 66},
        {0, 135, 68},
        {220, 237, 193}
};

class Polyhedron {
private:
    unsigned int indices[100];

    void assignColor(int ind, int colorInd) {
        vertices[ind][3] = COLORS[colorInd][0] / 255;
        vertices[ind][4] = COLORS[colorInd][1] / 255;
        vertices[ind][5] = COLORS[colorInd][2] / 255;
    }

protected:
    float scale;

    float vertices[200][6];
    int totalVerts;
    std::vector <std::vector<unsigned int>> faces;

    unsigned int vao_id;
    unsigned int vbo_id;
    unsigned int ebo_id;

protected:

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

    void convertFacesToIndices() {
        int ind = 0;
        bool used[totalVerts];
        memset(used, 0, sizeof(used));

        // copy vertex a into next empty slot
        auto dupe = [&](auto a) {
            vertices[totalVerts][0] = vertices[a][0];
            vertices[totalVerts][1] = vertices[a][1];
            vertices[totalVerts][2] = vertices[a][2];
            totalVerts++;
            return totalVerts - 1;
        };

        int faceInd = 0;
        for (auto &face : faces) {
            for (auto i : face) {
                int target = i;

                if (used[i]) target = dupe(i);
                else used[target] = true;

                std::cout << target << " " << faceInd << std::endl;
                assignColor(target, faceInd);
                indices[ind++] = target;
            }

            faceInd++;
        }
    }

    void finishedInit() {
        convertFacesToIndices();
        setupVertexObjects();
        setupElmBuffObjects();
    }

public:
    Polyhedron(float scale_arg = 1.0f) : scale(scale_arg), totalVerts(0) {
        setupVertexAttribs();
    }

    ~Polyhedron() {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(1, &vbo_id);
        glDeleteBuffers(1, &ebo_id);
    }

    void draw() {
        glBindVertexArray(vao_id);

        int faceCount = faces.size();
        int vertsDone = 0;

        for (int i = 0; i < faceCount; i++) {
            auto offset = vertsDone * sizeof(unsigned int);
            int vertCount = int(faces[i].size());

            glDrawElements(GL_TRIANGLE_FAN, vertCount, GL_UNSIGNED_INT, (void *) offset);

            vertsDone += vertCount;
        }
    }
};


#endif //A0_Polyhedron_H
