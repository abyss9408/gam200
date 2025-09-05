#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat3 aInstanceMatrix;
layout (location = 6) in vec3 aInstanceColor;    // New: Instance-specific color
layout (location = 7) in float aInstanceAlpha;   // Instance alpha value
layout (location = 8) in vec4 aInstanceUVRect;   // Instance UV rectangle

out vec3 ourColor;
out vec2 TexCoord;
out float alpha;

uniform mat3 view;
uniform mat3 projection;

void main()
{
    vec3 pos = projection * view * aInstanceMatrix * vec3(aPos, 1.0);
    gl_Position = vec4(pos.xy, 0.0, 1.0);
    ourColor = aColor * aInstanceColor;
    TexCoord = mix(aInstanceUVRect.xy, aInstanceUVRect.zw, aTexCoord);
    alpha = aInstanceAlpha;
}