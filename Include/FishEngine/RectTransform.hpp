#pragma once

#include "Component.hpp"
#include "Math/Vector2.hpp"

namespace FishEngine
{
	class RectTransform : public Component
	{
//	private:
	public:
		
		enum {ClassID = 224};
		
		Vector2 m_AnchorMin = {0.5f, 0.5f};
		Vector2 m_AnchorMax = {0.5f, 0.5f};
		Vector2 m_AnchoredPosition = {0, 0};
		Vector2 m_SizeDelta = {100, 100};
		Vector2 m_Pivot = {0.5f, 0.5f};
		
	public:
		RectTransform()
		{
			Object::s_objects[ClassID].insert(this);
		}
		
		~RectTransform()
		{
			Object::s_objects[ClassID].erase(this);
		}
		
		// for python
		RectTransform* Create();
	};
}
