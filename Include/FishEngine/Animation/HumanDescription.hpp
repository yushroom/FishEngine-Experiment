#pragma once

#include <string>
#include <vector>
#include "../Math/Vector3.hpp"
#include "../Math/Quaternion.hpp"

namespace FishEngine
{
	struct HumanLimit
	{
	public:

	//private:
		Vector3 m_Min;
		Vector3 m_Max;
		Vector3 m_Center;
		float m_AxisLength;
		bool m_UseDefaultValues = false;
	};

	struct HumanBone
	{
	//private:
		std::string boneName;
		std::string humanName;
		HumanLimit limit;
	};

	struct SkeletonBone
	{
	public:
		std::string name;
		Vector3 position;		// The T-pose position of the bone in local space.
		Quaternion rotation;	// The T-pose rotation of the bone in local space.
		Vector3 scale;			// The T-pose scaling of the bone in local space.
	};

	struct HumanDescription
	{
	public:
		std::vector<HumanBone> human;
		std::vector<SkeletonBone> skeleton;
	};
}
