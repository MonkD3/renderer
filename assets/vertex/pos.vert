#version 460 core
layout (location = 0) in vec4 iPos; 

void main(){
    gl_Position = iPos;
}
