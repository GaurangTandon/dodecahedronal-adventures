#version 330 core

in vec3 take_colors;
out vec4 frag_colour;

void main() {
    frag_colour = vec4(take_colors, 1);
}