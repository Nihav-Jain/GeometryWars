#version 330 core
layout (location = 0) in vec3 vertex; // vec2 position

uniform mat4 model_view_projection;

void main()
{
    gl_Position = model_view_projection * vec4(vertex.xy, 0.0, 1.0);
}
