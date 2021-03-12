#ifndef A0_UNIDECAGON_H
#define A0_UNIDECAGON_H

#include "polyhedron.h"
#include <numeric>

class Unidecagon : public Polyhedron {
    unsigned int vertsAtBase = 11;

    void initVertices() {
        const double PI = std::acos(-1);
        float angle = 0;
        float increment = 360.0f / float(vertsAtBase);

        for (unsigned int iter = 0; iter < vertsAtBase; iter++, angle += increment) {
            vertices[totalVerts][0] = cosf(PI * angle / 180) * scale;
            vertices[totalVerts][2] = 0;
            vertices[totalVerts][1] = sinf(PI * angle / 180) * scale;
            totalVerts++;
        }

        vertices[totalVerts][0] = vertices[totalVerts][1] = 0;
        vertices[totalVerts][2] = 1 * scale;
        totalVerts++;
    }

    void initFaces() {
        for (unsigned int i = 0; i < vertsAtBase; i++)
            faces.push_back({i, (i + 1) % vertsAtBase, vertsAtBase});

        std::vector<unsigned int> bigface(vertsAtBase);
        std::iota(bigface.begin(), bigface.end(), 0);
        faces.push_back(bigface);
    }

public:
    Unidecagon(float scale_arg = 1.0f) : Polyhedron(scale_arg) {
        initVertices();
        initFaces();
        finishedInit();
    }
};

#endif //A0_UNIDECAGON_H
