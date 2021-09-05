#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

in vec3 OriPos;
in vec3 FragPos;
in vec3 Normal;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(Normal);
    float dist = abs(dot(OriPos, OriPos));
    gColor = vec4(sin(50 * dist), sin(dist) * cos(dist), cos(dist), gl_FragCoord.z);
}