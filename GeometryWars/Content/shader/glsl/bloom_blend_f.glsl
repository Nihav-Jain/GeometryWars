#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;

void main()
{
    vec4 hdrColor = texture(scene, TexCoords);
    vec4 bloomColor = texture(bloomBlur, TexCoords);
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec4 result = vec4(1.0) - exp(-hdrColor * exposure);

    FragColor = result;
}
