#pragma once

#include "../Object.hpp"
#include "AnimationClip.hpp"
#include <map>

namespace FishEngine
{
	class Transform;
	
	// Runtime representation of the AnimatorController.
	// It can be used to change the Animator's controller during runtime.
	class RuntimeAnimatorController : public Object
	{
	public:
		DeclareObject(RuntimeAnimatorController, 93);
		RuntimeAnimatorController() : Object(ClassID, ClassName)
		{
		}
		RuntimeAnimatorController(int classID, const char* className) : Object(classID, className)
		{
			
		}

		// Retrieves all AnimationClip used by the controller.
		const std::vector<AnimationClip*>&
		GetAnimationClips() const;
		
		virtual void ApplyAnimation(float deltaTime, std::map<std::string, Transform*> const & skeleton) = 0;
	};
	

	enum class AnimatorControllerParameterType
	{
		Float,
		Int,
		Bool,
		Trigger
	};

	class AnimatorControllerParameter
	{
	public:
		std::string							m_Name;
		AnimatorControllerParameterType		m_Type;
		float								m_DefaultFloat = 0;
		int									m_DefaultInt = 0;
		bool								m_DefaultBool = false;
		RuntimeAnimatorController*			m_Controller = nullptr;
	};

	class AvatarMask : public Object
	{

	};
}

namespace FishEditor::Animations
{
	class AnimatorController;
	class AnimatorStateMachine;
	class AnimatorState;

	enum class AnimatorLayerBlendingMode
	{
		Override,
		Additive
	};
	
	enum class AnimatorConditionMode
	{
		If,			//The condition is true when the parameter value is true.
		IfNot,		//The condition is true when the parameter value is false.
		Greater,	//The condition is true when parameter value is greater than the threshold.
		Less,		//The condition is true when the parameter value is less than the threshold.
		Equals,		//The condition is true when parameter value is equal to the threshold.
		NotEqual,	//The condition is true when the parameter value is not equal to the threshold.
	};
	
	struct AnimatorCondition
	{
		AnimatorConditionMode mode;
		std::string parameter;
		float threshold;
	};

	class AnimatorTransitionBase : public FishEngine::Object
	{
	public:
		DeclareObject(AnimatorTransitionBase, 1111);
		AnimatorTransitionBase(int classID, const char* className) : Object(classID, className) {}
		
		// m_Conditions:
		// - m_ConditionMode: 4
		// m_ConditionEvent: Speed
		// m_EventTreshold: 0.1
		// m_DstStateMachine: {fileID: 0}
		// m_DstState: {fileID: 110252330}
		// m_Solo: 0
		// m_Mute: 0
		// m_IsExit: 0
//		std::vector<AnimatorCondition> m_Conditions;
		AnimatorStateMachine* m_DstStateMachine = nullptr;
		AnimatorState* m_DstState = nullptr;
		bool m_Solo = false;
		bool m_Mute = false;
		bool m_IsExit = false;
	};
	

	class AnimatorStateTransition : public AnimatorTransitionBase
	{
	public:
		DeclareObject(AnimatorStateTransition, 1101);
		AnimatorStateTransition() : AnimatorTransitionBase(ClassID, ClassName) {}
		
		// serializedVersion: 3
		float m_TransitionDuration = 0.25;
		float m_TransitionOffset = 0;
		float m_ExitTime = 0.79310346f;
		bool m_HasExitTime = false;
		bool m_HasFixedDuration = true;
		// m_InterruptionSource: 0
		// m_OrderedInterruption: 1
		// m_CanTransitionToSelf: 1
	};

	class BlendTree : public FishEngine::Motion
	{
		// m_ObjectHideFlags: 1
		// m_PrefabParentObject: {fileID: 0}
		// m_PrefabInternal: {fileID: 0}
		// m_Name: WalkingBlendTree
		// m_Childs:
		// - serializedVersion: 2
		// m_Motion: {fileID: 7400000, guid: dffa50cfe77e0434bbfa71245b3dd529, type: 3}
		// m_Threshold: 0
		// m_Position: {x: 0, y: 0}
		// m_TimeScale: 1
		// m_CycleOffset: 0
		// m_DirectBlendParameter: Speed
		// m_Mirror: 0
		// - serializedVersion: 2
		// m_Motion: {fileID: 7400000, guid: b1a5e04ae51004842aba06704a6c2903, type: 3}
		// m_Threshold: 1.5584016
		// m_Position: {x: 0, y: 0}
		// m_TimeScale: 1
		// m_CycleOffset: 0
		// m_DirectBlendParameter: Speed
		// m_Mirror: 0
		// - serializedVersion: 2
		// m_Motion: {fileID: 7400000, guid: 1cb8ed3cbba15f0479fbae54e0a963df, type: 3}
		// m_Threshold: 5.6623173
		// m_Position: {x: 0, y: 0}
		// m_TimeScale: 1
		// m_CycleOffset: 0
		// m_DirectBlendParameter: Speed
		// m_Mirror: 0
		// m_BlendParameter: Speed
		// m_BlendParameterY: Blend
		// m_MinThreshold: 0
		// m_MaxThreshold: 5.6623173
		// m_UseAutomaticThresholds: 0
		// m_NormalizedBlendValues: 0
		// m_BlendType: 0
	};

