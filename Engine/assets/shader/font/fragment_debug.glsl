#version 460 core

in vec2 uv;

out vec4 FragColor;

void main()
{
    // Render UV coordinates as RGB colors for debugging
    // U (0-1) maps to Red, V (0-1) maps to Green
    FragColor = vec4(uv.x, uv.y, 0.0, 1.0);
}
