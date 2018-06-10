#pragma once

#include <map>
#include <string>

#include "HumanBodyBones.hpp"

namespace FishEngine
{
	class Transform;
	
	void GetSkeleton(Transform*							t,
					 std::string const &				path,
					 std::map<std::string, Transform*>&	skeleton,
					 std::map<std::string, int> const&	boneToIndex);
	
	void GetSkeleton(Transform* t, Transform* skeleton[], std::map<std::string, HumanBodyBones> nameToId);
}
