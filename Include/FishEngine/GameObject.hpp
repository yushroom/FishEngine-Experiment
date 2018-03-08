#pragma once

#include "FishEngine.hpp"
#include "Object.hpp"
#include "Scene.hpp"
#include "Component.hpp"
#include "Transform.hpp"

#include <list>
#include <vector>
#include <type_traits>
#include <typeinfo>

namespace FishEngine
{
	class Component;
	class Transform;
	class RectTransform;

	class GameObject : public Object
	{
	public:
		enum {ClassID = 1};
		
		GameObject(const std::string& name = "GameObject");
		~GameObject();

		// for python
		// create GameObject and bind transform
//		static GameObject* CreateWithTransform(const std::string& name = "GameObject");
		
		// for python
		// special version for RectTransform
//		static GameObject* CreateWithRectTransform(const std::string& name = "GameObject");

		Transform* GetTransform() const
		{
			return m_transform;
		}

		const std::list<Component*>& GetAllComponents() const
		{
			return m_components;
		}

		void AddComponent(Component* comp);
		
		template<class T>
		T* GetComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
			for (Component* t : m_components)
			{
				if (t->GetClassID() == T::ClassID)
				{
					return dynamic_cast<T*>(t);
				}
			}
			return nullptr;
		}
		
		Component* GetComponent(int classID)
		{
			for (Component* t : m_components)
			{
				if (t->GetClassID() == classID)
				{
					return t;
				}
			}
			return nullptr;
		}
		
		template<class T>
		T* GetComponentInChildren()
		{
			for (auto child : m_transform->GetChildren())
			{
				auto ret = child->gameObject()->GetComponent<T>();
				if (ret != nullptr)
					return ret;
				ret = child->gameObject()->GetComponentInChildren<T>();
				if (ret != nullptr)
					return ret;
			}
			return nullptr;
		}
		
//		void AddRectTransform(RectTransform* t);
//		void RemoveRectTransform();

		Scene* GetScene() const
		{
			return m_scene;
		}

	protected:
		Scene*					m_scene = nullptr;
		Transform*				m_transform = nullptr;
		std::list<Component*>	m_components;
	};

	inline void GameObject::AddComponent(Component* comp)
	{
		m_components.push_back(comp);
		comp->m_gameObject = this;
	}
}
