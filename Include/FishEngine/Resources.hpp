#pragma once

#include <unordered_map>
#include <unordered_set>

namespace FishEngine
{
	class Object;
	
	class Resources
	{
	public:
		Resources() = delete;
		
		static void AddObjectOfType(int classID);
		static void FindObjectsOfType(int classID);
		
		static std::unordered_map<int, std::unordered_set<Object*>> s_objects;
	};
}
