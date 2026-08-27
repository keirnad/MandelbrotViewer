#version 460 core

layout (location = 0) in vec3 pos;

out vec4 TexCoord;
 
void main()
{
    gl_Position = vec4(pos.xyz, 1.0);
    TexCoord = gl_Position;
}