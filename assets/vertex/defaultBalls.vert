#version 460 core
layout (location = 0) in vec3 iPos; 
layout (location = 1) in float iRad;
layout (location = 2) in vec3 iCol;

out vec3 vCol;
out float vRad;

void main(){
    gl_Position = vec4(iPos, 1.0f);
    vCol = iCol;
    vRad = iRad;
}
