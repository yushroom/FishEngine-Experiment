#pragma once

#include <FishEngine/FishEngine.hpp>
#include "../Object.hpp"
#include "../Asset.hpp"
#include "WrapMode.hpp"
#include "AnimationBlendMode.hpp"
#include "AnimationEvent.hpp"

#include "AnimationCurve.hpp"

namespace FishEngine
{
	class FE_EXPORT Motion : public Object
	{
	public:
		DeclareObject(Motion, 207);

		explicit Motion(int classID=ClassID, const char* className = ClassName) : Object(classID, className)
		{
			LOGF;
		}
	};
	
	struct FE_EXPORT QuaternionCurve
	{
		std::string path;
		TAnimationCurve<Quaternion> curve;
	};

	struct FE_EXPORT Vector3Curve
	{
		std::string path;
		TAnimationCurve<Vector3> curve;
	};
	
	class Avatar;
	
	// Stores keyframe based animations.
	// AnimationClip is used by Animation to play back animations.
	class FE_EXPORT AnimationClip final : public Motion
	{
	public:
		
		DeclareObject(AnimationClip, 74);

		AnimationClip() : Motion(AnimationClip::ClassID, ClassName)
		{
			LOGF;
			AssetManager::GetInstance().AddAsset(this);
		}
		
		// Animation Events for this animation clip.
		std::vector<AnimationEvent> events;
		
		// Frame rate at which keyframes are sampled. (Read Only)
		// This is the frame rate that was used in the animation program you used to create the animation or model.
		float frameRate = 0;
		
		// Animation length in seconds. (Read Only)
		float length = 0;
		
		// Sets the default wrap mode used in the animation state.
		WrapMode wrapMode = WrapMode::Once;

		//TAnimationCurve<Vector3> m_positionCurve;
		//TAnimationCurve<Quaternion> m_rotationCurve;
		//TAnimationCurve<Vector3> m_scaleCurve;
		std::vector<Vector3Curve> m_positionCurve;
		std::vector<QuaternionCurve> m_rotationCurves;
		std::vector<Vector3Curve> m_eulersCurves;
		std::vector<Vector3Curve> m_scaleCurves;
		Avatar* m_avatar = nullptr;
	};


	// Information about the current or next state.
	struct AnimatorStateInfo
	{

	};
}
