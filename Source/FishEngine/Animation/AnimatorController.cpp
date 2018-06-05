#include <FishEngine/Animation/AnimatorController.hpp>

using namespace FishEngine;
using namespace FishEditor;

AnimationClip* FishEditor::Animations::AnimatorController::GetCurrentAnimationClip()
{
	auto layer0 = m_AnimatorLayers.front();
	auto stateMachine = layer0.m_StateMachine;
	auto state = stateMachine->m_DefaultState;
	return state->m_Motion->As<AnimationClip>();
}
