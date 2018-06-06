#include <FishEngine/Animation/AnimatorController.hpp>
#include <FishEngine/Transform.hpp>

using namespace FishEngine;
using namespace FishEditor;
using namespace FishEditor::Animations;

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

void ApplyAnimation(AnimationClip* clip, float timer, std::map<std::string, Transform*> const & skeleton)
{
	if (clip == nullptr)
		return;
	for (auto & curve : clip->m_positionCurve)
	{
		auto t = GetBone(curve.path, skeleton);
		if (t != nullptr)
		{
			auto v = curve.curve.Evaluate(timer, true);
			t->SetLocalPosition(v);
		}
	}
	for (auto & curve : clip->m_rotationCurves)
	{
		auto t = GetBone(curve.path, skeleton);
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
		auto t = GetBone(curve.path, skeleton);
		if (t != nullptr)
		{
			auto v = curve.curve.Evaluate(timer, true);
			t->SetLocalScale(v);
		}
	}
}

void ApplyAnimation2(AnimationClip* clip1, AnimationClip* clip2,
					 float timer1, float timer2, float weight1, float weight2,
					 std::map<std::string, Transform*> const & skeleton)
{
	assert(clip1 && clip2);
	
#if 1
	{
	std::map<Transform*, Vector3> curve1;
	std::map<Transform*, Vector3> curve2;
	for (auto & curve : clip1->m_positionCurve)
	{
		auto t = GetBone(curve.path, skeleton);
		if (t != nullptr)
		{
			curve1[t] = curve.curve.Evaluate(timer1, true);
		}
	}
	for (auto & curve : clip2->m_positionCurve)
	{
		auto t = GetBone(curve.path, skeleton);
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
			auto t = GetBone(curve.path, skeleton);
			if (t != nullptr)
			{
				curve1[t] = curve.curve.Evaluate(timer1, true);
			}
		}
		for (auto & curve : clip2->m_rotationCurves)
		{
			auto t = GetBone(curve.path, skeleton);
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
	
#else
	
	auto clip = clip1;
	float weight = weight1;
	float timer = timer1;
	for (auto & curve : clip->m_positionCurve)
	{
		auto t = GetBone(curve.path, skeleton);
		if (t != nullptr)
		{
//			auto p = t->GetLocalPosition();
			auto v = curve.curve.Evaluate(timer, true);
//			auto diff = v - p;
//			p += diff*weight;
//			t->SetLocalPosition(p);
			t->SetLocalPosition(v*weight);
		}
	}
	for (auto & curve : clip->m_rotationCurves)
	{
		auto t = GetBone(curve.path, skeleton);
		if (t != nullptr)
		{
			auto p = t->GetLocalRotation();
			auto v = curve.curve.Evaluate(timer, true);
			assert(!(isnan(v.x) || isnan(v.y) || isnan(v.z)));
			auto diff = v - p;
			p += diff*weight;
			t->SetLocalRotation(p);
		}
	}
	
	clip = clip2;
	weight = weight2;
	timer = timer2;
	for (auto & curve : clip->m_positionCurve)
	{
		auto t = GetBone(curve.path, skeleton);
		if (t != nullptr)
		{
//			auto p = t->GetLocalPosition();
			auto v = curve.curve.Evaluate(timer, true);
//			auto diff = v - p;
//			p += diff*weight;
//			t->SetLocalPosition(p);
			t->SetLocalPosition(v*weight);
		}
	}
	for (auto & curve : clip->m_rotationCurves)
	{
		auto t = GetBone(curve.path, skeleton);
		if (t != nullptr)
		{
			auto p = t->GetLocalRotation();
			auto v = curve.curve.Evaluate(timer, true);
			assert(!(isnan(v.x) || isnan(v.y) || isnan(v.z)));
			auto diff = v - p;
			p += diff*weight;
			t->SetLocalRotation(p);
		}
	}

//	for (auto & curve : clip->m_eulersCurves)
//	{
//		auto t = GetBone(curve.path, skeleton);
//		if (t != nullptr)
//		{
//			auto v = curve.curve.Evaluate(timer, true);
//			assert(!(isnan(v.x) || isnan(v.y) || isnan(v.z)));
//			t->SetLocalRotation(Quaternion::Euler(RotationOrder::XYZ, v));
//		}
//	}
//	for (auto & curve : clip->m_scaleCurves)
//	{
//		auto t = GetBone(curve.path, skeleton);
//		if (t != nullptr)
//		{
//			auto v = curve.curve.Evaluate(timer, true);
//			t->SetLocalScale(v);
//		}
//	}
#endif
}

void FishEditor::Animations::AnimatorController::ApplyAnimation(float deltaTime, std::map<std::string, Transform*> const & skeleton)
{
	auto layer0 = m_AnimatorLayers.front();
	auto stateMachine = layer0.m_StateMachine;
	auto& state = stateMachine->m_CurrentState;
	if (state == nullptr)
	{
		state = stateMachine->m_DefaultState;
		state->m_LocalPlayTimer = 0;
	}
	auto transition = state->m_Transitions[0];
	state->m_LocalPlayTimer += deltaTime;
	float timer = state->m_LocalPlayTimer;
	float exitTime = transition->m_ExitTime * state->m_Motion->As<AnimationClip>()->length;
	AnimatorState* state2 = nullptr;
	float weight1=1, weight2=0;
	if (timer > exitTime)
	{
		float startTime = state->m_LocalPlayTimer - exitTime;
		if (timer >= exitTime + transition->m_TransitionDuration)
		{
			state2 = nullptr;
			state = transition->m_DstState;
			state->m_LocalPlayTimer = startTime;
		}
		else	// blend
		{
			state2 = transition->m_DstState;
			state2->m_LocalPlayTimer = startTime;
			weight2 = startTime / transition->m_TransitionDuration;
			weight1 = 1 - weight2;
		}
	}
	
	if (state2 == nullptr)
	{
		auto clip = state->m_Motion->As<AnimationClip>();
		::ApplyAnimation(clip, state->m_LocalPlayTimer, skeleton);
	}
	else
	{
		auto clip1 = state->m_Motion->As<AnimationClip>();
		auto clip2 = state2->m_Motion->As<AnimationClip>();
		::ApplyAnimation2(clip1, clip2, state->m_LocalPlayTimer, state2->m_LocalPlayTimer, weight1, weight2, skeleton);
	}
}
