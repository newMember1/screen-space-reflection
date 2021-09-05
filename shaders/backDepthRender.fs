#version 330 core
layout (location = 3) out vec3 backDepth;

void main()
{
    backDepth = vec3(gl_FragCoord.z);
}