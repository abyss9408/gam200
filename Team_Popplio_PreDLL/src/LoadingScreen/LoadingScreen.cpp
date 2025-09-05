/******************************************************************************/
/*!
\file   LoadingScreen.cpp
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401
\par    Section : A
\date   2025/04/06
\brief
    This file contains the implementation of the LoadingScreen class, which
    provides a minimal loading screen implementation focused on reliable rendering.
    It includes functionality for displaying progress, handling splash screens,
    and managing OpenGL resources for rendering.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "LoadingScreen.h"
#include "../Logging/Logger.h"
#include "../AssetStore/AssetBatchLoader.h"

namespace Popplio
{
    // Basic vertex shader with position and texture coordinates
    const std::string vertexShader = 
    { 
        #include "Loading.vert.glsl"
    };

    // Enhanced background/scene fragment shader with multiple visual elements
    const std::string sceneFragmentShader = 
    {
        #include "Loading.frag.glsl"
    };

    // Simple texture fragment shader for splash screen
    const std::string splashFragmentShader =
    {
        #include "Splash.frag.glsl"
    };

    LoadingScreen::LoadingScreen(GLFWwindow* window)
        : window(window),
        loadingShaderProgram(0),
        splashShaderProgram(0),
        quadVAO(0), quadVBO(0), quadEBO(0),
        progress(0.0f), time(0.0f),
        state(ScreenState::LOADING),
        hasFirstSplash(false),
        hasSecondSplash(false),
        fadeInDuration(0.5f),
        fadeOutDuration(0.5f)
    {
    }

    LoadingScreen::~LoadingScreen()
    {
        Cleanup();
    }

    bool LoadingScreen::Initialize()
    {
        startTime = std::chrono::high_resolution_clock::now();

        // Create shader program
        loadingShaderProgram = CreateShaderProgram(vertexShader.c_str(), sceneFragmentShader.c_str());
        if (!loadingShaderProgram) {
            Logger::Error("Failed to create shader program for loading screen");
            return false;
        }

        // Create shader program for splash screen
        splashShaderProgram = CreateShaderProgram(vertexShader.c_str(), splashFragmentShader.c_str());
        if (!splashShaderProgram) {
            Logger::Error("Failed to create shader program for splash screen");
            return false;
        }

        // Create quad geometry for rendering the entire scene
        float quadVertices[] = {
            // positions (x, y, z)         // texture coords
            -1.0f, -1.0f, 0.0f,           0.0f, 0.0f,
             1.0f, -1.0f, 0.0f,           1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,           1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f,           0.0f, 1.0f
        };

        unsigned int quadIndices[] = {
            0, 1, 2,
            2, 3, 0
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glGenBuffers(1, &quadEBO);

        glBindVertexArray(quadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Immediately render once to avoid white screen
        Render();

        return true;
    }

    GLuint LoadingScreen::CreateShaderProgram(const char* vertexSource, const char* fragmentSource)
    {
        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShader, 1, &vertexSource, NULL);
        glCompileShader(vertShader);

        // Check for shader compile errors
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
            Logger::Error("Vertex shader compilation failed: " + std::string(infoLog));
            return 0;
        }

        // Fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        // Check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            Logger::Error("Fragment shader compilation failed: " + std::string(infoLog));
            return 0;
        }

        // Link shaders
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            Logger::Error("Shader program linking failed: " + std::string(infoLog));
            return 0;
        }

        // Shaders are linked, they can be deleted
        glDeleteShader(vertShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }

    void LoadingScreen::Update(float newProgress)
    {
        progress = std::max(0.0f, std::min(1.0f, newProgress));

        // Calculate elapsed time for animations
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - startTime;
        time = elapsed.count();

        // Update window title with loading progress
        if (state == ScreenState::LOADING) {
            std::string title = "Loading... " + std::to_string(static_cast<int>(progress * 100)) + "%";
            glfwSetWindowTitle(window, title.c_str());
        }

        // Check if loading is complete and we should transition to first splash screen
        if (progress >= 1.0f && state == ScreenState::LOADING) {
            if (hasFirstSplash && !CheckForSkip()) {
                state = ScreenState::SPLASH_FIRST;
                splashStartTime = std::chrono::high_resolution_clock::now();
                glfwSetWindowTitle(window, "Game Loading"); // Update window title
            }
            else if (hasSecondSplash && !CheckForSkip()) {
                state = ScreenState::SPLASH_SECOND;
                splashStartTime = std::chrono::high_resolution_clock::now();
                glfwSetWindowTitle(window, "Game Loading"); // Update window title
            }
            else {
                state = ScreenState::COMPLETED;
            }
        }
    }

    void LoadingScreen::Render()
    {
        // Check if we need to transition from splash to completed
        if (state == ScreenState::SPLASH_FIRST) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsed = currentTime - splashStartTime;

            if (elapsed.count() >= firstSplash.duration || CheckForSkip()) {
                if (hasSecondSplash) {
                    state = ScreenState::SPLASH_SECOND;
                    splashStartTime = std::chrono::high_resolution_clock::now();
                }
                else {
                    state = ScreenState::COMPLETED;
                    return;
                }
            }
        }
        else if (state == ScreenState::SPLASH_SECOND) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsed = currentTime - splashStartTime;

            if (elapsed.count() >= secondSplash.duration || CheckForSkip()) {
                state = ScreenState::COMPLETED;
                return; // Done showing splash screens
            }
        }


        // Get window dimensions
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Render appropriate screen based on state
        if (state == ScreenState::LOADING) {
            // Calculate elapsed time for animations
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsed = currentTime - startTime;
            time = elapsed.count();

            // Use shader and set uniforms
            glUseProgram(loadingShaderProgram);
            glUniform1f(glGetUniformLocation(loadingShaderProgram, "uTime"), time);
            glUniform1f(glGetUniformLocation(loadingShaderProgram, "uProgress"), progress);

            // Draw quad that covers the entire screen
            glBindVertexArray(quadVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        else if (state == ScreenState::SPLASH_FIRST && hasFirstSplash) {
            RenderSplash(firstSplash);
		}
		else if (state == ScreenState::SPLASH_SECOND && hasSecondSplash) {
			RenderSplash(secondSplash);
		}

        // Reset state
        glBindVertexArray(0);
        glUseProgram(0);

        // Swap buffers
        glfwSwapBuffers(window);
    }

    const unsigned int loadCycle = 16; // 16ms = ~60 FPS

    void LoadingScreen::LoadAssets(const std::function<void(std::function<void(float)>)>& assetLoadingFunction)
    {
        // Set loading state
        state = ScreenState::LOADING;

        // Create a proxy progress tracker that updates the loading screen
        auto progressProxy = [this](float progress) {
            // Update loading screen based on current progress
            this->Update(progress);
            this->Render();

            // Process events to keep UI responsive
            glfwPollEvents();

            // Small delay to avoid maxing CPU and allow UI updates
            std::this_thread::sleep_for(std::chrono::milliseconds(loadCycle)); 
            //std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        };

        // Initial update at 0%
        progressProxy(0.0f);

        // Call the provided asset loading function with our progress tracking proxy
        assetLoadingFunction(progressProxy);

        // Final update at 100%
        progressProxy(1.0f);

        // Asset loading complete, determine next state
        if (hasFirstSplash) {
            state = ScreenState::SPLASH_FIRST;
            splashStartTime = std::chrono::high_resolution_clock::now();
            glfwSetWindowTitle(window, "Game Loading");
        }
        else if (hasSecondSplash) {
            state = ScreenState::SPLASH_SECOND;
            splashStartTime = std::chrono::high_resolution_clock::now();
            glfwSetWindowTitle(window, "Game Loading");
        }
        else {
            // If no splash textures, mark as completed
            state = ScreenState::COMPLETED;
        }
    }

    bool LoadingScreen::IsComplete() const
    {
        return state == ScreenState::COMPLETED;
    }

    void LoadingScreen::SetFirstSplashTexture(const std::string& textureName, float duration)
    {
        firstSplash.textureName = textureName;
		firstSplash.duration = duration;

        // Try to get the texture from AssetStore
        firstSplash.textureID = AssetStore::GetTexture(textureName);

        if (firstSplash.textureID != 0) {
            hasFirstSplash = true;

            // Get texture dimensions
            firstSplash.dimensions = AssetStore::GetTextureSize(textureName);

            Logger::Info("Splash screen texture loaded: " + textureName +
                " (" + std::to_string(firstSplash.dimensions.first) + "x" +
                std::to_string(firstSplash.dimensions.second) + ")", LogDestination::WINDOWS_CONSOLE);
        }
        else {
            hasFirstSplash = false;
            Logger::Warning("Failed to find splash screen texture: " + textureName, LogDestination::WINDOWS_CONSOLE);
        }
    }

    void LoadingScreen::SetSecondSplashTexture(const std::string& textureName, float duration)
    {
		secondSplash.textureName = textureName;
		secondSplash.duration = duration;

		// Try to get the texture from AssetStore
		secondSplash.textureID = AssetStore::GetTexture(textureName);

        if (secondSplash.textureID != 0) {
			hasSecondSplash = true;

			// Get texture dimensions
			secondSplash.dimensions = AssetStore::GetTextureSize(textureName);

            Logger::Info("Splash screen texture loaded: " + textureName +
                " (" + std::to_string(secondSplash.dimensions.first) + "x" +
                std::to_string(secondSplash.dimensions.second) + ")", LogDestination::WINDOWS_CONSOLE);
        }
        else {
            hasSecondSplash = false;
            Logger::Warning("Failed to find splash screen texture: " + textureName, LogDestination::WINDOWS_CONSOLE);
        }
    }

    void LoadingScreen::Cleanup()
    {
        // Delete OpenGL resources
        if (loadingShaderProgram)
            glDeleteProgram(loadingShaderProgram);

        if (splashShaderProgram)
            glDeleteProgram(splashShaderProgram);

        if (quadVAO)
            glDeleteVertexArrays(1, &quadVAO);

        if (quadVBO)
            glDeleteBuffers(1, &quadVBO);

        if (quadEBO)
            glDeleteBuffers(1, &quadEBO);
    }

    void LoadingScreen::RenderSplash(const SplashData& splashData)
    {
        // Calculate fade effect (fade in then fade out)
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - splashStartTime;
        float fadeTime = elapsed.count();

        float fadeAlpha;
        float holdDuration = splashData.duration - (fadeInDuration + fadeOutDuration);  // Time to hold at full opacity

        if (fadeTime < fadeInDuration) {
            // Fade in
            fadeAlpha = fadeTime / fadeInDuration;
        }
        else if (fadeTime < fadeInDuration + holdDuration) {
            // Hold at full opacity
            fadeAlpha = 1.0f;
        }
        else {
            // Fade out
            fadeAlpha = 1.0f - ((fadeTime - fadeInDuration - holdDuration) / fadeOutDuration);
        }

        fadeAlpha = std::max(0.0f, std::min(1.0f, fadeAlpha)); // Clamp between 0 and 1

        // Get window dimensions
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // Use splash shader program
        glUseProgram(splashShaderProgram);

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, splashData.textureID);
        glUniform1i(glGetUniformLocation(splashShaderProgram, "splashTexture"), 0);

        // Set fade uniform
        glUniform1f(glGetUniformLocation(splashShaderProgram, "fadeAlpha"), fadeAlpha);

        // Set texture and screen size for aspect ratio calculation
        glUniform2f(glGetUniformLocation(splashShaderProgram, "textureSize"),
            static_cast<float>(splashData.dimensions.first),
            static_cast<float>(splashData.dimensions.second));
        glUniform2f(glGetUniformLocation(splashShaderProgram, "screenSize"),
            static_cast<float>(width),
            static_cast<float>(height));

        // Draw quad
        glBindVertexArray(quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    bool LoadingScreen::CheckForSkip()
    {
        // Check if spacebar is pressed to skip splash screens
        if (InputSystem::IsKeyTriggered(GLFW_KEY_SPACE))
        {
            Logger::Info("Splash screen skipped with spacebar");
            return true;
        }
        return false;
    }
}