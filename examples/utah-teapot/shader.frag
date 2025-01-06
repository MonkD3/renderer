#version 460 core
out vec4 FragColor;

in vec3 fNormal;

void main() {
    vec3 light = vec3(1, 1, 1);
    light = normalize(light);

    float brightness = clamp(-dot(fNormal, light), 0.1f, 1.0f);

    FragColor = brightness * vec4(1.0f, 1.0f, 1.0f, 1.0f);
} 
