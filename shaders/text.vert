#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;
uniform float scale;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 2.8, 1.0);
    TexCoords = vertex.zw;
}  