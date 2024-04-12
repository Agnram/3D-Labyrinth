#version 330 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec3 aV;


out vec3 FragPos;
out vec3 Normal;
//out vec3 viewPos;

uniform mat4 MVP;
void main()
{
    FragPos = aPos;
    Normal =  aNormal; 
    //viewPos = aV;
  
    gl_Position = MVP*vec4(aPos, 1.0);
}