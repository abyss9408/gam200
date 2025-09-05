/******************************************************************************/
/*!
\file   LoadingScreen.h
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401
\par    Section : A
\date   2025/04/06
\brief
    This file defines the LoadingScreen class, which provides a simplified
    loading screen implementation focused on reliable rendering. It includes
    functionality for displaying progress, handling splash screens, and managing
    OpenGL resources for rendering.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <string>

namespace Popplio
{
    class LoadingScreen
    {
    public:
        // Enum to track screen state
        enum class ScreenState {
            LOADING,        // Initial loading screen with progress bar
            SPLASH_FIRST,   // First splash screen (digipen logo)
            SPLASH_SECOND,  // Second splash screen (game logo)
            COMPLETED       // Loading process is complete
        };

        // Structure to hold splash screen data
        struct SplashData {
            std::string textureName;
            unsigned int textureID;
            std::pair<int, int> dimensions;
            float duration;  // Duration in seconds to show this splash

            SplashData(const std::string& name = "", float dur = 3.0f)
                : textureName(name), textureID(0), dimensions({ 0, 0 }), duration(dur) {
            }
        };

        LoadingScreen(GLFWwindow* window);
        ~LoadingScreen();

        // Initialize the loading screen
        bool Initialize();

        // Update the loading progress (0.0f to 1.0f)
        void Update(float progress);

        // Render the loading screen
        void Render();

        // Load assets with progress reporting
        void LoadAssets(const std::function<void(std::function<void(float)>)>& assetLoadingFunction);

        // Check if loading is complete
        bool IsComplete() const;

        // Set the first splash screen texture
        void SetFirstSplashTexture(const std::string & textureName, float duration = 3.0f);

        // Set the second splash screen texture
        void SetSecondSplashTexture(const std::string& textureName, float duration = 3.0f);

        /**
        * \brief Checks if the user wants to skip the current splash screen
        * \return True if the splash screen should be skipped
        */
        bool CheckForSkip();

    private:
        // Helper function to create a shader program
        GLuint CreateShaderProgram(const char* vertexSource, const char* fragmentSource);

        // Clean up resources
        void Cleanup();

        // Render the splash screen
        void RenderSplash(const SplashData& splashData);

        // Window reference
        GLFWwindow* window;

        // Shader program and quad rendering elements
        GLuint loadingShaderProgram;
        GLuint splashShaderProgram;
        GLuint quadVAO, quadVBO, quadEBO;

        // Splash screen textures
		SplashData firstSplash;
		SplashData secondSplash;
		bool hasFirstSplash;
		bool hasSecondSplash;

        // Loading state
        float progress;
        float time;
        ScreenState state;

        // Timing for animations
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> splashStartTime;

        // Fade durations
        float fadeInDuration; // Tie to fade in (seconds)
		float fadeOutDuration; // Tie to fade out (seconds)
    };
}