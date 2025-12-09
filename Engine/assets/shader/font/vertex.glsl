#version 460 core

layout (location = 0) in vec2 aPos;   // glyph quad vertex position
layout (location = 1) in vec2 aUV;    // atlas UV coords

uniform mat4 model; // transform
uniform mat4 view; // ortho camera

out vec2 uv;

void main()
{
    gl_Position = view * model * vec4(aPos, 0.0, 1.0);
    uv = aUV;
}