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

		static void SetActiveObject(FishEngine::Object* obj)
		{
			s_ActiveObject = obj;
			OnSelectionChanged();
		}

		static FishEngine::Object* GetActiveObject()
		{
			return s_ActiveObject;
		}

		static FishEngine::Transform* GetActiveTransform()
		{
			if (s_ActiveObject != nullptr && s_ActiveObject->GetClassID() == FishEngine::Transform::ClassID)
				return static_cast<FishEngine::Transform*>(s_ActiveObject);
			return nullptr;
		}
		
		static void SetActiveTransform(FishEngine::Transform* t)
		{
			s_ActiveObject = t;
			OnSelectionChanged();
		}
		
		static boost::signals2::signal<void(void)> OnSelectionChanged;

	private:
		//friend class ::HierarchyView;
		static FishEngine::Object* s_ActiveObject;
	};
}
