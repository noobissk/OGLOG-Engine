#version 460 core

in vec2 uv;

uniform sampler2D sdfAtlas;
uniform vec4 textColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform float smoothing = 0.02f;

out vec4 FragColor;

void main()
{
    float dist = 1.0 - texture(sdfAtlas, uv).r;

    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist);

    FragColor = vec4(textColor.rgb, textColor.a * alpha);
}