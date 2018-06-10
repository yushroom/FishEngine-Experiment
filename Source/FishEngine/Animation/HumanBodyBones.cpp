#include <FishEngine/Animation/HumanBodyBones.hpp>
#include <FishEngine/Debug.hpp>
#include <string>
#include <algorithm>

using namespace FishEngine;


static const char* HumanBodyBonesStr[] = {"Hips", "LeftUpperLeg", "RightUpperLeg", "LeftLowerLeg", "RightLowerLeg", "LeftFoot", "RightFoot", "Spine", "Chest", "UpperChest", "Neck", "Head", "LeftShoulder", "RightShoulder", "LeftUpperArm", "RightUpperArm", "LeftLowerArm", "RightLowerArm", "LeftHand", "RightHand", "LeftToes", "RightToes", "LeftEye", "RightEye", "Jaw", "LeftThumbProximal", "LeftThumbIntermediate", "LeftThumbDistal", "LeftIndexProximal", "LeftIndexIntermediate", "LeftIndexDistal", "LeftMiddleProximal", "LeftMiddleIntermediate", "LeftMiddleDistal", "LeftRingProximal", "LeftRingIntermediate", "LeftRingDistal", "LeftLittleProximal", "LeftLittleIntermediate", "LeftLittleDistal", "RightThumbProximal", "RightThumbIntermediate", "RightThumbDistal", "RightIndexProximal", "RightIndexIntermediate", "RightIndexDistal", "RightMiddleProximal", "RightMiddleIntermediate", "RightMiddleDistal", "RightRingProximal", "RightRingIntermediate", "RightRingDistal", "RightLittleProximal", "RightLittleIntermediate", "RightLittleDistal"};

HumanBodyBones FishEngine::StringToHumanBodyBones(const std::string& name)
{
//	auto n = name.replace(' ', '');
	auto n = name;
	auto it = std::remove(n.begin(), n.end(), ' ');
	n.erase(it, name.end());
	int idx = 0;
	bool found = false;
	constexpr int size = static_cast<int>(HumanBodyBones::LastBone);
	for(int i = 0; i < size; ++i)
	{
		if (n == HumanBodyBonesStr[i])
		{
			found = true;
			break;
		}
		idx ++;
	}
	if (!found)
	{
		LogWarning("name not matched");
		return HumanBodyBones::LastBone;
	}
	return static_cast<HumanBodyBones>(idx);
}
