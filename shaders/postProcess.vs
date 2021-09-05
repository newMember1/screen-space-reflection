#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
    TexCoords = tex;
    gl_Position = vec4(0.8 * pos, 1.0);
}