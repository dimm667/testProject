#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 14) out;

void build_cube(vec4 position, float size)
{
    gl_Position = position + size * vec4(-1.0, 1.0, 1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(1.0, 1.0, 1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(-1.0, -1.0, 1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(1.0, -1.0, 1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(1.0, -1.0, -1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(1.0, 1.0, 1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(1.0, 1.0, -1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(-1.0, 1.0, 1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(-1.0, 1.0, -1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(-1.0, -1.0, 1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(-1.0, -1.0, -1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(1.0, -1.0, -1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(-1.0, 1.0, -1.0, 0.0);
    EmitVertex();
    gl_Position = position + size * vec4(1.0, 1.0, -1.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    build_cube(gl_in[0].gl_Position, 0.1);
}
