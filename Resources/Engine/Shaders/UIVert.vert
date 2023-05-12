#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 ortho;

void main()
{
    gl_Position = model * vec4(inPosition, 1.0);
    TexCoords = inTexCoord;
}  