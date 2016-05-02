#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model_view_projection;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = model_view_projection * vec4(vertex.xy, 0.0, 1.0);
}
