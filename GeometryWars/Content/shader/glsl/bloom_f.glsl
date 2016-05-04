#version 330 core
in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D screenTexture;

void main()
{
    FragColor = texture(screenTexture, TexCoords);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.1)
        BrightColor = FragColor;
}
