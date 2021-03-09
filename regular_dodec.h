#ifndef A0_REGULAR_DODEC_H
#define A0_REGULAR_DODEC_H

#include <cmath>
#include "polyhedron.h"

// given two axis, returns the third axis
int other_axis(const int a, const int b) {
    return 2 + 1 + 0 - a - b;
}

int bit(const int mask, const int ind) {
    return (mask & (1 << ind)) > 0;
}

int insert_between(const int idx, const int mask) {
    if (idx == 0) return mask << 1;
    if (idx == 2) return mask;

    return (bit(mask, 1) << 2) + bit(mask, 0);
}

int remove_between(const int idx, int mask) {
    if (idx == 0) return mask >> 1;
    else if (idx == 1) return ((mask & 4) >> 1) + (mask & 1);
    mask &= 0b011;
    return mask;
}

// Reference: https://en.wikipedia.org/wiki/Regular_dodecahedron#Cartesian_coordinates
class RegularDodecahedron : public Polyhedron {
    // mask = {z,y,x} in this order
    int getVertPenta(int mask) {
        return mask;
    }

    // mask = {z,y,x} in this order
    int getVertPlane(int axis, int mask) {
        int offset = 4 * axis + 8;
        return offset + mask; // insert_between(axis, mask);
    }

    void initVertices() {


#define tripling(x, y, z) \
    for (int k = 0; k <= 1; k++) { \
        for (int j = 0; j <= 1; j++) { \
            for (int i = 0; i <= 1; i++) { \
                vertices[vertCount][0] = (1 - 2 * i) * x * scale; \
                vertices[vertCount][1] = (1 - 2 * j) * y * scale; \
                vertices[vertCount][2] = (1 - 2 * k) * z * scale; \
                vertCount++; \
 } \
 } \
 }

#define doubling(a, b, x, y) \
    for (int i = 0; i <= 1; i++) { \
        for (int j = 0; j <= 1; j++) { \
            vertices[vertCount][a] = (1 - 2 * i) * x * scale; \
            vertices[vertCount][b] = (1 - 2 * j) * y * scale; \
            vertices[vertCount][other_axis(a, b)] = 0;        \
            vertCount++; \
 } \
 }

        float phi = (1 + sqrt(5)) / 2;
        float iphi = 1 / phi;

        // NOTE: THIS EXACT ORDER MATTERS
        // it is being used in vertex number methods above

        // the regular cube (0-7)
        tripling(1, 1, 1);

        // yz plane rect (8-11)
        doubling(2, 1, iphi, phi);

        // xz plane rect (12-15)
        doubling(2, 0, phi, iphi);

        // xy plane rect (16-19)
        doubling(1, 0, iphi, phi);
    }

    void initFaces() {
        // axis=0 => x-axis is normal => yz plane
        // mask has bits set for the directional signs of the other two non-normal components
        // this mask has only two bits
        auto facer = [&](int axis, int mask) {
            int large_mask = insert_between(axis, mask);
            int penta_a = large_mask;
            int penta_b = large_mask | (1 << axis);

            // moving one step forward
//            int axis_2 = axis == 0 ? 1 : axis == 1 ? 2 : 0;
            int axis_2 = (axis + 1) % 3;

            // which direction of the axis_2 plane do we take
            int axis_3 = other_axis(axis, axis_2);
            int compo_axis_2 = (large_mask & (1 << axis_3)) >> axis_3;

            int plane_a_mask = compo_axis_2 * (1 << axis_3);
            int plane_b_mask = compo_axis_2 * (1 << axis_3) + (1 << axis);
            plane_a_mask = remove_between(axis_2, plane_a_mask);
            plane_b_mask = remove_between(axis_2, plane_b_mask);

            unsigned int v1 = getVertPlane(axis, mask),
                    v2 = getVertPenta(penta_a),
                    v3 = getVertPlane(axis_2, plane_a_mask),
                    v4 = getVertPlane(axis_2, plane_b_mask),
                    v5 = getVertPenta(penta_b);

            faces.push_back({v1, v2, v3, v4, v5});
        };

        for (int axis = 0; axis < 3; axis++) {
            for (int mask = 0; mask < 4; mask++) {
                facer(axis, mask);
            }
        }
    }

public:
    RegularDodecahedron(float scale_arg = 1.0f) : Polyhedron(scale_arg) {
#ifndef TEST
        initVertices();
        initFaces();
#endif
        finishedInit();
    }
};


#endif //A0_REGULAR_DODEC_H
