#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 vCol[];
in float vRad[];

out vec4 gCenter;
out vec4 gPos;
out vec3 gCol;
out float gRad;

uniform mat4 mvp = mat4(
    vec4(1.0f, 0.0f, 0.0f, 0.0f),
    vec4(0.0f, 1.0f, 0.0f, 0.0f),
    vec4(0.0f, 0.0f, 1.0f, 0.0f),
    vec4(0.0f, 0.0f, 0.0f, 1.0f)
);
uniform mat4 imvp;

void main() {

    gCenter = gl_in[0].gl_Position;
    gCol = vCol[0];
    gRad = vRad[0];

    // vec4 dx = imvp*vec4(gRad, 0.0f, 0.0f, 0.0f);
    // vec4 dy = imvp*vec4(0.0f, gRad, 0.0f, 0.0f);

    // vec4 p1 = gCenter - dx - dy;
    // vec4 p2 = gCenter + dx - dy;
    // vec4 p3 = gCenter - dx + dy;
    // vec4 p4 = gCenter + dx + dy;

    // gPos = p1;
    // gl_Position = mvp*p1;
    // EmitVertex();

    // gPos = p2;
    // gl_Position = mvp*p2;
    // EmitVertex();

    // gPos = p3;
    // gl_Position = mvp*p3;
    // EmitVertex();

    // gPos = p4;
    // gl_Position = mvp*p4;
    // EmitVertex();

    // EndPrimitive();

    vec4 dx = vec4(gRad, 0.0f, 0.0f, 0.0f);
    vec4 dy = vec4(0.0f, gRad, 0.0f, 0.0f);

    gPos = gCenter + imvp*(- dx - dy);
    gl_Position = mvp*gCenter - dx - dy;
    EmitVertex();

    gPos = gCenter + imvp*(+ dx - dy);
    gl_Position = mvp*gCenter + dx - dy;
    EmitVertex();

    gPos = gCenter + imvp*(- dx + dy);
    gl_Position = mvp*gCenter - dx + dy;
    EmitVertex();

    gPos = gCenter + imvp*(+ dx + dy);
    gl_Position = mvp*gCenter + dx + dy;
    EmitVertex();

    EndPrimitive();
}
