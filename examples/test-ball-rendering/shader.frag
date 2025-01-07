#version 460 core
out vec4 FragColor;

uniform mat4 mvp;
uniform float aspectRatio;
uniform float zoom;

in vec4 fPos;
flat in float fRad;
in vec3 fLight;

void main() {
    vec3 d = vec3(fPos.xy, 0.0f);
    float z = fRad*fRad - dot(d, d);
    if (0 > z) {
        discard;
    }
    d.z = sqrt(z); 
    d /= fRad;
    vec3 light = normalize(fLight);
    float brightness = clamp(-dot(d, light), 0.0f, 1.0f);

    FragColor = brightness * vec4(1.0f, 1.0f, 1.0f, 1.0f);
} 
