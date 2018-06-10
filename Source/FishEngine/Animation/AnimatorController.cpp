#include <FishEngine/Animation/AnimatorController.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/Animation/Animator.hpp>

using namespace FishEngine;
using namespace FishEditor;
using namespace FishEditor::Animations;


std::vector<AnimatorClipInfo>
FishEditor::Animations::AnimatorController::ApplyAnimation(float deltaTime, Animator* animator)
{
	auto layer0 = m_AnimatorLayers.front();
	auto stateMachine = layer0.m_StateMachine;
	auto& state = stateMachine->m_CurrentState;
	if (state == nullptr)
	{
		state = stateMachine->m_DefaultState;
		state->m_LocalPlayTimer = 0;
	}
	AnimatorStateTransition* transition = nullptr;
	if (!state->m_Transitions.empty())
		transition = state->m_Transitions[0];
	state->m_LocalPlayTimer += deltaTime;
	float timer = state->m_LocalPlayTimer;
	
//	GetBoneFunction GetBone = [animator](const std::string& path, HumanBodyBones boneId) {
//		if (animator->IsHumanBodyBonesMapped())
//			return animator->GetBoneTransform(boneId);
//		else
//			return animator->GetBoneTransformByPath(path);
//	};
	
	std::vector<AnimatorClipInfo> info;
	
	if (transition != nullptr)
	{
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
		
		auto clip1 = state->m_Motion->As<AnimationClip>();
		info.push_back(AnimatorClipInfo{clip1, weight1, state->m_LocalPlayTimer});
		
		if (state2 != nullptr)
		{
			auto clip2 = state2->m_Motion->As<AnimationClip>();
			info.push_back(AnimatorClipInfo{clip2, weight2, state2->m_LocalPlayTimer});
		}
		
//		if (state2 == nullptr)
//		{
//			auto clip = state->m_Motion->As<AnimationClip>();
//			::ApplyAnimation(clip, state->m_LocalPlayTimer, GetBone);
//		}
//		else
//		{
//			auto clip1 = state->m_Motion->As<AnimationClip>();
//			auto clip2 = state2->m_Motion->As<AnimationClip>();
//			::ApplyAnimation2(clip1, clip2, state->m_LocalPlayTimer, state2->m_LocalPlayTimer, weight1, weight2, GetBone);
//		}
	}
	else
	{
		auto clip = state->m_Motion->As<AnimationClip>();
//		::ApplyAnimation(clip, state->m_LocalPlayTimer, GetBone);
		info.push_back(AnimatorClipInfo{clip, 1, state->m_LocalPlayTimer});
	}
	return info;
}
