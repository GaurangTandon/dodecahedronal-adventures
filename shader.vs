#version 330 core

layout (location = 0) in vec3 inpvec;
layout (location = 1) in vec3 colors;

out vec3 take_colors;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // first apply world space transforms
    // then camera transforms
    // then 3d to 2d projection transforms

    gl_Position = projection * view * model * vec4(inpvec, 1.0);
    take_colors = colors;
}