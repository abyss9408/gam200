/******************************************************************************/
/*!
\file InputSystem.cpp
\author Val Tay Yun Ying				 (tay.v) (80%)
\co-author: ANG Bryan Wei Ze			 (bryanweize.ang) (15%)
\co-author: NG Jun Heng Shawn			 (n.junhengshawn) (5%)
\date 03-14-2025


All content � 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/
#include <pch.h>
#include "InputSystem.h"

namespace Popplio
{
	std::unordered_map<int, InputSystem::KeyState> InputSystem::keyStates;
	InputSystem::MouseState InputSystem::mouseState;
	bool InputSystem::initialized = false;
	double InputSystem::lastFrameTime = 0.0;
	std::queue<double> InputSystem::inputTimestamps;

	std::unordered_map<int, bool> InputSystem::keysToUpdate;
	std::unordered_map<int, bool> InputSystem::mouseButtonsToUpdate;

	void InputSystem::Init(GLFWwindow* window)
	{
		if (initialized)
		{
			Logger::Error("InputSystem already initialized");
			return;
		}

		// Set up key callback
		glfwSetKeyCallback(window, KeyCallback);

		// Set up mouse callback
		glfwSetCursorPosCallback(window, MousePositionCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetScrollCallback(window, MouseScrollCallback);

		initialized = true;
	}

	void InputSystem::Update()
	{
		// Process keys that need state transition from Triggered -> Down
		for (auto& [key, needsUpdate] : keysToUpdate)
		{
			if (needsUpdate && keyStates[key] == KeyState::Triggered)
			{
				keyStates[key] = KeyState::Down;
				keysToUpdate[key] = false;
			}
		}

		// Process keys that need state transition from Released -> None
		for (auto it = keyStates.begin(); it != keyStates.end(); /* no increment */)
		{
			if (it->second == KeyState::Released)
			{
				it = keyStates.erase(it);  // Remove from the map entirely
			}
			else
			{
				++it;
			}
		}

		// Process mouse buttons that need state transition from Triggered -> Down
		for (auto& [button, needsUpdate] : mouseButtonsToUpdate)
		{
			if (needsUpdate && mouseState.buttons[button] == KeyState::Triggered)
			{
				mouseState.buttons[button] = KeyState::Down;
				mouseButtonsToUpdate[button] = false;
			}
		}

		// Process mouse buttons that need state transition from Released -> None
		for (auto it = mouseState.buttons.begin(); it != mouseState.buttons.end(); /* no increment */)
		{
			if (it->second == KeyState::Released)
			{
				it = mouseState.buttons.erase(it);  // Remove from the map entirely
			}
			else
			{
				++it;
			}
		}

		// Update mouse deltas
		mouseState.positionDelta = mouseState.position - mouseState.positionPrevious;
		mouseState.positionPrevious = mouseState.position;

		// Clear scroll values only if they've been consumed
		if (!mouseState.hasScrolled)
		{
			mouseState.scroll = { 0.0f, 0.0f };
		}

		// Update frame timing
		double currentTime = glfwGetTime();
		lastFrameTime = currentTime;
	}

	PopplioMath::Vec2f InputSystem::GetMouse()
	{
		return mouseState.position;
	}

	PopplioMath::Vec2f InputSystem::GetMouseDelta()
	{
		return mouseState.positionDelta;
	}

	PopplioMath::Vec2f InputSystem::GetMousePrevious()
	{
		return mouseState.positionPrevious;
	}

	PopplioMath::Vec2f InputSystem::GetMouseScroll()
	{
		if (mouseState.hasScrolled)
		{
			PopplioMath::Vec2f scroll = mouseState.scroll;
			// Reset scroll values after they've been read
			mouseState.hasScrolled = false;
			return scroll;
		}
		return { 0.0f, 0.0f };
	}

	bool InputSystem::IsKeyTriggered(int key)
	{
		auto it = keyStates.find(key);
		// Mark that this key was checked and should transition next frame
		if (it != keyStates.end() && it->second == KeyState::Triggered)
		{
			keysToUpdate[key] = true;
			return true;
		}
		return false;
	}

	bool InputSystem::IsKeyDown(int key)
	{
		auto it = keyStates.find(key);
		return it != keyStates.end() && (it->second == KeyState::Down || it->second == KeyState::Triggered);
	}

	bool InputSystem::IsKeyReleased(int key)
	{
		auto it = keyStates.find(key);
		return it != keyStates.end() && it->second == KeyState::Released;
	}

	std::vector<int> InputSystem::GetKeysTriggered()
	{
		std::vector<int> triggeredKeys;
		for (const auto& [key, state] : keyStates)
		{
			if (state == KeyState::Triggered)
			{
				triggeredKeys.push_back(key);
			}
		}
		return triggeredKeys;
	}
    std::vector<int> InputSystem::GetKeysDown()
    {
        std::vector<int> downKeys;
        for (const auto& [key, state] : keyStates)
        {
            if (state == KeyState::Down)
            {
                downKeys.push_back(key);
            }
        }
        return downKeys;
    }
    std::vector<int> InputSystem::GetKeysReleased()
    {
        std::vector<int> releasedKeys;
        for (const auto& [key, state] : keyStates)
        {
            if (state == KeyState::Released)
            {
                releasedKeys.push_back(key);
            }
        }
        return releasedKeys;
    }

	bool InputSystem::IsMouseButtonTriggered(MouseButton button)
	{
		int buttonInt = static_cast<int>(button);
		auto it = mouseState.buttons.find(buttonInt);
		// Mark that this button was checked and should transition next frame
		if (it != mouseState.buttons.end() && it->second == KeyState::Triggered)
		{
			mouseButtonsToUpdate[buttonInt] = true;
			return true;
		}
		return false;
	}

	bool InputSystem::IsMouseButtonDown(MouseButton button)
	{
		int buttonInt = static_cast<int>(button);
		auto it = mouseState.buttons.find(buttonInt);
		return it != mouseState.buttons.end() && (it->second == KeyState::Down || it->second == KeyState::Triggered);
	}

	bool InputSystem::IsMouseButtonReleased(MouseButton button)
	{
		int buttonInt = static_cast<int>(button);
		auto it = mouseState.buttons.find(buttonInt);
		return it != mouseState.buttons.end() && it->second == KeyState::Released;
	}

	double InputSystem::GetInputLatency(int key)
	{
		if (keyStates[key] == KeyState::Triggered)
		{
			// needs to be enhanced for rhythm games
			double currentTime = glfwGetTime();
			inputTimestamps.push(currentTime);
			return 0.0;
		}
		return -1.0;
	}

	void InputSystem::ClearInputHistory()
	{
		while (!inputTimestamps.empty())
		{
			inputTimestamps.pop();
		}
	}

	void InputSystem::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		static_cast<void*>(window);
		static_cast<void>(mods);
		static_cast<void>(scancode);

		switch (action)
		{
		case GLFW_PRESS:
			keyStates[key] = KeyState::Triggered;
			break;
		case GLFW_REPEAT:
			keyStates[key] = KeyState::Down;
			break;
		case GLFW_RELEASE:
			keyStates[key] = KeyState::Released;
			break;
		}
	}

	void InputSystem::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		static_cast<void*>(window);
		static_cast<void>(mods);

		switch (action)
		{
		case GLFW_PRESS:
			mouseState.buttons[button] = KeyState::Triggered;
			break;
		case GLFW_RELEASE:
			mouseState.buttons[button] = KeyState::Released;
			break;
		}
	}

	void InputSystem::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		static_cast<void>(window);
		mouseState.position = { static_cast<float>(xpos), static_cast<float>(ypos) };
	}

	void InputSystem::MouseScrollCallback(GLFWwindow* window, double x, double y)
	{
		static_cast<void>(window);
		mouseState.scroll = { static_cast<float>(x), static_cast<float>(y) };
		mouseState.hasScrolled = true;
	}
}