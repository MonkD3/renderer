#version 460 core
layout (location = 0) in vec4 iPos; 
layout (location = 1) in vec3 iCol;

uniform mat4 mvp = mat4(1.0f);

out vec3 vCol;

void main(){
    gl_Position = mvp*iPos;
    vCol = iCol;
}
