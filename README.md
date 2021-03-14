# Graphics Assignment Zero

## Running

`make && ./main`

Requires glad, glfw3 and glm to be in the include path. Best way to do that is install them into your `/usr/local/include`.

## Modelling

All vertex coordinates and corresponding face indices are calculated inside of code. Look into `initVertices` and `initFaces` for the calculations.

1. Regular Dodecahedron: `regular_dodec.h`
2. Hexagonal Bipyramid: `hexa_bipyramid.h`
3. Undecagonal pyramid: `unidecagon.h`
4. Cube: `cube.h` (extra model)

All classes are very modular, and extend from the base class `Polyhedron.h`, which handles vertex duplication, coloring, drawing, etc.

**Keystrokes:** Press 0 for regular dodec, 1 for hexagonal bipy, 2 for undecagon py, and 3 for cube.

## Animation

1. For translation of camaera, use: R for +X, F for -X, T for +Y, G for -Y, Y for +Z, H for -Z, where X,Y,Z are the respective axis.
2. For translation of object, use: Q for +X, A for -X, W for +Y, S for -Y, E for +Z, D for -Z, where X,Y,Z are the respective axis.
3. Key to change camera position to three predefined positions: positive Z axis: 4, from negative Z axis: 5, from 45deg of XZ plane: 6.
4. Key to rotate object about Z axis: Z, about X axis: X, and about Y axis: C.
5. Press O (alphabet O) to rotate camera about the object (when object is at origin).

Additional:

1. Key to reset object position and camera position: V.

### Notes

Object moves along global X, Y, Z axis which do not change even if the object is rotated.

## BONUS: Memes (story/caption) and textures

Press P to get a DatBoi riding a uni-dodec :D The uni-dodec also rotates, btw, like a unicycle wheel would.
