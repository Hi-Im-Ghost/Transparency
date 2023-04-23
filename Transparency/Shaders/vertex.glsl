#version 330 core
layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Color;

out vec3 outColor;

void main()
{
    gl_Position = vec4(Pos, 1.0);
    outColor = Color;
}