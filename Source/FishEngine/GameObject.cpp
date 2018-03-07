#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/RectTransform.hpp>
#include <FishEngine/Script.hpp>

namespace FishEngine
{
    GameObject::GameObject(const std::string& name) : Object(GameObject::ClassID)
    {
        LOGF;
        this->name = name;
		m_scene = SceneManager::GetActiveScene();
        m_transform = new Transform();
		m_scene->AddTransform(m_transform);
        m_transform->m_gameObject = this;
        m_components.push_back(m_transform);
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
		LOGF;
		for (auto comp : m_components)
		{
			delete comp;
		}
		m_transform = nullptr;
	}

	void GameObject::AddComponent(Component* comp)
	{
		m_components.push_back(comp);
		comp->m_gameObject = this;
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
}
