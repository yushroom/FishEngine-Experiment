#pragma once

#include "../FishEngine.hpp"
#include "../Component/Behaviour.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Quaternion.hpp"
#include "../Math/Matrix4x4.hpp"
#include "Avatar.hpp"
#include "AnimatorClipInfo.hpp"
#include "../Component/SkinnedMeshRenderer.hpp"

namespace FishEngine
{
	class Avatar;
	class RuntimeAnimatorController;
	class SkinnedMeshRenderer;
	class RenderSystem;

	class FE_EXPORT Animator : public Behaviour
	{
	public:
		DeclareObject(Animator, 95);
		Animator() : Behaviour(ClassID, ClassName)
		{
			
		}
		
		RuntimeAnimatorController*
		GetRuntimeAnimatorController() const { return m_Controller; }
		
		Avatar*
		GetAvatar() const { return m_Avatar; }
		
		// Returns transform mapped to this human bone id.
		Transform*
		GetBoneTransform(HumanBodyBones humanBoneId)
		{
			if (humanBoneId == HumanBodyBones::LastBone || !m_HumanBodyBonesMapped)
				return nullptr;
			return m_Skeleton2[static_cast<int>(humanBoneId)];
		}
		
		Transform*
		GetBoneTransformByPath(const std::string& path);
		
		std::vector<AnimatorClipInfo>
		GetCurrentAnimatorClipInfo(int layerIndex);
		
		void Start();
		void Play();
		void Update(float deltaTime);
		
		// temp
		bool IsHumanBodyBonesMapped() { return m_HumanBodyBonesMapped; }
		
	private:
		friend class RenderSystem;
		
		Avatar* 					m_Avatar = nullptr;
		RuntimeAnimatorController* 	m_Controller = nullptr;
//		m_CullingMode;
//		m_UpdateMode;
		bool 						m_ApplyRootMotion = false;
		bool 						m_LinearVelocityBlending = false;
		std::string 				m_WarningMessage;
		bool 						m_HasTransformHierarchy = true;
		bool 						m_AllowConstantClipSamplingOptimization = true;
		
		bool m_HumanBodyBonesMapped = false;
		std::map<std::string, Transform*> m_Skeleton;
		Transform* m_Skeleton2[static_cast<int>(HumanBodyBones::LastBone)] = {nullptr};
		SkinnedMeshRenderer* m_SRenderer = nullptr;
	};
	
}
