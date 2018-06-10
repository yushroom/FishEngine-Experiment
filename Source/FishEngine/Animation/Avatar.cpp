#include <FishEngine/Animation/Avatar.hpp>
#include <FishEngine/GameObject.hpp>

#include <deque>

using namespace FishEngine;

#if 0
void Avatar::GetHumanSkeleton()
{
	constexpr int boneSize = static_cast<int>(HumanBodyBones::LastBone);
	auto bones = m_HumanSkeleton;
	for (int i = 0; i < boneSize; ++i)
	{
		auto go = new GameObject();
		bones[i] = go->GetTransform();
	}
	
	auto Hips = bones[(int)HumanBodyBones::Hips];
	Hips->GetGameObject()->SetName("Hips");
	
	auto LeftUpperLeg = bones[(int)HumanBodyBones::LeftUpperLeg];
	LeftUpperLeg->GetGameObject()->SetName("LeftUpperLeg");
	LeftUpperLeg->SetParent(Hips);
	
	auto RightUpperLeg = bones[(int)HumanBodyBones::RightUpperLeg];
	RightUpperLeg->GetGameObject()->SetName("RightUpperLeg");
	RightUpperLeg->SetParent(Hips);
	
	auto LeftLowerLeg = bones[(int)HumanBodyBones::LeftLowerLeg];
	LeftLowerLeg->GetGameObject()->SetName("LeftLowerLeg");
	LeftLowerLeg->SetParent(LeftUpperLeg);
	
	auto RightLowerLeg = bones[(int)HumanBodyBones::RightLowerLeg];
	RightLowerLeg->GetGameObject()->SetName("RightLowerLeg");
	RightLowerLeg->SetParent(RightUpperLeg);
	
	auto LeftFoot = bones[(int)HumanBodyBones::LeftFoot];
	LeftFoot->GetGameObject()->SetName("LeftFoot");
	LeftFoot->SetParent(LeftLowerLeg);
	
	auto RightFoot = bones[(int)HumanBodyBones::RightFoot];
	RightFoot->GetGameObject()->SetName("RightFoot");
	RightFoot->SetParent(RightLowerLeg);
	
	auto Spine = bones[(int)HumanBodyBones::Spine];
	Spine->GetGameObject()->SetName("Spine");
	Spine->SetParent(Hips);
	
	auto Chest = bones[(int)HumanBodyBones::Chest];
	Chest->GetGameObject()->SetName("Chest");
	Chest->SetParent(Spine);
	
	auto UpperChest = bones[(int)HumanBodyBones::UpperChest];
	UpperChest->GetGameObject()->SetName("UpperChest");
	UpperChest->SetParent(Chest);
	
	auto Neck = bones[(int)HumanBodyBones::Neck];
	Neck->GetGameObject()->SetName("Neck");
	Neck->SetParent(UpperChest);
	
	auto Head = bones[(int)HumanBodyBones::Head];
	Head->GetGameObject()->SetName("Head");
	Head->SetParent(Neck);
	
	auto LeftShoulder = bones[(int)HumanBodyBones::LeftShoulder];
	LeftShoulder->GetGameObject()->SetName("LeftShoulder");
	LeftShoulder->SetParent(UpperChest);
	
	auto RightShoulder = bones[(int)HumanBodyBones::RightShoulder];
	RightShoulder->GetGameObject()->SetName("RightShoulder");
	RightShoulder->SetParent(UpperChest);
	
	auto LeftUpperArm = bones[(int)HumanBodyBones::LeftUpperArm];
	LeftUpperArm->GetGameObject()->SetName("LeftUpperArm");
	LeftUpperArm->SetParent(LeftShoulder);
	
	auto RightUpperArm = bones[(int)HumanBodyBones::RightUpperArm];
	RightUpperArm->GetGameObject()->SetName("RightUpperArm");
	RightUpperArm->SetParent(RightShoulder);
	
	auto LeftLowerArm = bones[(int)HumanBodyBones::LeftLowerArm];
	LeftLowerArm->GetGameObject()->SetName("LeftLowerArm");
	LeftLowerArm->SetParent(LeftUpperArm);
	
	auto RightLowerArm = bones[(int)HumanBodyBones::RightLowerArm];
	RightLowerArm->GetGameObject()->SetName("RightLowerArm");
	RightLowerArm->SetParent(RightUpperArm);
	
	auto LeftHand = bones[(int)HumanBodyBones::LeftHand];
	LeftHand->GetGameObject()->SetName("LeftHand");
	LeftHand->SetParent(LeftLowerArm);
	
	auto RightHand = bones[(int)HumanBodyBones::RightHand];
	RightHand->GetGameObject()->SetName("RightHand");
	RightHand->SetParent(RightLowerArm);
	
	auto LeftToes = bones[(int)HumanBodyBones::LeftToes];
	LeftToes->GetGameObject()->SetName("LeftToes");
	LeftToes->SetParent(LeftFoot);
	
	auto RightToes = bones[(int)HumanBodyBones::RightToes];
	RightToes->GetGameObject()->SetName("RightToes");
	RightToes->SetParent(RightFoot);
	
	auto LeftEye = bones[(int)HumanBodyBones::LeftEye];
	LeftEye->GetGameObject()->SetName("LeftEye");
	LeftEye->SetParent(Head);
	
	auto RightEye = bones[(int)HumanBodyBones::RightEye];
	RightEye->GetGameObject()->SetName("RightEye");
	RightEye->SetParent(Head);
	
	auto Jaw = bones[(int)HumanBodyBones::Jaw];
	Jaw->GetGameObject()->SetName("Jaw");
	Jaw->SetParent(Head);
	
	auto LeftThumbProximal = bones[(int)HumanBodyBones::LeftThumbProximal];
	LeftThumbProximal->GetGameObject()->SetName("LeftThumbProximal");
	LeftThumbProximal->SetParent(LeftHand);
	
	auto LeftThumbIntermediate = bones[(int)HumanBodyBones::LeftThumbIntermediate];
	LeftThumbIntermediate->GetGameObject()->SetName("LeftThumbIntermediate");
	LeftThumbIntermediate->SetParent(LeftThumbProximal);
	
	auto LeftThumbDistal = bones[(int)HumanBodyBones::LeftThumbDistal];
	LeftThumbDistal->GetGameObject()->SetName("LeftThumbDistal");
	LeftThumbDistal->SetParent(LeftThumbIntermediate);
	
	auto LeftIndexProximal = bones[(int)HumanBodyBones::LeftIndexProximal];
	LeftIndexProximal->GetGameObject()->SetName("LeftIndexProximal");
	LeftIndexProximal->SetParent(LeftHand);
	
	auto LeftIndexIntermediate = bones[(int)HumanBodyBones::LeftIndexIntermediate];
	LeftIndexIntermediate->GetGameObject()->SetName("LeftIndexIntermediate");
	LeftIndexIntermediate->SetParent(LeftIndexProximal);
	
	auto LeftIndexDistal = bones[(int)HumanBodyBones::LeftIndexDistal];
	LeftIndexDistal->GetGameObject()->SetName("LeftIndexDistal");
	LeftIndexDistal->SetParent(LeftIndexIntermediate);
	
	auto LeftMiddleProximal = bones[(int)HumanBodyBones::LeftMiddleProximal];
	LeftMiddleProximal->GetGameObject()->SetName("LeftMiddleProximal");
	LeftMiddleProximal->SetParent(LeftHand);
	
	auto LeftMiddleIntermediate = bones[(int)HumanBodyBones::LeftMiddleIntermediate];
	LeftMiddleIntermediate->GetGameObject()->SetName("LeftMiddleIntermediate");
	LeftMiddleIntermediate->SetParent(LeftMiddleProximal);
	
	auto LeftMiddleDistal = bones[(int)HumanBodyBones::LeftMiddleDistal];
	LeftMiddleDistal->GetGameObject()->SetName("LeftMiddleDistal");
	LeftMiddleDistal->SetParent(LeftMiddleIntermediate);
	
	auto LeftRingProximal = bones[(int)HumanBodyBones::LeftRingProximal];
	LeftRingProximal->GetGameObject()->SetName("LeftRingProximal");
	LeftRingProximal->SetParent(LeftHand);
	
	auto LeftRingIntermediate = bones[(int)HumanBodyBones::LeftRingIntermediate];
	LeftRingIntermediate->GetGameObject()->SetName("LeftRingIntermediate");
	LeftRingIntermediate->SetParent(LeftRingProximal);
	
	auto LeftRingDistal = bones[(int)HumanBodyBones::LeftRingDistal];
	LeftRingDistal->GetGameObject()->SetName("LeftRingDistal");
	LeftRingDistal->SetParent(LeftRingIntermediate);
	
	auto LeftLittleProximal = bones[(int)HumanBodyBones::LeftLittleProximal];
	LeftLittleProximal->GetGameObject()->SetName("LeftLittleProximal");
	LeftLittleProximal->SetParent(LeftHand);
	
	auto LeftLittleIntermediate = bones[(int)HumanBodyBones::LeftLittleIntermediate];
	LeftLittleIntermediate->GetGameObject()->SetName("LeftLittleIntermediate");
	LeftLittleIntermediate->SetParent(LeftLittleProximal);
	
	auto LeftLittleDistal = bones[(int)HumanBodyBones::LeftLittleDistal];
	LeftLittleDistal->GetGameObject()->SetName("LeftLittleDistal");
	LeftLittleDistal->SetParent(LeftLittleIntermediate);
	
	auto RightThumbProximal = bones[(int)HumanBodyBones::RightThumbProximal];
	RightThumbProximal->GetGameObject()->SetName("RightThumbProximal");
	RightThumbProximal->SetParent(RightHand);
	
	auto RightThumbIntermediate = bones[(int)HumanBodyBones::RightThumbIntermediate];
	RightThumbIntermediate->GetGameObject()->SetName("RightThumbIntermediate");
	RightThumbIntermediate->SetParent(RightThumbProximal);
	
	auto RightThumbDistal = bones[(int)HumanBodyBones::RightThumbDistal];
	RightThumbDistal->GetGameObject()->SetName("RightThumbDistal");
	RightThumbDistal->SetParent(RightThumbIntermediate);
	
	auto RightIndexProximal = bones[(int)HumanBodyBones::RightIndexProximal];
	RightIndexProximal->GetGameObject()->SetName("RightIndexProximal");
	RightIndexProximal->SetParent(RightHand);
	
	auto RightIndexIntermediate = bones[(int)HumanBodyBones::RightIndexIntermediate];
	RightIndexIntermediate->GetGameObject()->SetName("RightIndexIntermediate");
	RightIndexIntermediate->SetParent(RightIndexProximal);
	
	auto RightIndexDistal = bones[(int)HumanBodyBones::RightIndexDistal];
	RightIndexDistal->GetGameObject()->SetName("RightIndexDistal");
	RightIndexDistal->SetParent(RightIndexIntermediate);
	
	auto RightMiddleProximal = bones[(int)HumanBodyBones::RightMiddleProximal];
	RightMiddleProximal->GetGameObject()->SetName("RightMiddleProximal");
	RightMiddleProximal->SetParent(RightHand);
	
	auto RightMiddleIntermediate = bones[(int)HumanBodyBones::RightMiddleIntermediate];
	RightMiddleIntermediate->GetGameObject()->SetName("RightMiddleIntermediate");
	RightMiddleIntermediate->SetParent(RightMiddleProximal);
	
	auto RightMiddleDistal = bones[(int)HumanBodyBones::RightMiddleDistal];
	RightMiddleDistal->GetGameObject()->SetName("RightMiddleDistal");
	RightMiddleDistal->SetParent(RightMiddleIntermediate);
	
	auto RightRingProximal = bones[(int)HumanBodyBones::RightRingProximal];
	RightRingProximal->GetGameObject()->SetName("RightRingProximal");
	RightRingProximal->SetParent(RightHand);
	
	auto RightRingIntermediate = bones[(int)HumanBodyBones::RightRingIntermediate];
	RightRingIntermediate->GetGameObject()->SetName("RightRingIntermediate");
	RightRingIntermediate->SetParent(RightRingProximal);
	
	auto RightRingDistal = bones[(int)HumanBodyBones::RightRingDistal];
	RightRingDistal->GetGameObject()->SetName("RightRingDistal");
	RightRingDistal->SetParent(RightRingIntermediate);
	
	auto RightLittleProximal = bones[(int)HumanBodyBones::RightLittleProximal];
	RightLittleProximal->GetGameObject()->SetName("RightLittleProximal");
	RightLittleProximal->SetParent(RightHand);
	
	auto RightLittleIntermediate = bones[(int)HumanBodyBones::RightLittleIntermediate];
	RightLittleIntermediate->GetGameObject()->SetName("RightLittleIntermediate");
	RightLittleIntermediate->SetParent(RightLittleProximal);
	
	auto RightLittleDistal = bones[(int)HumanBodyBones::RightLittleDistal];
	RightLittleDistal->GetGameObject()->SetName("RightLittleDistal");
	RightLittleDistal->SetParent(RightLittleIntermediate);
}

