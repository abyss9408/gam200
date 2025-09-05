#version 460 core
out vec4 FragColor;

in vec3  oColor;
in vec2  oTexCoord;

//  Basic uniforms
uniform sampler2D ourTexture;
uniform vec3  uColor;
uniform float uAlpha;

// 9-slice uniforms
uniform bool useNineSlice;
uniform vec2 spriteSize;
uniform vec2 textureSize;
uniform vec4 borderSize;

// Assumes:
//   borderSize = vec4(leftPixels, rightPixels, topPixels, bottomPixels)
//   spriteSize = vec2(finalWidthInPixels, finalHeightInPixels)  // how big the sprite will appear on screen
//   textureSize = vec2(srcTextureWidth, srcTextureHeight)
//   uv in [0..1]

//
// Given a single UV coordinate (0..1), sliceCoord() figures out which region
// we are in (left corner, right corner, or the middle stretch) and then
// maps that coordinate back into [0..1] so that corners are preserved.
//
float sliceCoord(
    float uvCoord,        // 0..1
    float finalSize,      // total size in *screen/pixel* space for this axis
    float borderA,        // e.g. left border in pixels
    float borderB,        // e.g. right border in pixels
    float texSize         // texture width (or height) in pixels
) {
    // Convert [0..1] uv into pixel coordinates of the final drawn quad
    float pixelPos = uvCoord * finalSize;

    // ---------------------------
    //  Left (or top) corner
    // ---------------------------
    // If we are in the corner region [0..borderA],
    // then we do a simple proportion of that sub-range in the original texture.
    if (pixelPos < borderA) {
        // Map pixelPos from [0..borderA] to [0..(borderA/texSize)] in uv
        return (pixelPos / borderA) * (borderA / texSize);
    }

    // ---------------------------
    //  Right (or bottom) corner
    // ---------------------------
    float rightStart = finalSize - borderB;  // start of right/bottom corner in final coordinates
    if (pixelPos > rightStart) {
        float distIntoCorner = pixelPos - rightStart;
        // The uv region for the right (or bottom) corner is from
        // [ (texSize - borderB)/texSize .. 1.0 ] in the texture
        float cornerUvStart  = (texSize - borderB) / texSize;
        // scale distIntoCorner within [0..borderB], then offset by cornerUvStart
        return cornerUvStart + (distIntoCorner / borderB) * (borderB / texSize);
    }

    // ---------------------------
    //  Middle (stretch) region
    // ---------------------------
    float centerStart  = borderA;
    float centerWidth  = finalSize - borderA - borderB;  // how many final pixels are “middle”
    float distIntoMid  = pixelPos - centerStart;
    // In texture UVs, the middle region is from
    // [ borderA/texSize .. 1.0 - (borderB/texSize) ]
    float uvCenterStart = borderA / texSize;
    float uvCenterSize  = 1.0 - (borderB / texSize) - uvCenterStart;

    return uvCenterStart + (distIntoMid / centerWidth) * uvCenterSize;
}

//
// 9-slice helper that does x and y independently:
//
vec2 uv9slice(vec2 uv, vec2 finalSize, vec4 borderSize, vec2 textureSize)
{
    float newU = sliceCoord(uv.x, finalSize.x, borderSize.x, borderSize.y, textureSize.x);
    float newV = sliceCoord(uv.y, finalSize.y, borderSize.z, borderSize.w, textureSize.y);
    return vec2(newU, newV);
}

void main()
{
    vec2 uv = oTexCoord;

    if (useNineSlice)
    {
        uv = uv9slice(uv, spriteSize, borderSize, textureSize);
    }

    vec4 sampled = texture(ourTexture, uv);
    FragColor = vec4(uColor, uAlpha) * vec4(oColor, uAlpha) * sampled;
}
