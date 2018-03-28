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
		enum {ClassID = 2};
		Component(int classID) : Object(classID)
		{
		}
		
		virtual ~Component()
		{
		}
		
		Transform* GetTransform() const;
		GameObject* GetGameObject() const
		{
			return m_GameObject;
		}

		virtual Component* Clone() const = 0;

		Prefab* GetPrefabParentObject() const { return m_PrefabParentObject; }
		void SetPrefabParentObject(Prefab* value) { m_PrefabParentObject = value; }

		Prefab* GetPrefabInternal() const { return m_PrefabInternal; }
		void SetPrefabInternal(Prefab* value) { m_PrefabInternal = value; }

		virtual void Serialize(Archive& archive) const override;

	protected:
		friend class GameObject;
		Prefab * m_PrefabParentObject = nullptr;
		Prefab * m_PrefabInternal = nullptr;
		GameObject* m_GameObject = nullptr;
	};
}
