#include <FishEngine/Animation/Animator.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Animation/AnimationClip.hpp>
#include <FishEngine/Animation/AnimatorController.hpp>
#include <FishEngine/Animation/AnimationUtils.hpp>
//#include <FishEngine/Time.hpp>
#include <FishEngine/Animation/Avatar.hpp>
#include <FishEngine/Component/SkinnedMeshRenderer.hpp>

#include <deque>
#include <boost/algorithm/string.hpp>

using namespace FishEngine;

inline Transform* GetBone(Animator* animator, const std::string& path, HumanBodyBones boneId)
{
	if (animator->IsHumanBodyBonesMapped())
		return animator->GetBoneTransform(boneId);
	else
		return animator->GetBoneTransformByPath(path);
}

void ApplyAnimation(Animator* animator, AnimationClip* clip, float timer)
{
	if (clip == nullptr)
		return;
	if (animator->GetAvatar() == clip->m_avatar)
	{
		for (auto & curve : clip->m_positionCurve)
		{
			auto t = GetBone(animator, curve.path, curve.boneId);
			if (t != nullptr)
			{
				auto v = curve.curve.Evaluate(timer, true);
				t->SetLocalPosition(v);
			}
		}
		for (auto & curve : clip->m_rotationCurves)
		{
			auto t = GetBone(animator, curve.path, curve.boneId);
			if (t != nullptr)
			{
				auto v = curve.curve.Evaluate(timer, true);
				v.NormalizeSelf();
				t->SetLocalRotation(v);
			}
		}
		//	for (auto & curve : clip->m_eulersCurves)
		//	{
		//		auto t = GetBone(curve.path, skeleton);
		//		if (t != nullptr)
		//		{
		//			auto v = curve.curve.Evaluate(timer, true);
		//			assert(!(isnan(v.x) || isnan(v.y) || isnan(v.z)));
		//			t->SetLocalRotation(Quaternion::Euler(v));
		//		}
		//	}
		for (auto & curve : clip->m_scaleCurves)
		{
			auto t = GetBone(animator, curve.path, curve.boneId);
			if (t != nullptr)
			{
				auto v = curve.curve.Evaluate(timer, true);
				t->SetLocalScale(v);
			}
		}
	}
	else
	{
		for (auto & curve : clip->m_positionCurve)
		{
			auto t = GetBone(animator, curve.path, curve.boneId);
			if (t != nullptr)
			{
				auto v = curve.curve.Evaluate(timer, true);
				int id = (int)curve.boneId;
				auto clipBone = clip->m_avatar->m_HumanSkeleton[id];
				clipBone->SetLocalPosition(v);
			}
		}
		for (auto & curve : clip->m_rotationCurves)
		{
			auto t = GetBone(animator, curve.path, curve.boneId);
			if (t != nullptr)
			{
				auto v = curve.curve.Evaluate(timer, true);
				v.NormalizeSelf();
				int id = (int)curve.boneId;
				auto clipBone = clip->m_avatar->m_HumanSkeleton[id];
				clipBone->SetLocalRotation(v);
			}
		}
		
		constexpr int size = (int)HumanBodyBones::LastBone;
		for (int i = 0; i < size; ++i)
		{
			auto bone = animator->GetBoneTransform((HumanBodyBones)i);
			if (bone == nullptr)
			{
//				auto boneId = (HumanBodyBones)i;
				continue;
			}
			auto clipBone = clip->m_avatar->m_HumanSkeleton[i];
//			bone->SetWorldToLocalMatrix(clipBone->GetWorldToLocalMatrix());
			bone->SetLocalToWorldMatrix(clipBone->GetLocalToWorldMatrix());
//			auto pos1 = bone->GetPosition();
//			auto pos2 = clipBone->GetPosition();
//			assert(pos1 == pos2);
		}
	}
}

