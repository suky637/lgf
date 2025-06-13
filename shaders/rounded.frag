#version 330 core

in vec2 fragUV;
out vec4 fragColor;

uniform vec4 colour;
uniform vec2 u_boundsPos;
uniform vec2 u_boundsSize;
uniform vec2 u_dimensions;
uniform float u_radius;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    vec2 fragPos = gl_FragCoord.xy;

    vec2 minCorner = u_boundsPos;
    vec2 maxCorner = u_boundsPos + u_boundsSize;

    if (fragPos.x < minCorner.x || fragPos.x > maxCorner.x ||
        fragPos.y < minCorner.y || fragPos.y > maxCorner.y) {
        discard;
    }

    vec2 uv = fragUV;
    vec2 pixelPos = uv * u_dimensions;
    float r = u_radius;

    vec2 corner;

    if (pixelPos.x < r && pixelPos.y < r)
        corner = vec2(r, r);
    else if (pixelPos.x > u_dimensions.x - r && pixelPos.y < r)
        corner = vec2(u_dimensions.x - r, r);
    else if (pixelPos.x > u_dimensions.x - r && pixelPos.y > u_dimensions.y - r)
        corner = vec2(u_dimensions.x - r, u_dimensions.y - r);
    else if (pixelPos.x < r && pixelPos.y > u_dimensions.y - r)
        corner = vec2(r, u_dimensions.y - r);
    else {
        fragColor = colour;
        return;
    }

    vec2 diff = pixelPos - corner;
    if (length(diff) > r)
        discard;

    fragColor = colour;
}