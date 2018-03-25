#pragma once

#include "Object.hpp"

namespace FishEngine
{
	class GameObject;

	class Prefab : public Object
	{
	public:
		enum { ClassID = 1001 };

		Prefab() : Object(ClassID) {

		}

		GameObject* GetRootGameObject() const
		{
			return m_RootGameObject;
		}

		void SetRootGameObject(GameObject* root)
		{
			m_RootGameObject = root;
		}

	protected:
		Prefab *		m_ParentPrefab = nullptr;
		GameObject *	m_RootGameObject = nullptr;
		bool			m_IsPrefabParent = true;
	};
}