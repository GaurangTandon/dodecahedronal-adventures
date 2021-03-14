#version 330 core
layout (location = 2) in vec3 aPos;
layout (location = 3) in vec2 aTexCoord;

out vec2 TexCoord;
// out vec3 take_colors;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    // take_colors = colors;
}