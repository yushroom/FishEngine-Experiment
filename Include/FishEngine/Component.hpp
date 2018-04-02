#pragma once

#include "Object.hpp"

namespace FishEngine
{
	class GameObject;
	class Transform;
	class Prefab;

	class Component : public Object
	{
	public:
		DeclareObject(Component, 2);

		Component(int classID, const char* className) : Object(classID, className)
		{
		}
		
		virtual ~Component()
		{
		}
		
		Transform* GetTransform() const;
		GameObject* GetGameObject() const { return m_GameObject; }
//		void SetGameObject(GameObject* gameoObject);

		Component* GetPrefabParentObject() const { return m_PrefabParentObject; }
		void SetPrefabParentObject(Component* value) { m_PrefabParentObject = value; }

		Prefab* GetPrefabInternal() const { return m_PrefabInternal; }
		void SetPrefabInternal(Prefab* value) { m_PrefabInternal = value; }

	protected:
		friend class GameObject;
		Component * m_PrefabParentObject = nullptr;
		Prefab * m_PrefabInternal = nullptr;
		GameObject* m_GameObject = nullptr;
	};
}
