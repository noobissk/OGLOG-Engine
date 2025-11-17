#version 460 core

in vec2 fragTexCoords;

uniform sampler2D ourTexture;

out vec4 oFragColor;

void main()
{
    oFragColor = texture(ourTexture, fragTexCoords);
}