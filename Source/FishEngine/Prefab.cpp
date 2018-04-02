#include <FishEngine/Prefab.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/CloneObject.hpp>
#include <FishEngine/CreateObject.hpp>

#include <FishEngine/Serialization/UpdateValueArchive.hpp>

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
		std::map<Object*, Object*> memo;
		auto cloned = dynamic_cast<Prefab*>( CloneObject(this, memo) );
		for (auto&& p : m_FileIDToObject)
		{
			auto fileID = p.first;
			auto origin = p.second;
			auto cloned_ = memo[origin];
			cloned->m_FileIDToObject[fileID] = cloned_;
		}
		return cloned;
	}


	Prefab* Prefab::InstantiateWithModification(const PrefabModification& modification)
	{
//		Prefab* instance = Instantiate();
		std::map<Object*, Object*> memo;
		auto cloned = dynamic_cast<Prefab*>( CloneObject(this, memo) );
		for (auto&& p : m_FileIDToObject)
		{
			auto fileID = p.first;
			auto origin = p.second;
			auto cloned_ = memo[origin];
			cloned->m_FileIDToObject[fileID] = cloned_;
		}
//		return cloned;

		UpdateValueArchive archive;
		cloned->GetRootGameObject()->GetTransform()->SetParent( modification.m_TransformParent, false);
		cloned->m_Modification = modification;
//		for (auto& mod : modification.m_Modifications )
		for (int i = 0; i < modification.m_Modifications.size(); ++i)
		{
			auto& mod = modification.m_Modifications[i];
			assert(mod.target != nullptr);
			Object * target =  memo[mod.target];
			archive.UpdateValue(target, mod.propertyPath, mod.value, mod.objectReference);
			cloned->m_Modification.m_Modifications[i].target = target;
		}
		assert(modification.m_RemovedComponents.empty());
		return cloned;
	}
}