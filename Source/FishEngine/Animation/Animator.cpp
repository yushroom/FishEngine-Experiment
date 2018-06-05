#include <FishEngine/Animation/Animator.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/Animation/AnimationClip.hpp>
#include <FishEngine/Animation/AnimatorController.hpp>
#include <FishEngine/Animation/AnimationUtils.hpp>
//#include <FishEngine/Time.hpp>
#include <FishEngine/Animation/Avatar.hpp>

#include <deque>
#include <boost/algorithm/string.hpp>


using namespace FishEngine;


void Animator::Start()
{
	auto t = this->GetTransform();
	GetSkeleton(t, "", m_Skeleton, m_Avatar->m_boneToIndex);
}

inline Transform* GetBone(std::string const & path, std::map<std::string, Transform*> const & skeleton)
{
	auto it = skeleton.find(path);
	if (it == skeleton.end())
	{
		//LogWarning(Format("Bone [{}] not found\n", path));
		return nullptr;
	}
	return it->second;
}

void Animator::Update(float deltaTime)
{
	auto clip = m_Controller->GetCurrentAnimationClip();
	if (clip == nullptr)
		return;
	m_localTimer += deltaTime;
	for (auto & curve : clip->m_positionCurve)
	{
		auto t = GetBone(curve.path, m_Skeleton);
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
	for (auto & curve : clip->m_eulersCurves)
	{
		auto t = GetBone(curve.path, m_Skeleton);
		if (t != nullptr)
		{
			auto v = curve.curve.Evaluate(m_localTimer, true);
			assert(!(isnan(v.x) || isnan(v.y) || isnan(v.z)));
			t->SetLocalRotation(Quaternion::Euler(RotationOrder::XYZ, v));
		}
	}
	for (auto & curve : clip->m_scaleCurves)
	{
		auto t = GetBone(curve.path, m_Skeleton);
		if (t != nullptr)
		{
			auto v = curve.curve.Evaluate(m_localTimer, true);
			t->SetLocalScale(v);
		}
	}
}


#if 0

void Animator::Update() {
	if (!m_playing)
		return;
	m_time += Time::deltaTime();
	if (m_time * m_animation->ticksPerSecond > m_animation->duration) {
		if (m_playingOnce) {
			Stop();
		}
		m_time -= m_animation->duration / m_animation->ticksPerSecond;
		return;
	}

	RecursivelyUpdate(gameObject());

	if (m_playOneFrame) {
		m_playing = false;
		m_playOneFrame = false;
	}
}

void Animator::RecursivelyUpdate(const GameObjectPtr& go)
{
	auto name = go->name();
	auto t = go->transform();
	auto it = m_animation->channels.find(name);
	auto tm = m_time * m_animation->ticksPerSecond;
	float time = fmodf(tm, m_animation->duration);
	if (it != m_animation->channels.end()) {
		auto& node = it->second;

		///////////////////////////////////////////////
		Vector3 presentPosition(0, 0, 0);
		if (node.positionKeys.size() > 0) 
		{
			uint32_t frame = 0;
			while (frame < node.positionKeys.size() - 1)
			{
				if (time < node.positionKeys[frame + 1].time)
					break;
				frame++;
			}
			uint32_t nextFrame = (frame + 1) % node.positionKeys.size();
			const auto& key = node.positionKeys[frame];
			const auto& nextKey = node.positionKeys[nextFrame];
			float diffTime = nextKey.time - key.time;
			if (diffTime < 0.0f)
			{
				diffTime += m_animation->duration;
			}
			if (diffTime > 0)
			{
				float factor = float((time - key.time) / diffTime);
				presentPosition = Mathf::LerpUnclamped(key.value, nextKey.value, factor);
			}
			else
			{
				presentPosition = key.value;
			}
		}

		/////////////////////////////////////////////////////////////
		auto presentRotation = Quaternion::identity;
		if (node.rotationKeys.size() > 0)
		{
			uint32_t frame = 0;
			while (frame < node.rotationKeys.size() - 1)
			{
				if (time < node.rotationKeys[frame + 1].time)
					break;
				frame++;
			}
			uint32_t nextFrame = (frame + 1) % node.rotationKeys.size();
			const auto& key = node.rotationKeys[frame];
			const auto& nextKey = node.rotationKeys[nextFrame];
			float diffTime = nextKey.time - key.time;
			if (diffTime < 0.0f)
			{
				diffTime += m_animation->duration;
			}
			if (diffTime > 0)
			{
				float factor = float((time - key.time) / diffTime);
				presentRotation = Quaternion::SlerpUnclamped(key.value, nextKey.value, factor);
			}
			else
			{
				presentRotation = key.value;
			}
		}

		/////////////////////////////////////////////////////////////
		auto presentScaling = Vector3::one;
		if (node.scalingKeys.size() > 0)
		{
			uint32_t frame = 0;
			while (frame < node.scalingKeys.size() - 1)
			{
				if (time < node.scalingKeys[frame + 1].time)
					break;
				frame++;
			}
			// TODO: LERP scale?
			presentScaling = node.scalingKeys[frame].value;
		}

#if 1
		t->setLocalPosition(presentPosition);
		t->setLocalRotation(presentRotation);
		t->setLocalScale(presentScaling);
#else
		Matrix4x4 m;
		uint32_t j = 0;
		for (j = 1; j < node.transformationKeys.size() && tm > node.transformationKeys[j].time; ++j) {}
		m = node.transformationKeys[j - 1].value;
		t->setLocalToWorldMatrix(m);
#endif
	}
	for (auto child : t->children())
	{
		RecursivelyUpdate(child->gameObject());
	}
}

void FishEngine::Animator::Play()
{
	m_playing = true;
	m_playingOnce = false;
	m_playOneFrame = false;
}

void FishEngine::Animator::PlayOnce()
{
	m_playing = true;
	m_playingOnce = true;
	m_playOneFrame = false;
}

void FishEngine::Animator::NextFrame()
{
	m_playing = true;
	m_playingOnce = false;
	m_playOneFrame = true;
	//m_currentFrame++;
	//RecursivelyUpdate2(gameObject());
}

//void FishEngine::Animator::RecursivelyUpdate2(const std::shared_ptr<GameObject>& go)
//{
//    auto name = go->name();
//    auto t = go->transform();
//    auto it = m_animation->channels.find(name);
//    //auto tm = m_time * m_animation->ticksPerSecond;
//    if (it != m_animation->channels.end()) {
//        auto& node = it->second;
//        t->setLocalToWorldMatrix(node.transformationKeys[m_currentFrame].value);
//    }
//
//    for (auto child : t->children()) {
//        RecursivelyUpdate2(child.lock()->gameObject());
//    }
//}

#endif
