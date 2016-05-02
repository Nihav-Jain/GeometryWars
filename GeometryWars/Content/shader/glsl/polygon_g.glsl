#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float width;
uniform mat4 model_view_projection;

void main()
{
    vec2 line = vec2(gl_in[0].gl_Position - gl_in[1].gl_Position);
    vec2 normal1 = normalize(vec2(-line.y, line.x));
    vec2 normal2 = normalize(vec2(line.y, -line.x));

    vec2 point = gl_in[0].gl_Position.xy + width * 0.01 * normal1;
    gl_Position = model_view_projection * vec4(point, 0.0, 1.0);
    EmitVertex();

    point = gl_in[0].gl_Position.xy + width * 0.01 * normal2;
    gl_Position = model_view_projection * vec4(point, 0.0, 1.0);
    EmitVertex();

    point = gl_in[1].gl_Position.xy + width * 0.01 * normal1;
    gl_Position = model_view_projection * vec4(point, 0.0, 1.0);
    EmitVertex();

    point = gl_in[1].gl_Position.xy + width * 0.01 * normal2;
    gl_Position = model_view_projection * vec4(point, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
