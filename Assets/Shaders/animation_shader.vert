#version 460 core
layout (location = 0) in vec2 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord;

uniform mat3 model;     // Model transformation matrix
uniform mat3 view;      // View transformation matrix for camera translation
uniform mat3 projection; // Projection matrix for zoom (and perspective/orthographic projection)

out vec3 oColor; // output a color to the fragment shader
out vec2 oTexCoord;

void main()
{
    vec3 ndc_pos = projection * view * model * vec3(aPos, 1.0); // Apply transformations to the vertex position
    gl_Position = vec4(ndc_pos, 1.0);
     // Combine projection, view, and model matrices to transform the vertex position
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
    oColor = aColor; // set ourColor to the input color we got from the vertex data
    oTexCoord = aTexCoord;
}