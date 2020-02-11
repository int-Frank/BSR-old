#version 430

layout (location = 0) in vec2 inPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader

uniform bool u_bool[3];

void main()
{
  gl_Position = vec4(inPos, 0.0, 1.0); // see how we directly give a vec3 to vec4's constructor
  vertexColor = vec4(0.0, 0.0, 0.0, 1.0);
  if (u_bool[0])
    vertexColor[0] = 1.0;
  if (!u_bool[1])
    vertexColor[1] = 1.0;
  if (u_bool[2])
    vertexColor[2] = 1.0;
}