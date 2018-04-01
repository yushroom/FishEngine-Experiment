#include <FishEngine/Prefab.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Serialization/CloneArchive.hpp>
#include <FishEngine/CreateObject.hpp>

#include <map>

namespace FishEngine
{

	class CloneUtil
	{
	public:
		CloneUtil() = default;
		CloneUtil(CloneUtil&) = delete;
		CloneUtil& operator=(CloneUtil&) = delete;

		std::map<Object*, Object*> memo;
		std::list<Object*> todo;
	};


	Prefab* Prefab::Instantiate()
	{
		// first pass
		CollectObjectsArchive archive;
		archive.SerializeObject(this);

		std::map<Object*, Object*> memo;
		for (auto obj : archive.m_Objects)
		{
			int classID = obj->GetClassID();
			Object* cloned = CreateEmptyObjectByClassID(classID);
			memo[obj] = cloned;
		}

		auto instance = (Prefab*)memo[this];
		instance->m_IsPrefabParent = false;
		instance->m_ParentPrefab = this;
		return instance;
	}


	Prefab* Prefab::Instantiate(const PrefabModification& modification)
	{
		Prefab* instance = Instantiate();
		return instance;
	}
}