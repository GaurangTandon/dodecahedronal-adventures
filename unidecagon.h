#ifndef A0_UNIDECAGON_H
#define A0_UNIDECAGON_H

#include "polyhedron.h"

class Unidecagon : public Polyhedron {
    void initVertices() {
        const double PI = std::acos(-1);

        for (int angle = 0; angle < 360; angle += 360 / 12) {
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
        for (unsigned int i = 0; i < 12; i++)
            faces.push_back({i, (i + 1) % 12, 12});

        std::vector<unsigned int> bigface;
        for (unsigned int i = 0; i < 12; i++) bigface.push_back(i);
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
