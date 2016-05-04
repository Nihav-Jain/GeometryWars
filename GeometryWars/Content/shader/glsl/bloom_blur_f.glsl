#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform float colorThreshold;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
    vec4 result = texture(image, TexCoords) * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            vec4 color1 = texture(image, TexCoords + vec2(tex_offset.x * i, 0.0));
            vec4 color2 = texture(image, TexCoords - vec2(tex_offset.x * i, 0.0));
            result.rgb += color1.rgb * weight[i];
            result.a += color1.a * weight[i] * 2;
            result.rgb += color2.rgb * weight[i];
            result.a += color2.a * weight[i] * 2;
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            vec4 color1 = texture(image, TexCoords + vec2(0.0, tex_offset.y * i));
            vec4 color2 = texture(image, TexCoords - vec2(0.0, tex_offset.y * i));
            result.rgb += color1.rgb * weight[i];
            result.a = color1.a * weight[i] * 2;
            result.rgb += color2.rgb * weight[i];
            result.a = color2.a * weight[i] * 2;
        }
    }

    if (length(result.rgb) > colorThreshold) {
        FragColor = vec4(result.rgb, 1.0);
    } else {
        FragColor = result;
    }
}
