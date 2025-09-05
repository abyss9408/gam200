/******************************************************************************/
/*!
\file InputSystem.h
\author Val Tay Yun Ying				 (tay.v) (80%)
\co-author: ANG Bryan Wei Ze			 (bryanweize.ang) (20%)
\co-author: NG Jun Heng Shawn			 (n.junhengshawn) (5%)
\date 03-14-2025


All content � 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include <GLFW/glfw3.h>
#include <queue>
#include <array>
#include <unordered_map>
#include "../Math/Vector2D.h"

namespace Popplio
{
	

	class InputSystem
	{
	public:

		enum class KeyState
		{
			None,
			Triggered,  // Just pressed this frame
			Down,       // Held down
			Released    // Just released this frame
		};

		enum class MouseButton
		{
			Left = GLFW_MOUSE_BUTTON_LEFT,
			Right = GLFW_MOUSE_BUTTON_RIGHT,
			Middle = GLFW_MOUSE_BUTTON_MIDDLE
		};

		struct MouseState
		{
			PopplioMath::Vec2f position = { 0.0, 0.0 };
			PopplioMath::Vec2f positionPrevious = { 0.0, 0.0 };
			PopplioMath::Vec2f positionDelta = { 0.0, 0.0 };
			PopplioMath::Vec2f positionWorld = { 0.0, 0.0 };
			PopplioMath::Vec2f scroll = { 0.0, 0.0 };
			bool hasScrolled = false;
			std::unordered_map<int, KeyState> buttons;
		};

	private:
		static std::unordered_map<int, KeyState> keyStates;
		static MouseState mouseState;
		static bool initialized;

		// pending state changes
		static std::unordered_map<int, bool> keysToUpdate;
		static std::unordered_map<int, bool> mouseButtonsToUpdate;

		// Keep track of frame timing
		static double lastFrameTime;
		static std::queue<double> inputTimestamps;

		// Callback handlers
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void MouseScrollCallback(GLFWwindow* window, double x, double y);

	public:
		InputSystem(const InputSystem&) = delete;
		InputSystem(InputSystem&&) = delete;
		InputSystem& operator=(const InputSystem&) = delete;
		InputSystem& operator=(InputSystem&&) = delete;
		~InputSystem() = delete;

		static void Init(GLFWwindow* window);
		static void Update();

		// Keyboard Input Methods
		static bool IsKeyTriggered(int key);
		static bool IsKeyDown(int key);
		static bool IsKeyReleased(int key);

		static std::vector<int> GetKeysTriggered();
        static std::vector<int> GetKeysDown();
        static std::vector<int> GetKeysReleased();

		// Mouse Input Methods
		static bool IsMouseButtonTriggered(MouseButton button);
		static bool IsMouseButtonDown(MouseButton button);
		static bool IsMouseButtonReleased(MouseButton button);

		// Mouse Position Methods
		static PopplioMath::Vec2f GetMouse();
		static PopplioMath::Vec2f GetMouseDelta();
		static PopplioMath::Vec2f GetMousePrevious();

		// Mouse Scroll Methods
		static PopplioMath::Vec2f GetMouseScroll();

		// Frame Timing Methods
		static double GetInputLatency(int key);
		static void ClearInputHistory();
	};
}
