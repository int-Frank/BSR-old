#version 430 core
out vec4 FragColor;
  
uniform bool u_bool;
uniform bool u_bool_fs;
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

void main()
{
  FragColor = vertexColor;
  if (u_bool)
  {
    FragColor[1] = 1.0;
  }
  if (u_bool_fs)
  {
    FragColor[2] = 1.0;
  }
} 