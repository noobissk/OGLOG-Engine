#version 460 core

layout (location = 0) in vec3 _screenCoords;
layout (location = 1) in vec2 _textureCoord;

out vec2 fragTexCoords;

void main()
{
    gl_Position = vec4(_screenCoords, 1.0);
    fragTexCoords = _textureCoord;
}