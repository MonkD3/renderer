#version 460 core

layout (std140) uniform worldBlock {
    mat4 view;
    mat4 proj;
    mat4 iview;
    vec4 light;
    vec4 scalings;
    bvec4 worldOpts;
};

uniform mat4 model = mat4(1.0f);
uniform bool useCmap = false;
uniform vec2 cmapRange = vec2(-1.0f, 1.0f);
uniform sampler1D cmap;

in vec3 vCol;
in vec3 vLight;
in vec3 vNormal;
in vec4 vPos;

out vec4 FragColor;

void main(){

    float brightness = 1.0f;
    
    // We use ambiant lighting 
    if (worldOpts[0] && !worldOpts[1]) {
        brightness = dot(vLight, vNormal);
    }

    // We use pointwise lighting
    if (worldOpts[0] && worldOpts[1]) {
        vec3 lightDir = normalize(vLight - vPos.xyz);
        brightness = dot(lightDir, vNormal);
    }

    vec4 col = vec4(vCol, 1.0f);
    if (useCmap) {
        col = texture(cmap, (vCol.x - cmapRange.x)/(cmapRange.y - cmapRange.x));
    }

    FragColor = clamp(brightness, 0.5f, 1.0f) * col;
}
