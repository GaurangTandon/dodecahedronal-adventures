#ifndef A0_CUBE_H
#define A0_CUBE_H

#include "polyhedron.h"

class Cube : public Polyhedron {
    void initVertices() {
        int ind = 0;

        for (int k = 0; k < 2; k++) {
            for (int j = 0; j < 2; j++) {
                for (int i = 0; i < 2; i++) {
                    vertices[ind][0] = (1 - 2 * i) * scale;
                    vertices[ind][1] = (1 - 2 * j) * scale;
                    vertices[ind][2] = (1 - 2 * k) * scale;
                    assignColor(ind);
                    ind++;
                }
            }
        }
    }

    void initFaces() {
        for (int axis = 0; axis < 3; axis++) {
            for (int side = 0; side < 2; side++) {
                int x, y;
                if (axis == 0) x = 1, y = 2;
                else if (axis == 1) x = 0, y = 2;
                else x = 0, y = 1;

                std::vector<unsigned int> face;
#define insert(a, b) \
                        {int mask = (1 << x) * a + (1 << y) * b + (1 << axis) * side; \
                        face.push_back(mask);}

                // insertion must be done in "grey codes"
                insert(0, 0)
                insert(0, 1)
                insert(1, 1)
                insert(1, 0)
                faces.push_back(face);
            }
        }
    }

public:

    Cube(float scale_arg = 1.0f) : Polyhedron(scale_arg) {
        initVertices();
        initFaces();
        finishedInit();
    }

};

#endif //A0_CUBE_H
