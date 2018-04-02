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

		static FishEngine::Object* GetActiveObject()
		{
			return s_ActiveObject;
		}

		static void SetActiveObject(FishEngine::Object* obj)
		{
			s_ActiveObject = obj;
//			puts("SetActiveObject");
			OnSelectionChanged();
		}


		static FishEngine::Transform* GetActiveTransform()
		{
			if (s_ActiveObject != nullptr)
				return s_ActiveObject->As<FishEngine::Transform>();
			return nullptr;
		}
		
		static void SetActiveTransform(FishEngine::Transform* t)
		{
			SetActiveObject(t);
		}
		
		static boost::signals2::signal<void(void)> OnSelectionChanged;

	private:
		//friend class ::HierarchyView;
		static FishEngine::Object* s_ActiveObject;
	};
}
