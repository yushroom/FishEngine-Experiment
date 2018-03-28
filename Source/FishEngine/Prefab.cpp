#include <FishEngine/Prefab.hpp>
#include <FishEngine/GameObject.hpp>

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

	FishEngine::Prefab* Prefab::Instantiate()
	{
		auto instance = new Prefab();
		instance->m_IsPrefabParent = false;
		instance->m_ParentPrefab = this;

		std::map<Object*, Object*> memo;
		memo[this] = instance;
		instance->m_RootGameObject = m_RootGameObject->Clone();
		return instance;
	}
}