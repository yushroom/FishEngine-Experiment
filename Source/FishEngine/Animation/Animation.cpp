#include <FishEngine/Animation/Animation.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/Animation/AnimationClip.hpp>
#include <FishEngine/Animation/AnimationUtils.hpp>
//#include <FishEngine/Time.hpp>
#include <FishEngine/Animation/Avatar.hpp>

#include <deque>
#include <boost/algorithm/string.hpp>

using namespace FishEngine;


void Animation::Start()
{
	if (m_clip == nullptr)
		return;
	auto t = this->GetTransform();
	GetSkeleton(t, "", m_skeleton, m_clip->m_avatar->m_boneToIndex);
}

inline Transform* GetBone(std::string const & path, std::map<std::string, Transform*> const & skeleton)
{
	auto it = skeleton.find(path);
	if (it == skeleton.end())
	{
		//abort();
		//LogWarning(Format("Bone [{}] not found\n", path));
		return nullptr;
	}
	return it->second;
}

void Animation::Update(float deltaTime)
{
	if (m_clip == nullptr)
		return;
	m_localTimer += deltaTime;
	for (auto & curve : m_clip->m_positionCurve)
	{
		auto t = GetBone(curve.path, m_skeleton);
		if (t != nullptr)
		{
			auto v = curve.curve.Evaluate(m_localTimer, true);
			t->SetLocalPosition(v);
		}
	}
	//for (auto & curve : m_clip->m_rotationCurves)
	//{
	//	auto t = GetBone(curve.path, m_skeleton);
	//	if (t != nullptr)
	//	{
	//		auto v = curve.curve.Evaluate(m_localTimer, true);
	//		v.NormalizeSelf();
	//		t->SetLocalRotation(v);
	//	}
	//}
	for (auto & curve : m_clip->m_eulersCurves)
	{
		auto t = GetBone(curve.path, m_skeleton);
		if (t != nullptr)
		{
			auto v = curve.curve.Evaluate(m_localTimer, true);
			assert(!(isnan(v.x) || isnan(v.y) || isnan(v.z)));
			t->SetLocalRotation(Quaternion::Euler(RotationOrder::XYZ, v));
		}
	}
	for (auto & curve : m_clip->m_scaleCurves)
	{
		auto t = GetBone(curve.path, m_skeleton);
		if (t != nullptr)
		{
			auto v = curve.curve.Evaluate(m_localTimer, true);
			t->SetLocalScale(v);
		}
	}
}
