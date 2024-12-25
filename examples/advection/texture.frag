#version 460 core
out vec4 FragColor;
  
in float TexCoord;

uniform sampler1D cmap;

void main()
{
    FragColor = texture(cmap, TexCoord);
}
