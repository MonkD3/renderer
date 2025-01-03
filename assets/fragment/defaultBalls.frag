#version 460 core

in vec4 gCenter; // Center of the ball
in vec4 gPos;    // Position of the fragment
in vec3 gCol;    // Color of the fragment
in float gRad;   // Radius of fragment

out vec4 FragColor; // Final color of fragment

uniform mat4 mvp;
uniform mat4 imvp;

void main() {
    vec4 d = gCenter - gPos;
    if (dot(d, d) > gRad*gRad) discard;

    // Light comes from the top left and goes to the bottom right
    vec4 light = mvp*vec4(-1.0f, -1.0f, 0.0f, 0.0f);
    vec3 lightNorm = normalize(light.xyz);
    d = normalize(d);

    float brightness = clamp(dot(d.xyz, lightNorm), 0.1f, 1.0f);
    FragColor = vec4(brightness * gCol, 1.0f);
}
