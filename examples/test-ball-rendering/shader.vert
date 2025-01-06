#version 460 core
layout (location = 0) in vec4 vInstancedPos; 
layout (location = 1) in vec4 vPos;
layout (location = 2) in float vRad;
layout (location = 3) in vec3 vLight;

uniform mat4 mvp;
uniform float aspectRatio;
uniform float zoom;

out vec4 fPos;
out float fRad;
out vec3 fLight;

void main(){
    fRad = vRad;
    fPos = vRad * vInstancedPos;
    fLight = vLight;

    vec4 scaledInstancedPos = vInstancedPos;
    scaledInstancedPos.x /= aspectRatio;
    gl_Position = (zoom*vRad)*scaledInstancedPos + mvp*vPos;
}
