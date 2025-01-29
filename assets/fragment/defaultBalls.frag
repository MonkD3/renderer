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

in vec4 vPos;    // Position of the fragment
in vec3 vCol;    // Color of the fragment
in vec3 vNormal; // Normal to the vertex
in vec3 vLight;  // Where is the light
in float vRad;   // Radius of fragment

out vec4 FragColor; // Final color of fragment

void main() {
    vec3 d = vec3(vPos.xy, 0.0f);
    float z = vRad*vRad - dot(d, d);
    if (0 > z) {
        discard;
    }
    d.z = sqrt(z); 
    d /= vRad;
    vec3 light = normalize(vLight);
    float brightness = dot(d, light);

    vec4 col = vec4(vCol, 1.0f);
    if (useCmap) {
        col = texture(cmap, (vCol.x - cmapRange.x)/(cmapRange.y - cmapRange.x));
    }

    FragColor = clamp(brightness, 0.0f, 1.0f) * col;
}
