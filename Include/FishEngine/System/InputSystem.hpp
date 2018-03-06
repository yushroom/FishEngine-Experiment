#pragma once

#include "../Math/Vector2.hpp"
#include "../KeyCode.hpp"

namespace FishEngine
{
	enum class KeyState {
		Up = 0,
		Down = 1,
		Held = 2,
		None = 3,
	};
	
	enum class MouseButtonState {
		None = 0,
		Down = 1,
		Held = 2,
		Up = 3,
	};
	
	enum class MouseButtonCode {
		Left = 0,
		Right = 1,
		Middle = 2,
	};
	
	enum class Axis {
		Vertical = 0,   // w, a, s, d and arrow keys
		Horizontal,
//		Fire1,          // Control
//		Fire2,          // Option(Alt)
//		Fire3,          // Command
//		Jump,
		MouseX,         // delta of mouse movement
		MouseY,
//		MouseScrollWheel,
//		WindowShakeX,   // movement of the window
//		WindwoShakeY,
		
		AxisCount,		// not a enum
	};
	
	constexpr int KeyCount = 512;
	
	class InputSystem
	{
	public:
		static InputSystem& GetInstance()
		{
			static InputSystem instance;
			return instance;
		}
		
		void Init();
		void Update();
		
	private:
		friend class GameApp;
		
		InputSystem() = default;
		InputSystem(InputSystem&) = delete;
		InputSystem& operator=(InputSystem&) = delete;
		
		void UpdateAxis(Axis axis, float value);
		void UpdateMousePosition(float xpos, float ypos);
		void UpdateKeyState(KeyCode key, KeyState state);
		void UpdateKeyState(int key, KeyState state);
		void UpdateMouseButtonState(int button, MouseButtonState state);
		
	private:
		KeyState m_keyStates[KeyCount];
		MouseButtonState m_mouseButtonStates[3];
		Vector2 m_mousePosition = {0, 0};
		float m_axis[(int)Axis::AxisCount] = {0.0f};
	};
}
