#pragma once

#include <FishEngine/Transform.hpp>

namespace FishEditor
{
	class Selection
	{
	public:
		Selection() = delete;

		static FishEngine::Object* activeObject()
		{
			return s_activeObject;
		}

		static FishEngine::Transform* GetActiveTransform()
		{
			if (s_activeObject != nullptr && s_activeObject->GetClassID() == FishEngine::Transform::ClassID)
				return static_cast<FishEngine::Transform*>(s_activeObject);
			return nullptr;
		}

	private:
		friend class HierarchyWidget;
		static FishEngine::Object* s_activeObject;
	};
}