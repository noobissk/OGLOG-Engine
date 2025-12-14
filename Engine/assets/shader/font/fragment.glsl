#version 460 core

in vec2 uv;

uniform sampler2D sdfAtlas;      // your SDF atlas texture
uniform vec4 textColor;          // RGBA color of the text
uniform float smoothing;         // recommended: 0.08–0.18 depending on resolution

out vec4 FragColor;

void main()
{
    float dist = 1 - texture(sdfAtlas, uv).r;

    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist);

    FragColor = vec4(textColor.rgb, alpha);
}

