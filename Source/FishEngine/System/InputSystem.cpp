#include <FishEngine/System/InputSystem.hpp>

namespace FishEngine
{
	void InputSystem::Init()
	{
		for (auto& s : m_keyStates)
			s = KeyState::None;
		for (auto& b : m_mouseButtonStates)
			b = MouseButtonState::None;
		
		for (auto& a : m_axis) {
			a = 0.f;
		}
	}
	
	void InputSystem::Update()
	{
		for (auto& s : m_keyStates) {
			if (s == KeyState::Down)
				s = KeyState::Held;
			if (s == KeyState::Up)
				s = KeyState::None;
		}
		
		for (auto& b : m_mouseButtonStates) {
			if (b == MouseButtonState::Down)
				b = MouseButtonState::Held;
			else if (b == MouseButtonState::Up)
				b = MouseButtonState::None;
		}
		
		for (auto& a : m_axis) {
			a = 0.f;
		}
	}
	
	void InputSystem::UpdateAxis(Axis axis, float value)
	{
		m_axis[(int)axis] = value;
	}
	
	void InputSystem::UpdateMousePosition(float xpos, float ypos)
	{
		m_axis[(int)Axis::MouseX] = xpos - m_mousePosition.x;
		m_axis[(int)Axis::MouseY] = ypos - m_mousePosition.y;
		m_mousePosition.x = xpos;
		m_mousePosition.y = ypos;
	}
	
	void InputSystem::UpdateKeyState(int key, KeyState state)
	{
		if (key >= 0 && key < KeyCount)
			m_keyStates[key] = state;
	}
	
	void InputSystem::UpdateKeyState(KeyCode key, KeyState state)
	{
		int iKey = static_cast<int>(key);
		UpdateKeyState(iKey, state);
	}
	
	void InputSystem::UpdateMouseButtonState(int button, MouseButtonState state)
	{
		if (button >= 0 && button <= 3)
			m_mouseButtonStates[button] = state;
	}
}
