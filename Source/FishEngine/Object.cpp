#include <FishEngine/Object.hpp>

namespace FishEngine
{
    int Object::s_InstanceCounter = 0;
	int Object::s_DeleteCounter = 0;
	
	std::unordered_map<int, std::unordered_set<Object*>> Object::s_Objects;
	
	
}
