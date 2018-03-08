#include "HierarchyWidget.hpp"

#include <FishGUI/ModelView/TreeWidget.hpp>

#include <FishEngine/Transform.hpp>
#include <FishEngine/GameObject.hpp>

class HierarchyModel : public FishGUI::TItemModel<FishEngine::Transform*>
{
	typedef FishEngine::Transform* T;
public:

	virtual T Parent(T child) const override
	{
		return child->GetParent();
	}

	virtual T ChildAt(int index, T parent) const override
	{
		if (parent == nullptr)
		{
			auto& roots = FishEngine::SceneManager::GetActiveScene()->GetRootTransforms();
			return roots[index];
		}
		else
		{
			return parent->GetChildAt(index);
		}
	}

	virtual int ChildCount(T item) const override
	{
		if (item == nullptr)
		{
			return FishEngine::SceneManager::GetActiveScene()->GetRootCount();
		}
		return (int)item->GetChildren().size();
	}

	virtual std::string Text(T item) const override
	{
		return item->GetGameObject()->GetName();
	}
};


HierarchyWidget::HierarchyWidget(const char* name) : Super(name)
{
	m_model = new HierarchyModel();
	m_imContext->ymargin = 0;

	m_selectionModel.selectionChanged.connect([this](FishEngine::Transform* selected) {
		this->OnSelectionChanged();
	});
}

#include <FishEditor/Selection.hpp>

void HierarchyWidget::OnSelectionChanged()
{
	if (m_selectionModel.SelectedItems().empty())
	{
		FishEditor::Selection::s_activeObject = nullptr;
	}
	else
	{
		FishEditor::Selection::s_activeObject = m_selectionModel.SelectedItem();
	}
}