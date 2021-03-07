#version 330 core

layout (location = 0) in vec3 inpvec;
layout (location = 1) in vec3 colors;

out vec3 take_colors;

void main() {
    gl_Position = vec4(inpvec, 1.0);
    take_colors = colors;
}