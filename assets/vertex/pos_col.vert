#version 460 core
layout (location = 0) in vec3 iPos; 
layout (location = 1) in vec3 iCol;

uniform mat4 mvp = mat4(
    vec4(1.0f, 0.0f, 0.0f, 0.0f),
    vec4(0.0f, 1.0f, 0.0f, 0.0f),
    vec4(0.0f, 0.0f, 1.0f, 0.0f),
    vec4(0.0f, 0.0f, 0.0f, 1.0f)
);

out vec3 vCol;

void main(){
    gl_Position = mvp*vec4(iPos, 1.0f);
    vCol = iCol;
}
