#pragma once

#include <FishEngine/GameObject.hpp>
#include <FishGUI/ModelView/TreeWidget.hpp>

class HierarchyView : public FishGUI::TreeWidget<FishEngine::Transform*>
{
public:
	typedef FishGUI::TreeWidget<FishEngine::Transform*> Super;

	HierarchyView(const char* name);

private:
	void OnSelectionChanged();
};
