#version 460 core
out vec4 FragColor;

uniform mat4 mvp;
uniform float aspectRatio;
uniform float zoom;

in vec4 fPos;
in float fRad;
in vec3 fLight;

void main() {
    vec3 d = vec3(fPos.xy, 0.0f);
    float z = fRad*fRad - dot(d, d);
    if (0 > z) {
        discard;
    }
    d.z = sqrt(z); 
    // Instead of dividig d by the radius, we divide the 
    // inner product d^T light, this saves us a bit of compute
    // power
    vec3 light = normalize(fLight);
    float brightness = - dot(d, light) / fRad;

    FragColor = brightness * vec4(1.0f, 1.0f, 1.0f, 1.0f);
} 
