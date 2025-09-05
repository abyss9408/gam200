/******************************************************************************/
/*!
\file   Loading.vert.glsl
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401
\par    Section : A
\date   2025/04/06
\brief
    This file contains the GLSL vertex shader code for rendering the loading
    screen. The shader processes vertex positions and texture coordinates,
    passing the texture coordinates to the fragment shader for further processing.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
        
    out vec2 TexCoord;
        
    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)"