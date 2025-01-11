#version 460 core
layout (location = 0) in vec4 iPos; 
layout (location = 1) in vec3 iCol;
layout (location = 2) in vec4 iNormal;

layout (std140) uniform worldBlock {
    mat4 view;
    mat4 proj;
    mat4 iview;
    vec4 light;
    vec4 scalings;
    bvec4 worldOpts;
};

uniform mat4 model = mat4(1.0f);

out vec3 vCol;
out vec3 vLight;
out vec3 vNormal;
out vec4 vPos;

void main(){
    vPos = view * model * iPos;
    gl_Position = proj * vPos;
    vCol = iCol;
    
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
