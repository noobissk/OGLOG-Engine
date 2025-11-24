#version 460 core

in vec2 fragTexCoords;
in vec2 uvCoords;

uniform sampler2D ourTexture;

out vec4 oFragColor;

void main()
{
    oFragColor = texture(ourTexture, uvCoords);
}