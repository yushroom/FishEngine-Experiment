#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/RectTransform.hpp>
#include <FishEngine/Script.hpp>
#include <FishEngine/Scene.hpp>

#include <pybind11/embed.h>
#include <FishEngine/Debug.hpp>

namespace FishEngine
{
	GameObject::GameObject(const std::string& name, GameObjectConstructionFlag flag)
			: Object(ClassID, ClassName)
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
		if (comp == nullptr)
		{
			LogWarning("the component is nullptr");
			return;
		}
		if (comp->m_GameObject != nullptr)
		{
			assert(comp->m_GameObject == this);
		}
		if (comp->GetClassID() == Transform::ClassID)
		{
			if (m_Transform != nullptr)
				assert(m_Transform == comp);
			m_Transform = (Transform*)comp;
		}

		// do not add it twice!
		auto pos = std::find(m_Component.begin(), m_Component.end(), comp);
		if (pos == m_Component.end())
			m_Component.push_back(comp);
		comp->m_GameObject = this;
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
