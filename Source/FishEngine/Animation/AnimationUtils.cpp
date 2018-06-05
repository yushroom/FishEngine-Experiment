#include <FishEngine/Animation/AnimationUtils.hpp>
#include <FishEngine/Transform.hpp>

namespace FishEngine
{
	void GetSkeleton(
		Transform*								t,
		std::string const &						path,
		std::map<std::string, Transform*>&		skeleton,
		std::map<std::string, int> const &		boneToIndex)
	{
		std::string current_path;
		if (path != "")
		{
			current_path = path + "/";
		}

		auto name = t->GetName();
		if (boneToIndex.find(name) != boneToIndex.end())
			current_path += name;

		if (current_path != "")
			skeleton[current_path] = t;
		for (auto & child : t->GetChildren())
		{
			GetSkeleton(child, current_path, skeleton, boneToIndex);
		}
	}
}
