#version 460 core
in vec3 ourColor;
in vec2 TexCoord;
in float alpha;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform bool useTexture;

void main()
{
    vec4 textureColor = texture(ourTexture, TexCoord);

    if (useTexture)
    {
		vec4 textureColor = texture(ourTexture, TexCoord);
        FragColor = textureColor * vec4(ourColor, alpha);
	}
    else
    {
        FragColor = vec4(ourColor, alpha);
    }
}