	class AnimatorState : public FishEngine::Object
	{
	public:
		DeclareObject(AnimatorState, 1102);
		
		AnimatorState() : Object(ClassID, ClassName)
		{
			
		}
		// The default speed of the motion.
		// 1.0 is base speed.
		// float m_Speed: 1

		// float m_CycleOffset = 0;

		std::vector<AnimatorStateTransition*> m_Transitions;

		// m_StateMachineBehaviours: []

		// Vector3 m_Position = {50, 50, 0};

		// Should Foot IK be respected for this state.
		// bool m_IKOnFeet = false;

		// Whether or not the AnimatorStates writes back the default values for properties that are not animated by its Motion.
		// bool m_WriteDefaultValues: 1

		// Should the state be mirrored.
		// bool m_Mirror = false;

		// The animator controller parameter that drives the mirror value.
		// std::string m_MirrorParameter;
		//  Define if the mirror value is driven by an Animator controller parameter or by the value set in the editor.
		// bool m_MirrorParameterActive = false;


		// std::string m_SpeedParameter;
		// bool m_SpeedParameterActive = false;
		
		FishEngine::Motion* m_Motion = nullptr;
		
		// std::string m_Tag;

		// std::string m_CycleOffsetParameter;
		// bool m_CycleOffsetParameterActive = false;
		
		float m_LocalPlayTimer = 0;
	};

	struct ChildAnimatorState
	{
	public:
		AnimatorStateMachine* 	m_StateMachine = nullptr;

		AnimatorState* 			m_State = nullptr;

		// in editor
		// The position the the state machine node in the context of its parent state machine.
//		FishEngine::Vector3 	m_Position;
	};

	class AnimatorStateMachine : public FishEngine::Object
	{
	public:
		DeclareObject(AnimatorStateMachine, 1107);
		AnimatorStateMachine() : Object(ClassID, ClassName)
		{
			
		}
		
	private:
		friend class AnimatorController;
		std::vector<ChildAnimatorState> m_ChildStates;
		// m_ChildStateMachines;
		// m_AnyStateTransitions;
		// m_EntryTransitions;
		// m_StateMachineTransitions;
		// m_StateMachineBehaviours;
		// m_AnyStatePosition;
		// m_EntryPosition;
		// m_ExitPosition;
		// m_ParentStateMachinePosition;
		AnimatorState* m_DefaultState = nullptr;
		
	private:
		AnimatorState* m_CurrentState = nullptr;
		float m_CurrentStatePlayTimer = 0;
	};

	class AnimatorControllerLayer
	{
	public:
		std::string 				m_Name;
		AnimatorStateMachine* 		m_StateMachine = nullptr;
		// FishEngine::AvatarMask*		m_Mask = nullptr;
		// m_Motions;
		// m_Behaviours;
		// AnimatorLayerBlendingMode 	m_BlendingMode;
		// int 						m_SyncedLayerIndex = -1;
		// float 						m_DefaultWeight = 0;
		// bool 						m_IKPass = false;

		// When active, the layer will take control of the duration of the Synced Layer.
		// bool 						m_SyncedLayerAffectsTiming = false;
		AnimatorController* 		m_Controller = nullptr;
	};

	class AnimatorController : public FishEngine::RuntimeAnimatorController
	{
	public:
		DeclareObject(AnimatorController, 91);
		AnimatorController() : RuntimeAnimatorController(ClassID, ClassName)
		{
			
		}
		
		virtual void ApplyAnimation(float deltaTime, std::map<std::string, FishEngine::Transform*> const & skeleton)  override;

	private:
		std::vector<AnimatorControllerLayer>
		m_AnimatorLayers;

		std::vector<FishEngine::AnimatorControllerParameter>
		m_AnimatorParameters;
	};
}
