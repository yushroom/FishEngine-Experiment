#pragma once

#include <FishEngine/GameObject.hpp>
#include <FishGUI/ModelView/TreeWidget.hpp>

namespace FishEditor
{
	class EditorApplication;
}

class HierarchyView : public FishGUI::TreeWidget<FishEngine::Transform*>
{
public:
	typedef FishGUI::TreeWidget<FishEngine::Transform*> Super;

	explicit HierarchyView(const char* name);

private:
	friend class FishEditor::EditorApplication;
	void OnSelectionChanged();
};
