#version 460 core

layout (location = 0) in vec3 _screenCoords;
layout (location = 1) in vec2 _textureCoord;

out vec2 fragTexCoords;
out vec2 uvCoords;

uniform mat4 model; // transform
uniform mat4 view; // camera
uniform mat4 projection; // ortho/perspect

void main()
{
    gl_Position = projection * view * model * vec4(_screenCoords, 1.0);
    fragTexCoords = _textureCoord;
    uvCoords = _textureCoord;
}