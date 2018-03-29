#pragma once

#include "Object.hpp"

namespace FishEngine
{
	class GameObject;

	class Prefab : public Object
	{
	public:
		DeclareObject(Prefab, 1001);

		Prefab() : Object(ClassID, ClassName) {

		}

		GameObject* GetRootGameObject() const
		{
			return m_RootGameObject;
		}

		void SetRootGameObject(GameObject* root)
		{
			m_RootGameObject = root;
		}

		Prefab* Instantiate();

	protected:
		Prefab *		m_ParentPrefab = nullptr;
		GameObject *	m_RootGameObject = nullptr;
		bool			m_IsPrefabParent = true;
		
		std::string		m_GUID;
	};


}
