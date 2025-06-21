#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColour;

void main()
{    
    float alpha = texture(text, TexCoords).r;
    color = vec4(textColour.rgb, textColour.a * alpha);
}