#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/RectTransform.hpp>
//#include <FishEngine/Script.hpp>
#include <FishEngine/Scene.hpp>

namespace FishEngine
{
    GameObject::GameObject(const std::string& name, GameObjectConstructionFlag flag) : Object(GameObject::ClassID)
    {
        LOGF;
        this->SetName(name);
		m_scene = SceneManager::GetActiveScene();
		
		if (flag == GameObjectConstructionFlag::WithTransform)
		{
			m_transform = new Transform();
			m_scene->AddRootTransform(m_transform);
			m_transform->m_gameObject = this;
			m_components.push_back(m_transform);
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
		LOGF;
		//for (auto comp : m_components)
		//{
		//	delete comp;
		//}
		delete m_transform;
		m_transform = nullptr;
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
	
	GameObject* GameObject::Clone(Scene* scene)
	{
		if (scene == nullptr)
			scene = this->m_scene;
		auto cloned = new GameObject(this->m_name, GameObjectConstructionFlag::Empty);
		cloned->m_scene = scene;
		auto cloned_t = m_transform->Clone();
		cloned->m_transform = cloned_t;
		cloned->m_components.push_back(cloned_t);
		cloned_t->m_gameObject = this;
		cloned->m_scene->AddRootTransform(cloned_t);
//		cloned->m_components.resize( m_components.size() );
		
		cloned->m_transform->m_children.reserve(this->m_transform->m_children.size());
		for (auto child : this->m_transform->m_children)
		{
			auto cloned_child = child->GetGameObject()->Clone();
			cloned_child->m_transform->SetParent(cloned_child->m_transform);
//			cloned->m_transform->m_children.push_back(cloned_child->m_transform);
		}
		
		return cloned;
	}
}
