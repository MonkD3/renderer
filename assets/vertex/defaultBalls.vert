#version 460 core
layout (location = 0) in vec4 iPos; 
layout (location = 1) in vec3 iCol;
layout (location = 2) in float iRad;
layout (location = 3) in vec4 iNormal;
layout (location = 4) in vec4 iInstancePos;

layout (std140) uniform worldBlock {
    mat4 view;
    mat4 proj;
    mat4 iview;
    vec4 light;
    vec4 scalings;
    vec4 clipplane;
    bvec4 worldOpts;
};

uniform mat4 model = mat4(1.0f);

out vec3 vCol;
out vec3 vLight;
out vec3 vNormal;
out vec4 vPos;
out float gl_ClipDistance[1];
out float vRad;

void main(){
    vCol = iCol;
    vRad = iRad;
    vPos = iRad * iInstancePos;
    gl_ClipDistance[0] = dot(iPos, clipplane);
    
    vec4 scaledInstancePos = iInstancePos;
    scaledInstancePos.x /= scalings.x;
    scaledInstancePos.w = 0.0f;
    gl_Position = (scalings.y*iRad)*scaledInstancePos + proj*view*model*iPos;
    
    // We use lighting 
    if (worldOpts[0]) {
        // Only normalize if we use ambiant lighting
        if (!worldOpts[1]) vLight = normalize(light.xyz);
        else vLight = (view * light).xyz;

        vNormal = (transpose(inverse(view * model)) * iNormal).xyz;
        vNormal = normalize(vNormal);
    }
    // If we do not use lighting, set twice the same vector 
    // to give a brightness of 1 in the fragment shader
    else {
        vLight = vec3(0.0f, 0.0f, 1.0f);
        vNormal = vec3(0.0f, 0.0f, 1.0f);
    }
}
