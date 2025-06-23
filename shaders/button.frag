#version 330 core

in vec2 fragUV;
out vec4 fragColor;

uniform vec4 colour;
uniform vec2 u_boundsPos;
uniform vec2 u_boundsSize;
uniform vec2 u_dimensions;
uniform float u_radius;

uniform vec2 u_mousePosition;
uniform vec2 u_bounds_size;
uniform vec2 u_bounds_pos;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    vec4 out_colour = colour;

    if (u_mousePosition.x > u_bounds_pos.x - u_bounds_size.x / 2.0 && u_mousePosition.x < u_bounds_pos.x + u_bounds_size.x / 2.0 && u_mousePosition.y > u_bounds_pos.y - u_bounds_size.y / 2.0 && u_mousePosition.y < u_bounds_pos.y + u_bounds_size.y / 2.0) {
        if ((out_colour.r + out_colour.g + out_colour.b) / 3.0 > 0.5) {
            out_colour = vec4(max(out_colour.r - 0.04, 0.0), max(out_colour.g - 0.04, 0.0), max(out_colour.b - 0.04, 0.0), out_colour.a);
        } else {
            out_colour = vec4(min(out_colour.r + 0.04, 1.0), min(out_colour.g + 0.04, 1.0), min(out_colour.b + 0.04, 1.0), out_colour.a);
        }
    }

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
        fragColor = out_colour;
        return;
    }

    vec2 diff = pixelPos - corner;
    if (length(diff) > r)
        discard;

    fragColor = out_colour;
}