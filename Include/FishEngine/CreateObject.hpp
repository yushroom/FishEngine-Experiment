#pragma once

#include "FishEngine2.hpp"

#include "ClassID.hpp"

namespace FishEngine
{
	template<class T>
	inline T* CreateEmptyObject() { return new T(); }

	template<>
	inline GameObject* CreateEmptyObject<GameObject>() {
		return new GameObject("", GameObjectConstructionFlag::Empty);
	}


	inline Object* CreateEmptyObjectByClassID(int classID)
	{
//		if (classID == Prefab::ClassID)
//			return CreateEmptyObject<Prefab>();
		Object* ret = nullptr;
		switch(classID)
		{
			case GameObject::ClassID:
				ret = CreateEmptyObject<GameObject>(); break;
			case Transform::ClassID:
				ret = CreateEmptyObject<Transform>(); break;
			case Camera::ClassID:
				ret = CreateEmptyObject<Camera>(); break;
			case Light::ClassID:
				ret = CreateEmptyObject<Light>(); break;
			case RectTransform::ClassID:
				ret = CreateEmptyObject<RectTransform>(); break;
			case MeshFilter::ClassID:
				ret = CreateEmptyObject<MeshFilter>(); break;
			case MeshRenderer::ClassID:
				ret = CreateEmptyObject<MeshRenderer>(); break;
			case SkinnedMeshRenderer::ClassID:
				ret = CreateEmptyObject<SkinnedMeshRenderer>(); break;
			case BoxCollider::ClassID:
				ret = CreateEmptyObject<BoxCollider>(); break;
			case SphereCollider::ClassID:
				ret = CreateEmptyObject<SphereCollider>(); break;
			case Rigidbody::ClassID:
				ret = CreateEmptyObject<Rigidbody>(); break;
			case Animation::ClassID:
				ret = CreateEmptyObject<Animation>(); break;
			case Animator::ClassID:
				ret = CreateEmptyObject<Animator>(); break;
//			case Script::ClassID:
//				ret = CreateEmptyObject<Script>(); break;
			case FishEditor::Animations::AnimatorStateTransition::ClassID:
				ret = CreateEmptyObject<FishEditor::Animations::AnimatorStateTransition>(); break;
			case FishEditor::Animations::AnimatorState::ClassID:
				ret = CreateEmptyObject<FishEditor::Animations::AnimatorState>(); break;
			case FishEditor::Animations::AnimatorController::ClassID:
				ret = CreateEmptyObject<FishEditor::Animations::AnimatorController>(); break;
			case FishEditor::Animations::AnimatorStateMachine::ClassID:
				ret = CreateEmptyObject<FishEditor::Animations::AnimatorStateMachine>(); break;
			case RenderSettings::ClassID:
				ret = CreateEmptyObject<RenderSettings>(); break;
			default:
				LogWarning(Format("Unknown class ID: {}[{}]", classID, GetNameByClassID(classID)));
		}
		return ret;
	}
}
