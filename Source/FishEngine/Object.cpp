#include <FishEngine/Object.hpp>
#include <FishEngine/Scene.hpp>

namespace FishEngine
{
    int Object::s_InstanceCounter = 0;
	int Object::s_DeleteCounter = 0;
	
	std::unordered_map<int, std::unordered_set<Object*>> Object::s_Objects;
	
	std::vector<Object*> Object::FindObjectsOfType(int classID)
	{
		std::vector<Object*> ret;
		auto scene = SceneManager::GetActiveScene();
		const auto& objs = s_Objects[classID];
		ret.reserve(objs.size());
		for (auto o : objs)
		{
			ret.push_back(o);
		}
		return ret;
	}
}
