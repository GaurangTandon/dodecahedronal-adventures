#ifndef A0_HEXA_BIPYRAMID_H
#define A0_HEXA_BIPYRAMID_H

#include "polyhedron.h"
#include <cmath>

class HexagonalBipyramid : public Polyhedron {
    void initVertices() {
        const double PI = std::acos(-1);

        // hexagon vertices in clockwise order
        for (int angle = 0; angle < 360; angle += 60) {
            vertices[totalVerts][0] = cosf(PI * angle / 180) * scale;
            vertices[totalVerts][1] = 0;
            vertices[totalVerts][2] = sinf(PI * angle / 180) * scale;
            totalVerts++;
        }

        // top and bottom, respectively
        for (int side = 0; side < 2; side++) {
            vertices[totalVerts][0] = vertices[totalVerts][2] = 0;
            vertices[totalVerts][1] = (1 - 2 * side) * scale;
            totalVerts++;
        }
    }

    void initFaces() {
        for (unsigned int side = 0; side < 2; side++) {
            for (unsigned int v = 0; v < 6; v++) {
                faces.push_back({v, (v + 1) % 6, 6 + side});
            }
        }
    }

public:
    HexagonalBipyramid(float scale_arg = 1.0f) : Polyhedron(scale_arg) {
        initVertices();
        initFaces();
        finishedInit();
    }
};

#endif //A0_HEXA_BIPYRAMID_H
