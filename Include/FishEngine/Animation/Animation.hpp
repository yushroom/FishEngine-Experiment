#pragma once

#include "../Component/Behaviour.hpp"
#include "WrapMode.hpp"

namespace FishEngine
{
	
	class AnimationClip;
	
	class FE_EXPORT Animation : public Behaviour
	{
	public:
		DeclareObject(Animation, 111);

		Animation() = default;

		void Start();
		void Update(float deltaTime);

		// the default animation
		AnimationClip* m_clip;

		bool m_isPlaying = false;

		bool m_playAutomatically = true;

		WrapMode m_wrapMode;

		// temp
		float m_localTimer = 0.0f;
		std::map<std::string, Transform*> m_skeleton;
	};
}
