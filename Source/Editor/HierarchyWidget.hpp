#pragma once

#include <FishEngine/GameObject.hpp>
#include <FishGUI/ModelView/TreeWidget.hpp>

class HierarchyWidget : public FishGUI::TreeWidget<FishEngine::Transform*>
{
public:
	typedef FishGUI::TreeWidget<FishEngine::Transform*> Super;

	HierarchyWidget(const char* name);

private:
	void OnSelectionChanged();
};
