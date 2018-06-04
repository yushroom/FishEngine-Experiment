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
	class Prefab;
	
	enum class GameObjectConstructionFlag
	{
		WithTransform,
		WithRectTransform,
		Empty,
	};

	class GameObject : public Object
	{
	public:
		DeclareObject(GameObject, 1);
		
		GameObject(const std::string& name = "GameObject", GameObjectConstructionFlag flag = GameObjectConstructionFlag::WithTransform);
		~GameObject();

		Transform* GetTransform() const
		{
			return m_Transform;
		}

		const std::list<Component*>& GetAllComponents() const
		{
			return m_Component;
		}

		void AddComponent(Component* comp);
		
		template<class T>
		T* GetComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
			for (Component* t : m_Component)
			{
				if (t->Is<T>())
				{
					return dynamic_cast<T*>(t);
				}
			}
			return nullptr;
		}
		
		Component* GetComponent(int classID)
		{
			for (Component* t : m_Component)
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
			for (auto child : m_Transform->GetChildren())
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
			for (auto child : m_Transform->GetChildren())
			{
				child->GetGameObject()->GetComponentsInChildren<T>(components);
			}
		}


		Scene* GetScene() const
		{
			return m_Scene;
		}
		
		const std::string& GetTag() const { return m_TagString; }
		
		// The local active state of this GameObject.
		bool IsActive() const { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }
		
		// Is the GameObject active in the scene?
		bool IsActiveInHierarchy() const;

		GameObject* GetPrefabParentObject() const { return m_PrefabParentObject; }
		void SetPrefabParentObject(GameObject* value) { m_PrefabParentObject = value; }

		Prefab* GetPrefabInternal() const { return m_PrefabInternal; }
		void SetPrefabInternal(Prefab* value) { m_PrefabInternal = value; }

	protected:
		GameObject * 			m_PrefabParentObject = nullptr;
		Prefab * 				m_PrefabInternal = nullptr;
		Scene*					m_Scene = nullptr;
		Transform*				m_Transform = nullptr;
		std::list<Component*>	m_Component;
		int 					m_Layer;
		std::string 			m_TagString;
		bool					m_IsActive = true;	// activeSelf
	};
}
