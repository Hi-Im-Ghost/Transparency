#version 330 core
layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec2 TexCoord;

out vec3 outColor;
out vec2 outTexCoord;
void main()
{
    gl_Position = vec4(Pos, 1.0);
    outColor = Color;
    outTexCoord = TexCoord;
}