/******************************************************************************/
/*!
\file   Loading.frag.glsl
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401
\par    Section : A
\date   2025/04/06
\brief
    This file contains the GLSL fragment shader code for rendering the loading
    screen. The shader includes various visual effects such as a loading bar,
    animated lights, and a sky effect to create an engaging loading screen
    experience.

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

uniform float uTime;
uniform float uProgress;

// Useful functions
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

// SDF for basic shapes
float sdBox(vec2 p, vec2 b) {
    vec2 d = abs(p) - b;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

// PBR shading functions
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159265359 * denom * denom;
    
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

// Apply PBR shading to a building
vec3 applyPBR(vec3 baseColor, vec2 uv, vec3 neonColor, float buildingMask) {
    // PBR material properties
    float metallic = 0.8 * buildingMask;  // Buildings are metallic
    float roughness = 0.3 * buildingMask; // Polished surface
    float ao = 0.5 + 0.5 * buildingMask;  // Ambient occlusion
    
    // Lighting setup
    vec3 lightPositions[3] = vec3[](
        vec3(0.3, 0.8, 3.0),  // Main light source
        vec3(0.7, 0.3, 2.0),  // Secondary light
        vec3(-0.5, 0.5, 1.0)  // Fill light
    );
    
    vec3 lightColors[3] = vec3[](
        vec3(1.0, 0.8, 0.9) * 2.0,  // Pink-ish main light
        vec3(0.2, 0.8, 1.0) * 1.5,  // Cyan secondary light
        vec3(0.5, 0.2, 0.7) * 1.0   // Purple fill light
    );
    
    // Animate lights subtly
    lightPositions[0].x += sin(uTime * 0.5) * 0.2;
    lightPositions[1].y += cos(uTime * 0.3) * 0.1;
    
    // Normal calculation - generate simple normal from position with some variation
    vec3 N = normalize(vec3(
        sin(uv.x * 10.0) * 0.1 * buildingMask, 
        cos(uv.y * 8.0) * 0.1 * buildingMask, 
        1.0
    ));
    
    // Viewing direction (toward camera)
    vec3 V = normalize(vec3(0.0, 0.0, 1.0));
    
    // Reflectance at normal incidence (F0)
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, baseColor, metallic);
    
    // Reflectance equation
    vec3 Lo = vec3(0.0);
    
    for(int i = 0; i < 3; ++i) {
        // Calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - vec3(uv * 2.0 - 1.0, 0.0));
        vec3 H = normalize(V + L);
        
        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;
        
        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular = numerator / denominator;
        
        // Add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * baseColor / 3.14159265359 + specular) * lightColors[i] * NdotL;
    }
    
    // Ambient lighting
    vec3 ambient = vec3(0.03) * baseColor * ao;
    
    // Enhanced neon glow effect
    float rim = 1.0 - max(dot(N, V), 0.0);
    rim = pow(rim, 3.0) * buildingMask;
    vec3 neonRim = neonColor * rim * 2.0;
    
    // Combine all lighting components
    vec3 color = ambient + Lo + neonRim;
    
    // Tone mapping (simplified HDR)
    color = color / (color + vec3(1.0));
    
    // Add subtle reflections on the building surface
    float reflection = pow(1.0 - roughness, 3.0) * metallic * buildingMask;
    color += reflection * neonColor * 0.2;
    
    return color;
}

// Simple sky with subtle effects - matches reference better
vec4 skyEffect(vec2 uv, float time) {
    vec3 finalSky = vec3(0.0);
    
    // Very subtle gradient for the top part
    float topGradient = pow(1.0 - uv.y, 3.0) * 0.05;
    finalSky += vec3(0.05, 0.02, 0.1) * topGradient;
    
    // A few sparse stars
    for (int i = 0; i < 10; i++) {
        float randX = random(vec2(float(i), 0.42));
        float randY = random(vec2(float(i), 0.91));
        
        // Only place stars in the top portion
        vec2 starPos = vec2(
            randX,
            randY * 0.2 // Only in top ~20%
        );
        
        // Small star with minimal twinkling
        float starSize = 0.001 + random(vec2(float(i), 0.33)) * 0.001;
        float dist = length(uv - starPos);
        float brightness = smoothstep(starSize, 0.0, dist) * 0.3;
        brightness *= 0.7 + 0.3 * sin(time * 0.5 + randX * 10.0);
        
        finalSky += brightness * vec3(0.8, 0.8, 1.0);
    }
    
    return vec4(finalSky, 1.0);
}

// Function to create a loading bar with neon effect - matching reference image
vec4 loadingBar(vec2 uv, float progress) {
    // Bar positioning and dimensions - make it thinner as in reference
    float barY = 0.425;  // Just above middle
    float barHeight = 0.015;
    float barWidth = 0.7;
    float barX = 0.5 - (barWidth / 2.0);
    
    // Create simple bar without much glow - cleaner like in reference
    // Bar shape and mask
    float barShape = sdBox(vec2(uv.x - 0.5, uv.y - barY), vec2(barWidth/2.0, barHeight));
    float barMask = smoothstep(0.002, 0.0, abs(barShape));
    
    // Calculate progress position
    float progressPos = barX + (barWidth * progress);
    
    // Create t parameter for full bar gradient
    float t = (uv.x - barX) / barWidth;
    
    // Check if we're inside the bar boundaries
    if (uv.x >= barX && uv.x <= barX + barWidth &&
        uv.y >= barY - barHeight && uv.y <= barY + barHeight) {
        
        // Create gradient colors for the entire bar
        vec3 startColor = vec3(0.2, 0.7, 1.0); // Cyan
        vec3 endColor = vec3(1.0, 0.2, 0.7);   // Pink
        vec3 fullGradient = mix(startColor, endColor, t);
        
        // If in the filled portion, use full brightness
        if (uv.x <= progressPos) {
            return vec4(fullGradient, 1.0);
        } 
        // If in unfilled portion, use darker version of the same gradient
        else {
            // Dimmed version of the same color
            return vec4(fullGradient * 0.2, 1.0);
        }
    }
    
    // Outside bar
    return vec4(0.0);
}

// Add the missing gridBuilding function
vec4 gridBuilding(vec2 uv, float posX, float width, float height, vec3 neonColor) {
    // Building dimensions and positioning
    float buildingX = posX;
    float buildingWidth = width;
    float buildingHeight = height;
    
    // Check if we're inside the building - start from bottom of screen
    bool insideBuilding = (uv.x >= buildingX - buildingWidth/2.0) && 
                          (uv.x <= buildingX + buildingWidth/2.0) && 
                          (uv.y >= 0.0) && 
                          (uv.y <= buildingHeight);
    
    // If not inside the building, return transparent
    if (!insideBuilding) {
        return vec4(0.0);
    }
    
    // Building base color - dark with slight variation
    vec3 baseColor = vec3(0.08, 0.05, 0.15);
    
    // Grid pattern for windows
    float windowSizeX = 0.015;
    float windowSizeY = 0.018;
    float windowSpacingX = 0.025;
    float windowSpacingY = 0.03;
    
    // Normalized coordinates within the building - adjusted for bottom start
    vec2 buildingUV = vec2(
        (uv.x - (buildingX - buildingWidth/2.0)) / buildingWidth,
        uv.y / buildingHeight
    );
    
    // Create window grid
    float windowX = mod(buildingUV.x, windowSpacingX);
    float windowY = mod(buildingUV.y, windowSpacingY);
    
    // Window effect with some randomization
    bool isWindow = (windowX < windowSizeX) && (windowY < windowSizeY);
    float windowRandom = random(vec2(
        floor(buildingUV.x / windowSpacingX),
        floor(buildingUV.y / windowSpacingY)
    ));
    
    // Randomize window glow
    float windowGlow = isWindow ? 0.3 + 0.7 * windowRandom : 0.0;
    windowGlow *= 0.5 + 0.5 * sin(uTime * 0.2 + windowRandom * 10.0);
    
    // Apply PBR shading to the building
    vec3 buildingColor = applyPBR(baseColor, buildingUV, neonColor, 1.0);
    
    // Add window glow
    buildingColor += windowGlow * mix(neonColor, vec3(1.0), 0.7);
    
    // Building outline
    float outlineWidth = 0.003;
    bool isOutline = (buildingUV.x < outlineWidth) || 
                     (buildingUV.x > 1.0 - outlineWidth) || 
                     (buildingUV.y < outlineWidth) || 
                     (buildingUV.y > 1.0 - outlineWidth);
    
    // Add neon outline
    if (isOutline) {
        float outlinePulse = 0.7 + 0.3 * sin(uTime * 2.0);
        buildingColor = mix(buildingColor, neonColor, outlinePulse);
    }
    
    return vec4(buildingColor, 1.0);
}

void main() {
    // Base background - dark gradient
    vec3 topColor = vec3(0.02, 0.01, 0.05);
    vec3 bottomColor = vec3(0.15, 0.05, 0.3);
    vec3 backgroundColor = mix(bottomColor, topColor, pow(TexCoord.y, 0.5));
    
    // Apply sky effects at the top
    vec4 sky = skyEffect(TexCoord, uTime);
    backgroundColor += sky.rgb;
    
    // Final color will be built up from here
    vec4 finalColor = vec4(backgroundColor, 1.0);
    
    // Remove the horizon line - buildings should start from bottom
    // Just add a very subtle darkening at the bottom
    float groundDarkening = smoothstep(0.0, 0.2, TexCoord.y);
    finalColor.rgb *= mix(0.7, 1.0, groundDarkening);
    
    // Create the building layout - similar to the reference image
    // Building configuration:
    // 1. Array of rectangular buildings from the bottom with different heights
    // 2. Alternating cyan and pink neon outlines
    
    // Building positions and dimensions - adjusted to be taller
    vec3 buildingData[6] = vec3[](
        // x, width, height (from bottom)
        vec3(0.1, 0.15, 0.7),    // 1st building
        vec3(0.3, 0.15, 0.85),   // 2nd building  
        vec3(0.5, 0.15, 0.9),    // 3rd building - tallest center
        vec3(0.7, 0.15, 0.75),   // 4th building
        vec3(0.9, 0.15, 0.8),    // 5th building
        vec3(-0.05, 0.15, 0.65)  // Additional building slightly off-screen left
    );
    
    // Alternating pink and cyan neon colors
    vec3 neonColors[6] = vec3[](
        vec3(1.0, 0.2, 0.8),  // Pink
        vec3(0.2, 0.8, 1.0),  // Cyan
        vec3(1.0, 0.2, 0.8),  // Pink
        vec3(0.2, 0.8, 1.0),  // Cyan
        vec3(1.0, 0.2, 0.8),  // Pink
        vec3(0.2, 0.8, 1.0)   // Cyan
    );
    
    // Draw buildings
    for (int i = 0; i < 6; i++) {
        vec2 uv = TexCoord;
        float height = buildingData[i].z;
        vec4 building = gridBuilding(uv, buildingData[i].x, buildingData[i].y, height, neonColors[i]);
        finalColor.rgb = mix(finalColor.rgb, building.rgb, building.a);
    }
    
    // Add some floating particles instead of musical notes
    for (int i = 0; i < 15; i++) {
        // Generate pseudo-random position that moves slowly over time
        float randX = random(vec2(float(i), 0.42));
        float randY = random(vec2(float(i), 0.91));
        
        // Animate the particles
        vec2 particlePos = vec2(
            mod(randX + uTime * (0.02 + randX * 0.05), 1.0),
            randY * 0.6 + 0.2 + sin(uTime * (0.5 + randX) + randY * 10.0) * 0.03
        );
        
        // Calculate particle size and opacity
        float size = 0.002 + random(vec2(float(i), 0.33)) * 0.004;
        float particleOpacity = 0.5 + 0.5 * sin(uTime * (0.3 + randX) + randY * 5.0);
        
        // Calculate distance to particle
        float particleDist = length(TexCoord - particlePos);
        
        // Generate color based on position
        vec3 particleColor = mix(
            vec3(0.2, 0.8, 1.0),  // Cyan
            vec3(1.0, 0.3, 0.7),  // Pink
            random(vec2(float(i), 0.5))
        );
        
        // Add the particle with glow
        float glow = smoothstep(size, 0.0, particleDist);
        finalColor.rgb += glow * particleColor * particleOpacity * 0.5;
    }
    
    // Add the loading bar in the center
    vec4 loadingBarEffect = loadingBar(TexCoord, uProgress);
    finalColor.rgb = mix(finalColor.rgb, loadingBarEffect.rgb, loadingBarEffect.a);
    
    // Add background glow to the center area
    vec2 centerPoint = vec2(0.5, 0.5);
    float centerDistance = length(TexCoord - centerPoint);
    vec3 centerGlow = mix(vec3(0.8, 0.2, 0.9), vec3(0.2, 0.8, 1.0), sin(uTime * 0.5) * 0.5 + 0.5);
    float glowIntensity = smoothstep(0.5, 0.0, centerDistance) * 0.2 * (0.5 + 0.5 * sin(uTime));
    finalColor.rgb += centerGlow * glowIntensity;
    
    // Add subtle post-processing
    // 1. Vignette
    float vignette = length(TexCoord - vec2(0.5)) * 0.5;
    finalColor.rgb *= 1.0 - vignette;
    
    // 2. Subtle scanlines
    float scanline = 0.95 + 0.05 * sin(TexCoord.y * 200.0);
    finalColor.rgb *= scanline;
    
    // 3. Add some noise for a more gritty look
    float noise = random(TexCoord + uTime * 0.01) * 0.03;
    finalColor.rgb += noise;
    
    // Output final color
    FragColor = finalColor;
}
)"