#version 460 core
layout (lines) in; 
layout (triangle_strip, max_vertices = 4) out;

in vec3 vCol[];
out vec3 gCol;

uniform mat4 mvp;

void main(){
    vec4 s = mvp * gl_in[0].gl_Position;
    vec4 t = mvp * gl_in[1].gl_Position;

    // Direction and normal
    vec4 d = vec4(normalize((t.xy - s.xy)), 0.0f, 0.0f);
    vec4 n = vec4(vec2(-d.y, d.x), 0.0f, 0.0f);

    // Width
    float w = 0.01f;

    gCol = vCol[0];
    gl_Position = s - w*n;
    EmitVertex();

    gCol = vCol[0];
    gl_Position = s + w*n;
    EmitVertex();

    gCol = vCol[0];
    gl_Position = t - w*n;
    EmitVertex();

    gCol = vCol[0];
    gl_Position = t + w*n;
    EmitVertex();
    
    EndPrimitive();
}
