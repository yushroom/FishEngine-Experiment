#include <FishEngine/Object.hpp>

namespace FishEngine
{
    int Object::s_instanceCounter = 0;
	int Object::s_deleteCounter = 0;
	
	std::unordered_map<int, std::unordered_set<Object*>> Object::s_objects;
	
	
}
