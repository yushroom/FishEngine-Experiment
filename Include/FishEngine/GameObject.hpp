#pragma once

#include "FishEngine.hpp"
#include "Object.hpp"
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
	class Scene;
	
	enum class GameObjectConstructionFlag
	{
		WithTransform,
		WithRectTransform,
		Empty,
	};

	class GameObject : public Object
	{
	public:
		enum {ClassID = 1};
		
		GameObject(const std::string& name = "GameObject", GameObjectConstructionFlag flag = GameObjectConstructionFlag::WithTransform);
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
		
		// Returns the component of Type type in the GameObject or any of its children using depth first search.
		// A component is returned only if it is found on an active GameObject.
		template<class T>
		T* GetComponentInChildren()
		{
			// find self
			auto c = GetComponent<T>();
			if (c != nullptr)
				return c;
				
			// find children
			for (auto child : m_transform->GetChildren())
			{
				auto ret = child->GetGameObject()->GetComponentInChildren<T>();
				if (ret != nullptr)
					return ret;
			}
			return nullptr;
		}
		
		// Returns all components of Type type in the GameObject or any of its children.
		template<class T>
		void GetComponentsInChildren(std::vector<T*>& components)
		{
			// find self
			auto c = GetComponent<T>();
			if (c != nullptr)
				components.push_back(c);
			
			// find children
			for (auto child : m_transform->GetChildren())
			{
				child->GetGameObject()->GetComponentsInChildren<T>(components);
			}
		}
		
//		void AddRectTransform(RectTransform* t);
//		void RemoveRectTransform();

		Scene* GetScene() const
		{
			return m_scene;
		}
		
		GameObject* Clone(Scene* scene = nullptr);
		
		// The local active state of this GameObject.
		bool IsActive() const { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }
		
		// Is the GameObject active in the scene?
		bool IsActiveInHierarchy() const;

	protected:
		Scene*					m_scene = nullptr;
		Transform*				m_transform = nullptr;
		std::list<Component*>	m_components;
		bool					m_IsActive = true;	// activeSelf
	};
}