#endif


template<typename T, typename B>
B GetValueOrDefault(const std::map<T, B> & dict, const T & key, const B& defaultValue)
{
	auto it = dict.find(key);
	if (it != dict.end())
		return it->second;
	return defaultValue;
}


Avatar* Avatar::BuildHumanAvatar(GameObject* go, const HumanDescription& humanDescription)
{
	constexpr int BoneSize = static_cast<int>(HumanBodyBones::LastBone);
	auto avatar = new Avatar;
	avatar->m_HumanDescription = humanDescription;
	avatar->m_IsHuman = true;
//	avatar->m_BindPoses2.resize(boneSize);
//	for (auto& m : avatar->m_BindPoses2)
//	{
//		m = Matrix4x4::identity;
//	}

	for (auto&& h : humanDescription.human)
	{
		auto boneId = StringToHumanBodyBones(h.humanName);
		avatar->m_BoneNameToHuman[h.boneName] = boneId;
	}
	

	std::deque<Transform*> todo;
	todo.push_back(go->GetTransform());
	while (!todo.empty())
	{
		auto t = todo.front();
		todo.pop_front();
		auto name = t->GetName();
		auto boneId = GetValueOrDefault( avatar->m_BoneNameToHuman, name, HumanBodyBones::LastBone );
		if (boneId != HumanBodyBones::LastBone)
		{
			int id = static_cast<int>(boneId);
			avatar->m_HumanSkeleton[id] = t;
		}
	
		for (auto c : t->GetChildren())
		{
			todo.push_back(c);
		}
	}
	
	for (auto&& b : humanDescription.skeleton)
	{
		auto boneId = GetValueOrDefault( avatar->m_BoneNameToHuman, b.name, HumanBodyBones::LastBone );
		if (boneId != HumanBodyBones::LastBone)
		{
			int id = static_cast<int>(boneId);
			auto t = avatar->m_HumanSkeleton[id];
			t->SetLocalPosition( b.position );
			t->SetLocalRotation( b.rotation );
			t->SetLocalScale( b.scale );
			avatar->m_HumanSkeletonBones[id] = b;
		}
	}
	
//	for (int i = 0; i < boneSize; ++i)
//	{
//		auto b = avatar->m_HumanSkeleton[i];
//		if (b != nullptr)
//			avatar->m_BindPoses2[i] = b->GetWorldToLocalMatrix();
//	}
	
	return avatar;
}
