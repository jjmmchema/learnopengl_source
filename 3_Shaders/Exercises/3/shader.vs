#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 pos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); // add the xOffset to the x position of the vertex position
    pos = gl_Position;
}