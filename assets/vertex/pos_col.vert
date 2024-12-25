#version 460 core
layout (location = 0) in vec4 iPos; 
layout (location = 1) in vec3 iCol;

out vec3 vCol;

void main(){
    gl_Position = iPos;
    vCol = iCol;
}
