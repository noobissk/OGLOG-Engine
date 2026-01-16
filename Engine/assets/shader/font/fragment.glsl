#version 460 core

in vec2 uv;

uniform sampler2D sdfAtlas;
uniform vec4 textColor;
uniform float smoothing;

out vec4 FragColor;

void main()
{
    float dist = 1 - texture(sdfAtlas, uv).r;

    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist);

    FragColor = vec4(textColor.rgb, alpha);
}

