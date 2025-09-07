#version 330 core

in vec2 fragUV;
out vec4 fragColor;

uniform vec4 colour;
uniform vec2 u_boundsPos;
uniform vec2 u_boundsSize;
uniform vec2 u_dimensions;
uniform vec2 u_uvmod;
uniform vec2 u_uvoffset;
uniform float u_radius;

uniform mat4 proj;
uniform mat4 view;

uniform sampler2D tex0;

void main()
{
    vec2 fragPos = gl_FragCoord.xy;

    vec2 minCorner = u_boundsPos - u_boundsSize / 2.0;
    vec2 maxCorner = u_boundsPos + u_boundsSize / 2.0;

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
        fragColor = texture(tex0, fragUV * u_uvmod + u_uvoffset) * colour;
        return;
    }

    vec2 diff = pixelPos - corner;
    if (length(diff) > r)
        discard;

    fragColor = texture(tex0, fragUV * u_uvmod + u_uvoffset) * colour;
}