#ifndef A0_DODECAHEDRON_H
#define A0_DODECAHEDRON_H

#include <vector>

class Dodecahedron {
protected:
    float vertices[20][3];

public:
    std::vector <std::vector<int>> faces;
};


#endif //A0_DODECAHEDRON_H