void ApplyAnimation2(Animator* animator, AnimationClip* clip1, AnimationClip* clip2,
					 float timer1, float timer2, float weight1, float weight2)
{
	assert(clip1 && clip2);
	
	{
		std::map<Transform*, Vector3> curve1;
		std::map<Transform*, Vector3> curve2;
		for (auto & curve : clip1->m_positionCurve)
		{
			auto t = GetBone(animator, curve.path, curve.boneId);
			if (t != nullptr)
			{
				curve1[t] = curve.curve.Evaluate(timer1, true);
			}
		}
		for (auto & curve : clip2->m_positionCurve)
		{
			auto t = GetBone(animator, curve.path, curve.boneId);
			if (t != nullptr)
			{
				curve2[t] = curve.curve.Evaluate(timer2, true);
			}
		}
		
		for (auto&& p : curve1)
		{
			auto t = p.first;
			auto it = curve2.find(t);
			auto pos1 = p.second;
			if (it == curve2.end())
			{
				auto pos = t->GetLocalPosition();
				pos += (pos1-pos) * weight1;
				t->SetLocalPosition(pos);
			}
			else
			{
				auto pos2 = it->second;
				curve2.erase(it);
				t->SetLocalPosition(pos1*weight1 + pos2*weight2);
			}
		}
		for (auto&& p : curve2)
		{
			auto t = p.first;
			auto pos2 = p.second;
			auto pos = t->GetLocalPosition();
			pos += (pos2-pos) * weight2;
			t->SetLocalPosition(pos);
		}
	}
	
	{
		std::map<Transform*, Quaternion> curve1;
		std::map<Transform*, Quaternion> curve2;
		for (auto & curve : clip1->m_rotationCurves)
		{
			auto t = GetBone(animator, curve.path, curve.boneId);
			if (t != nullptr)
			{
				curve1[t] = curve.curve.Evaluate(timer1, true);
			}
		}
		for (auto & curve : clip2->m_rotationCurves)
		{
			auto t = GetBone(animator, curve.path, curve.boneId);
			if (t != nullptr)
			{
				curve2[t] = curve.curve.Evaluate(timer2, true);
			}
		}
		
		for (auto&& p : curve1)
		{
			auto t = p.first;
			auto it = curve2.find(t);
			auto rot1 = p.second;
			Quaternion rot;
			if (it == curve2.end())
			{
				rot = t->GetLocalRotation();
			}
			else
			{
				rot = it->second;
				curve2.erase(it);
			}
			rot = Quaternion::Slerp(rot, rot1, weight1);
			t->SetLocalRotation(rot);
		}
		for (auto&& p : curve2)
		{
			auto t = p.first;
			auto rot2 = p.second;
			auto rot = t->GetLocalRotation();
			rot = Quaternion::Slerp(rot, rot2, weight1);
			t->SetLocalRotation(rot);
		}
	}
}




Transform* Animator::GetBoneTransformByPath(const std::string& path)
{
	auto it = m_Skeleton.find(path);
	if (it == m_Skeleton.end())
	{
		return nullptr;
	}
	return it->second;
}



std::vector<AnimatorClipInfo>
Animator::GetCurrentAnimatorClipInfo(int layerIndex)
{
	
}


void Animator::Start()
{
	auto t = this->GetTransform();
	m_SRenderer = this->GetGameObject()->GetComponentInChildren<SkinnedMeshRenderer>();
	
	if (m_Avatar != nullptr)
	{
		m_HumanBodyBonesMapped = !m_Avatar->m_BoneNameToHuman.empty();
		if (m_HumanBodyBonesMapped)
		{
			GetSkeleton(t, m_Skeleton2, m_Avatar->m_BoneNameToHuman);
		}
		else
		{
			GetSkeleton(t, "", m_Skeleton, m_Avatar->m_boneToIndex);
		}
	}
	
	if (m_Controller != nullptr)
		m_Controller->Reset();
}


void Animator::Update(float deltaTime)
{
	auto clipInfos = m_Controller->ApplyAnimation(deltaTime, this);
	
	if (clipInfos.size() == 1)
	{
		auto& i = clipInfos[0];
		m_SRenderer->SetOverrideAvatar(i.clip->m_avatar);
		ApplyAnimation(this, i.clip, i.time);
	}
	else if (clipInfos.size() == 2)
	{
		abort();
		auto& i = clipInfos[0];
		auto& i2 = clipInfos[1];
		ApplyAnimation2(this, i.clip, i2.clip, i.time, i2.time, i.weight, i2.weight);
	}
}
