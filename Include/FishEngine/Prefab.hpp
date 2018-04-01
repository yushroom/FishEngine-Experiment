#pragma once

#include "Object.hpp"
#include <vector>
#include <map>

namespace FishEditor
{
	class FBXImporter;
	class NativeFormatImporter;
}

namespace FishEngine
{
	class GameObject;
	class Component;
	class Transform;

	struct Modification
	{
		Object* 		target = nullptr;
		std::string 	propertyPath;
		std::string 	value;
		Object*		objectReference = nullptr;
	};
	
	struct PrefabModification
	{
		Transform* 					m_TransformParent = nullptr;
		std::vector<Modification> 	m_Modifications;
		std::vector<Component*> 		m_RemovedComponents;
	};

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

		Prefab* InstantiateWithModification(const PrefabModification& modification);


		void AddObject(int64_t fileID, Object* obj)
		{
			m_FileIDToObject[fileID] = obj;
		}

		Object* GetObjectByFileID(int64_t fileID)
		{
			auto it = m_FileIDToObject.find(fileID);
			assert(it != m_FileIDToObject.end());
//			return this->m_FileIDToObject[fileID];
			return it->second;
		}

	protected:
		friend class FishEditor::FBXImporter;
		friend class FishEditor::NativeFormatImporter;
		PrefabModification 			m_Modification;
		Prefab *					m_ParentPrefab = nullptr;
		GameObject *				m_RootGameObject = nullptr;
		bool						m_IsPrefabParent = true;
		
		std::string					m_GUID;
		std::map<int64_t, Object*> 	m_FileIDToObject;
	};
}
