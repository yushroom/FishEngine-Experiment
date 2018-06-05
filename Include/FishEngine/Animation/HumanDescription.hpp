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
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
	};

	struct HumanDescription
	{
	public:
		std::vector<HumanBone> human;
		std::vector<SkeletonBone> skeleton;
	};
}