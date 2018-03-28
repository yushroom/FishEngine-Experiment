#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/RectTransform.hpp>
#include <FishEngine/Script.hpp>
#include <FishEngine/Scene.hpp>

#include <FishEngine/Archive.hpp>

#include <pybind11/embed.h>

namespace FishEngine
{
    GameObject::GameObject(const std::string& name, GameObjectConstructionFlag flag) : Object(GameObject::ClassID)
    {
        LOGF;
        this->SetName(name);
		m_Scene = SceneManager::GetActiveScene();
		
		if (flag == GameObjectConstructionFlag::WithTransform)
		{
			m_Transform = new Transform();
			m_Scene->AddRootTransform(m_Transform);
			m_Transform->m_GameObject = this;
			m_Component.push_back(m_Transform);
		}
		else if (flag == GameObjectConstructionFlag::WithRectTransform)
		{
			abort();
		}
		else
		{
			// empty
		}
    }
	
//	// for python
//	// create GameObject and bind transform
//	GameObject* GameObject::CreateWithTransform(const std::string& name)
//	{
//		auto go = new GameObject(name);
//		go->m_transform = new Transform();
//		go->m_transform->m_gameObject = go;
//		go->m_components.push_back(go->m_transform);
//		return go;
//	}
//
//	// for python
//	// special version for RectTransform
//	GameObject* GameObject::CreateWithRectTransform(const std::string& name)
//	{
//		auto go = new GameObject(name);
//		go->m_transform = new RectTransform();
//		go->m_transform->m_gameObject = go;
//		go->m_components.push_back(go->m_transform);
//		return go;
//	}
	
	GameObject::~GameObject()
	{
//		LOGF;
//		printf("~GameObject %s\n", m_name.c_str());
		for (auto comp : m_Component)
		{
			if (comp->GetClassID() != Script::ClassID)
				delete comp;
		}
		//delete m_transform;
		//m_transform = nullptr;
	}

	void GameObject::AddComponent(Component* comp)
	{
		assert(comp->m_GameObject == nullptr);
		m_Component.push_back(comp);
		comp->m_GameObject = this;
		//if (comp->GetClassID() == Script::ClassID)
		//{
		//	comp->m_self.attr
		//}
	}

	
//	void GameObject::AddRectTransform(RectTransform* t)
//	{
//		auto old = m_transform;
////		t->setLocalPosition(old->localPosition());
////		t->setLocalRotation(old->localRotation());
////		t->setLocalScale(old->localScale());
//		t->m_localPosition = old->m_localPosition;
//		t->m_localRotation = old->m_localRotation;
//		t->m_localScale = old->m_localScale;
//		t->m_RootOrder = old->m_RootOrder;
//		t->m_children = std::move(old->m_children);
//		t->m_parent = old->m_parent;
//		t->m_isDirty = true;	// mark dirty, update matrix
//		old->m_parent->m_children[old->m_RootOrder] = t;
//		m_transform = t;
//		delete old;
//	}
	
//	void GameObject::RemoveRectTransform()
//	{
//
//	}
	
	GameObject* GameObject::Clone()
	{
		CollectObjectArchive archive;
		this->Serialize(archive);

		std::ofstream fout("test_output_archive.yaml");
		OutputArchive archive2(fout);
		archive2.Dump(this);
		fout.close();

		auto cloned = new GameObject(this->m_Name, GameObjectConstructionFlag::Empty);
		//auto module = pybind11::module::import("FishEngine");
		//cloned->m_self = module.attr("GameObject")();
		auto cloned_t = m_Transform->Clone();
		cloned->m_Transform = cloned_t;
		cloned->m_Component.push_back(cloned_t);
		cloned_t->m_GameObject = cloned;
		cloned->m_Scene->AddRootTransform(cloned_t);

		auto it = m_Component.begin();
		it++;
		for (; it != m_Component.end(); ++it)
		{
			auto c = *it;
			cloned->AddComponent(c->Clone());
		}
		
		cloned->m_Transform->m_Children.reserve(this->m_Transform->m_Children.size());
		for (auto child : this->m_Transform->m_Children)
		{
			auto cloned_child = child->GetGameObject()->Clone();
			cloned_child->m_Transform->SetParent(cloned->m_Transform, false);
//			cloned->m_transform->m_children.push_back(cloned_child->m_transform);
		}
		
		return cloned;
	}
	
	bool GameObject::IsActiveInHierarchy() const
	{
		if (!m_IsActive)
			return false;
		
		if (m_Transform->GetParent() != nullptr)
		{
			return m_Transform->GetParent()->GetGameObject()->IsActiveInHierarchy();
		}
		return true;
	}
}
