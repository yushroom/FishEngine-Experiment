#pragma once

#include <map>
#include <string>

namespace FishEngine
{
	class Transform;
	
	void GetSkeleton(Transform*							t,
					 std::string const &				path,
					 std::map<std::string, Transform*>&	skeleton,
					 std::map<std::string, int> const&	boneToIndex);
}
