#pragma once

#include "Object.hpp"
#include <vector>
#include <map>
#include <cassert>

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
		Object*			objectReference = nullptr;
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
			printf("Prefab::Prefab, instanceID=%d\n", this->GetInstanceID());
		}

		virtual ~Prefab() {
//			printf("delete prefab: %d\n", this->GetInstanceID());
		}

//		const PrefabModification& GetModification() const { return m_Modification; }
//		void SetModification(const PrefabModification& value) { m_Modification = value; }

		Prefab* GetParentPrefab() const { return m_ParentPrefab; }
//		void SetParentPrefab(Prefab* value) { m_ParentPrefab = value; }

		GameObject* GetRootGameObject() const { return m_RootGameObject; }
		void SetRootGameObject(GameObject* value) { m_RootGameObject = value; }

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
