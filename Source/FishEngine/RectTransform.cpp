#include <FishEngine/RectTransform.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/GameObject.hpp>

namespace FishEngine
{
	void RectTransform::Update()
	{
		auto parent = this->GetTransform()->GetParent();
		if (parent != nullptr)
		{
			auto parent_rt = parent->GetGameObject()->GetComponent<RectTransform>();
			assert(parent_rt != nullptr);
			auto& parentRect = parent_rt->m_Rect;
			
			if (m_AnchorMin == m_AnchorMax)
			{
				Vector2 anchor;
				anchor.x = parentRect.m_Width * m_AnchorMin.x;
				anchor.y = parentRect.m_Height * m_AnchorMin.y;
				int width = m_SizeDelta.x;
				int height = m_SizeDelta.y;
				auto pivot = m_Pivot;
				pivot.x *= width;
				pivot.y *= height;
				auto pivotP = anchor + m_AnchoredPosition;
				Vector2 pos = pivotP - pivot;
				m_Rect.m_XMin = pos.x;
				m_Rect.m_YMin = pos.y;
				m_Rect.m_Width = width;
				m_Rect.m_Height = height;
			}
		}
		
		for (auto childT : this->GetTransform()->GetChildren())
		{
			auto childRT = childT->GetGameObject()->GetComponent<RectTransform>();
			assert(childRT != nullptr);
			childRT->Update();
		}
	}
}
