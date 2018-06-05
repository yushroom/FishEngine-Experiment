#pragma once

#include <FishEngine/Serialization/Archive.hpp>

namespace FishEditor::Animations
{
	class AnimatorControllerLayer;
	class ChildAnimatorStateMachine;
}

namespace FishEngine
{
	struct Vector2;
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, Vector2& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Vector2& t);

	struct Vector3;
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, Vector3& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Vector3& t);

	struct Vector4;
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, Vector4& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Vector4& t);

	struct Quaternion;
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, Quaternion& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Quaternion& t);

	struct Color;
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, Color& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Color& t);

	struct Modification;
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, Modification& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Modification& t);

	struct PrefabModification;
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, PrefabModification& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const PrefabModification& t);

	struct HumanLimit;
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, HumanLimit& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const HumanLimit& t);

	struct HumanBone;
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, HumanBone& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const HumanBone& t);

	struct SkeletonBone;
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, SkeletonBone& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const SkeletonBone& t);

	struct HumanDescription;
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, HumanDescription& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const HumanDescription& t);

	
	// AnimatorControllerLayer
	InputArchive& operator>>(InputArchive& archive, FishEditor::Animations::AnimatorControllerLayer& t);
	OutputArchive& operator<<(OutputArchive& archive, const FishEditor::Animations::AnimatorControllerLayer& t);
	
	// ChildAnimatorStateMachine
	InputArchive& operator>>(InputArchive& archive, FishEditor::Animations::ChildAnimatorStateMachine& t);
	OutputArchive& operator<<(OutputArchive& archive, const FishEditor::Animations::ChildAnimatorStateMachine& t);
	
}
