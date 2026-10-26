#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoords;

uniform mat4 orthoMatrix;

out vec2 TexCoords;

void main()
{
    gl_Position = orthoMatrix * vec4(position.xy, 0.0, 1.0);
    TexCoords = texCoords;
}
