#version 460 core
layout (location = 0) in vec2 aPos;   // the position variable has attribute position 0

uniform mat3 model;
uniform mat3 view;
uniform mat3 projection;

void main()
{
    vec3 pos = projection * view * model * vec3(aPos, 1.0);
    gl_Position = vec4(pos.xy, 0.0, 1.0);
}