#version 430

layout (location = 0) in vec2 inPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader

struct MyStruct
{
  bool myBool[3];
};

uniform MyStruct u_myStruct;
uniform float u_float;
void main()
{
  gl_Position = vec4(inPos, 0.0, 1.0); // see how we directly give a vec3 to vec4's constructor
  vertexColor = vec4(0.0, 0.0, 0.0, 1.0);
  if (u_myStruct.myBool[0])
    vertexColor[0] = 1.0;
  if (!u_myStruct.myBool[1])
    vertexColor[1] = 1.0;
  if (u_myStruct.myBool[2])
    vertexColor[2] = 1.0;
  if (u_float != 3.0)
    vertexColor[2] = 0.0;
}