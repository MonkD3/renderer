#version 460 core
out vec4 FragColor;
  
in vec3 gCol;

void main() {
    FragColor = vec4(gCol, 1.0f);
}
