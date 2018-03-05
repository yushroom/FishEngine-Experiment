#pragma once

//#include "ReflectClass.hpp"
#include "../FishEngine.hpp"
#include <cstdint>

namespace FishEngine
{
	static constexpr int MaxBoneForEachVertex = 4;

	struct FE_EXPORT BoneWeight
	{
//		Meta(NonSerializable)
		int boneIndex[MaxBoneForEachVertex];

//		Meta(NonSerializable)
		float weight[MaxBoneForEachVertex];

		BoneWeight();

		void AddBoneData(uint32_t boneIndex, float weight);
	};
}
