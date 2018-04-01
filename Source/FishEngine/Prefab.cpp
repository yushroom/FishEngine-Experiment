#include <FishEngine/Prefab.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/CloneObject.hpp>
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
		return dynamic_cast<Prefab*>( CloneObject(this) );
	}


	Prefab* Prefab::InstantiateWithModification(const PrefabModification& modification)
	{
		Prefab* instance = Instantiate();
		return instance;
	}
}