#version 460 core
layout (location = 0) in vec3 iPos; 
layout (location = 1) in vec3 vNormal;

uniform mat4 mvp = mat4(1.0f);
uniform mat4 imvp = mat4(1.0f);

out vec3 fNormal;

void main(){
    gl_Position = mvp*vec4(iPos, 1.0f);
    fNormal = normalize((transpose(imvp)*vec4(vNormal, 1.0f)).xyz);
}
