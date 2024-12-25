#version 460 core

layout (location = 0) in vec4 iPos;
layout (location = 1) in float iVal;

out float TexCoord;

void main(){
    gl_Position = iPos;
    TexCoord = iVal;
}
