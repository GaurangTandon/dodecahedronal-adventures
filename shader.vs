#version 330 core
layout (location = 0) in vec3 inpvec;
void main() {
    gl_Position = vec4(inpvec, 1.0);
}