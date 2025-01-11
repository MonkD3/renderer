#version 460 core
layout (location = 0) in vec4 iPos;

uniform mat4 mvp = mat4(1.0f);

void main(){
    gl_Position = mvp * iPos;
}
