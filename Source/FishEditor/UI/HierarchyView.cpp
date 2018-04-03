#include <FishEditor/UI/HierarchyView.hpp>

#include <FishGUI/ModelView/TreeWidget.hpp>

#include <FishEngine/Transform.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEditor/Selection.hpp>

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

	virtual NVGcolor TextColor(T item) const override
	{
		FishEngine::Color c(0, 0, 0, 1);
		bool isPrefab = item->GetGameObject()->GetPrefabInternal() != nullptr;
		bool active = item->GetGameObject()->IsActiveInHierarchy();
		if (isPrefab)
		{
			if (active)
				c = FishEngine::Color(10, 47, 134, 255) / 255;
			else
				c = FishEngine::Color(84, 106, 158, 255) / 255;
		}
		else
		{
			if (!active)
				c = FishEngine::Color(84, 84, 84, 255) / 255;
		}
		return NVGcolor{{c.r, c.g, c.b, 1.0f}};
	}
};


HierarchyView::HierarchyView(const char* name) : Super(name)
{
	m_model = new HierarchyModel();
	m_imContext->ymargin = 0;

	FishEditor::Selection::OnSelectionChanged.connect([this](){
		auto t = FishEditor::Selection::GetActiveTransform();
		if (t != nullptr && t != m_selectionModel.SelectedItem())
		{
			this->m_selectionModel.BlockSignals(true);
			this->m_selectionModel.SelectItem(t);
			this->m_selectionModel.BlockSignals(false);
		}
	});

	m_selectionModel.OnSelectionChanged.connect([this](FishEngine::Transform* selected) {
		this->OnSelectionChanged();
	});
}

#include <FishEditor/Selection.hpp>

void HierarchyView::OnSelectionChanged()
{
	if (m_selectionModel.SelectedItems().empty())
	{
		FishEditor::Selection::SetActiveObject(nullptr);
	}
	else
	{
		FishEditor::Selection::SetActiveObject(m_selectionModel.SelectedItem());
	}
}
