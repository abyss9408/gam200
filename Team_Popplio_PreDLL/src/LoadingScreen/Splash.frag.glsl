/******************************************************************************/
/*!
\file   Splash.frag.glsl
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401
\par    Section : A
\date   2025/04/06
\brief
    This file contains the GLSL fragment shader code for rendering the splash
    screen with aspect ratio adjustments and fade effects. The shader ensures
    that the splash screen image maintains its aspect ratio and applies a fade
    effect based on the provided alpha value.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
R"(
    #version 460 core
    in vec2 TexCoord;
    out vec4 FragColor;
    
    uniform sampler2D splashTexture;
    uniform float fadeAlpha;     // Alpha value for fading
    uniform vec2 textureSize;    // Size of the texture (width, height)
    uniform vec2 screenSize;     // Size of the screen (width, height)
    
    void main()
    {
        // Calculate aspect ratios
        float textureAspect = textureSize.x / textureSize.y;
        float screenAspect = screenSize.x / screenSize.y;
        
        // Calculate the texture coordinates that maintain aspect ratio
        vec2 adjustedTexCoord = TexCoord;
        
        if (screenAspect > textureAspect) {
            // Screen is wider than texture
            float scale = textureAspect / screenAspect;
            adjustedTexCoord.x = 0.5 + (TexCoord.x - 0.5) / scale;
            
            // If the adjusted coordinates are outside [0,1], draw transparent
            if (adjustedTexCoord.x < 0.0 || adjustedTexCoord.x > 1.0) {
                FragColor = vec4(0.0, 0.0, 0.0, 0.0);
                return;
            }
        } else {
            // Screen is taller than texture
            float scale = screenAspect / textureAspect;
            adjustedTexCoord.y = 0.5 + (TexCoord.y - 0.5) / scale;
            
            // If the adjusted coordinates are outside [0,1], draw transparent
            if (adjustedTexCoord.y < 0.0 || adjustedTexCoord.y > 1.0) {
                FragColor = vec4(0.0, 0.0, 0.0, 0.0);
                return;
            }
        }
        
        vec4 texColor = texture(splashTexture, adjustedTexCoord);
        FragColor = vec4(texColor.rgb, texColor.a * fadeAlpha);
    }
)"