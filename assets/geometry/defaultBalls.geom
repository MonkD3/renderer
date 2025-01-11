#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout (std140) uniform worldBlock {
    mat4 view;
    mat4 proj;
    mat4 iview;
    vec4 light;
    vec4 scalings;
    bvec4 worldOpts;
};

in vec3 vCol[];
in float vRad[];

out vec4 gCenter;
out vec4 gPos;
out vec3 gCol;
out float gRad;

void main() {

    gCenter = gl_in[0].gl_Position;
    gCol = vCol[0];
    gRad = vRad[0];

    vec4 dx = vec4(gRad, 0.0f, 0.0f, 0.0f);
    vec4 dy = vec4(0.0f, gRad, 0.0f, 0.0f);

    gPos = gCenter + iview*(- dx - dy);
    gl_Position = view*gCenter - dx - dy;
    EmitVertex();

    gPos = gCenter + iview*(+ dx - dy);
    gl_Position = view*gCenter + dx - dy;
    EmitVertex();

    gPos = gCenter + iview*(- dx + dy);
    gl_Position = view*gCenter - dx + dy;
    EmitVertex();

    gPos = gCenter + iview*(+ dx + dy);
    gl_Position = view*gCenter + dx + dy;
    EmitVertex();

    EndPrimitive();
}
