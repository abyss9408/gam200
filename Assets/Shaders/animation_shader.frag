#version 460 core
out vec4 FragColor; 

in vec3 oColor;
in vec2 oTexCoord;

uniform sampler2D ourTexture;
uniform vec3 uColor;    // uniform color
uniform float uAlpha;   // uniform alpha value

// Animation specific uniforms
uniform vec4 u_TextureCoords;

void main()
{
    // Interpolating texture coordinates
    vec2 animTexCoords = mix(u_TextureCoords.xy, u_TextureCoords.zw, oTexCoord);

    // Sample using the coordinates
    vec4 color = texture(ourTexture, animTexCoords);

    // Use the sampled color with uColor, oColor, and uAlpha
    FragColor = vec4(uColor * oColor, uAlpha) * color;
}
