#pragma once

#include <FishEngine/Transform.hpp>
#include <boost/signals2/signal.hpp>

class HierarchyView;

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
		
		static void SetActiveTransform(FishEngine::Transform* t)
		{
			s_activeObject = t;
		}
		
		static boost::signals2::signal<void(void)> selectionChanged;

	private:
		friend class ::HierarchyView;
		static FishEngine::Object* s_activeObject;
	};
}
