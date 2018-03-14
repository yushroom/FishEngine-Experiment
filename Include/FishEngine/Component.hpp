#pragma once

#include "Object.hpp"

namespace FishEngine
{
	class GameObject;
	class Transform;

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

	protected:
		friend class GameObject;
		GameObject* m_GameObject = nullptr;
	};
}
