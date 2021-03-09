#ifndef A0_UNIDECAGON_H
#define A0_UNIDECAGON_H

#include "polyhedron.h"

class Unidecagon : public Polyhedron {
    void initVertices() {
        const double PI = std::acos(-1);

        for (int angle = 0; angle < 360; angle += 360 / 12) {
            vertices[vertCount][0] = cos(PI * angle / 180) * scale;
            vertices[vertCount][2] = 0;
            vertices[vertCount][1] = sin(PI * angle / 180) * scale;
            vertCount++;
        }

        vertices[vertCount][0] = vertices[vertCount][1] = 0;
        vertices[vertCount][2] = 1 * scale;
        vertCount++;
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
