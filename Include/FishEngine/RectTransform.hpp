#pragma once

#include "Component.hpp"
#include "Math/Vector2.hpp"
#include "Math/Rect.hpp"

namespace FishEngine
{
	class RectTransform final : public Component
	{
	public:
		DeclareObject(RectTransform, 224);

		Vector2 m_AnchorMin = {0.5f, 0.5f};
		Vector2 m_AnchorMax = {0.5f, 0.5f};
		Vector2 m_AnchoredPosition = {0, 0};
		Vector2 m_SizeDelta = {100, 100};
		Vector2 m_Pivot = {0.5f, 0.5f};
		
		mutable Rect m_Rect;
		
	public:
		RectTransform() : Component(RectTransform::ClassID, ClassName)
		{
		}
		
		~RectTransform()
		{
		}
		
		void Update();
	};
}
