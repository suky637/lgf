#version 330 core

layout(location=0) in vec2 aPos; // positions -1 to 1
layout(location = 1) in vec2 aUV;

out vec2 fragUV;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    fragUV = aUV;
    gl_Position = proj * view * model * vec4(aPos, 0.0, 1.0);
}
