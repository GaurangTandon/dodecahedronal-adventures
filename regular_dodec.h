#ifndef A0_REGULAR_DODEC_H
#define A0_REGULAR_DODEC_H

#include <cmath>
#include "dodecahedron.h"

// Reference: https://en.wikipedia.org/wiki/Regular_dodecahedron#Cartesian_coordinates
class RegularDodecahedron : Dodecahedron {
    void initVertices() {
#define tripling(x, y, z) \
    for (int i = 0; i <= 1; i++) { \
        for (int j = 0; j <= 1; j++) { \
            for (int k = 0; k <= 1; k++) { \
                vertices[nxt][0] = (1 - 2 * i) * x; \
                vertices[nxt][1] = (1 - 2 * j) * y; \
                vertices[nxt][2] = (1 - 2 * k) * z; \
                nxt++; \
 } \
 } \
 }

#define doubling(a, b, x, y) \
    for (int i = 0; i <= 1; i++) { \
        for (int j = 0; j <= 1; j++) { \
            vertices[nxt][a] = (1 - 2 * i) * x; \
            vertices[nxt][b] = (1 - 2 * j) * y; \
            vertices[nxt][2 + 1 + 0 - a - b] = 0; \
            nxt++; \
 } \
 }

        int nxt = 0;
        float phi = (1 + sqrt(5)) / 2;
        float iphi = 1 / phi;

        // the regular cube (0-7)
        tripling(1, 1, 1);

        // yz plane rect (8-11)
        doubling(1, 2, phi, iphi);

        // xy plane rect (12-15)
        doubling(0, 1, phi, iphi);

        // xz plane rect (16-19)
        doubling(0, 2, iphi, phi);
    }

    void initFaces() {
        auto facer = [&](int w, int x, int y, int z, int plane, int side) {
            int a, b;

            if (plane == 0) { // yz
                if (side == -1) {
                    a = 8 + 2;
                    b = 8 + 2 + 1;
                }
            }

            faces.push_back({w, x, b, a});
        };

        facer(0b010, 0b011, 0b110, 0b111, 0, -1);
    }

public:
    RegularDodecahedron() : Dodecahedron() {
        initVertices();
        initFaces();
    }
};


#endif //A0_REGULAR_DODEC_H
