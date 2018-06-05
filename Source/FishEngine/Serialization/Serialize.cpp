#include <FishEngine/Serialization/Serialize.hpp>
#include <FishEngine/Serialization/Archive.hpp>
#include <FishEngine/FishEngine2.hpp>

#include <FishEngine/Animation/HumanDescription.hpp>
#include <FishEngine/Animation/AnimatorController.hpp>

using namespace FishEngine;
using namespace FishEditor;
using namespace FishEditor::Animations;

namespace FishEngine
{
	// Vector2
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, Vector2& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Vector2& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		return archive;
	}

	// Vector3
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, Vector3& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Vector3& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		return archive;
	}

	// Vector4
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, Vector4& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Vector4& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}

	// Quaternion
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, Quaternion& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Quaternion& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}

	// Color
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, Color& t)
	{
		archive.AddNVP("r", t.r);
		archive.AddNVP("g", t.g);
		archive.AddNVP("b", t.b);
		archive.AddNVP("a", t.a);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Color& t)
	{
		archive.AddNVP("r", t.r);
		archive.AddNVP("g", t.g);
		archive.AddNVP("b", t.b);
		archive.AddNVP("a", t.a);
		return archive;
	}

	// Modification
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, Modification& t)
	{
		archive.AddNVP("target", t.target);
		archive.AddNVP("propertyPath", t.propertyPath);
		archive.AddNVP("value", t.value);
		archive.AddNVP("objectReference", t.objectReference);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const Modification& t)
	{
		archive.AddNVP("target", t.target);
		archive.AddNVP("propertyPath", t.propertyPath);
		archive.AddNVP("value", t.value);
		archive.AddNVP("objectReference", t.objectReference);
		return archive;
	}

	// PrefabModification
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, PrefabModification& t)
	{
		archive.AddNVP("m_TransformParent", t.m_TransformParent);
		archive.AddNVP("m_Modifications", t.m_Modifications);
		archive.AddNVP("m_RemovedComponents", t.m_RemovedComponents);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const PrefabModification& t)
	{
		archive.AddNVP("m_TransformParent", t.m_TransformParent);
		archive.AddNVP("m_Modifications", t.m_Modifications);
		archive.AddNVP("m_RemovedComponents", t.m_RemovedComponents);
		return archive;
	}

	// AnimatorControllerLayer
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, AnimatorControllerLayer& t)
	{
		archive.AddNVP("m_Name", t.m_Name);
		archive.AddNVP("m_StateMachine", t.m_StateMachine);
		archive.AddNVP("m_Controller", t.m_Controller);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const AnimatorControllerLayer& t)
	{
		archive.AddNVP("m_Name", t.m_Name);
		archive.AddNVP("m_StateMachine", t.m_StateMachine);
		archive.AddNVP("m_Controller", t.m_Controller);
		return archive;
	}

	// ChildAnimatorStateMachine
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, ChildAnimatorStateMachine& t)
	{
		archive.AddNVP("m_State", t.m_State);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const ChildAnimatorStateMachine& t)
	{
		archive.AddNVP("m_State", t.m_State);
		return archive;
	}

	// HumanLimit
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, HumanLimit& t)
	{
		archive.AddNVP("min", t.m_Min);
		archive.AddNVP("max", t.m_Max);
		archive.AddNVP("value", t.m_Center);
		archive.AddNVP("length", t.m_AxisLength);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const HumanLimit& t)
	{
		archive.AddNVP("min", t.m_Min);
		archive.AddNVP("max", t.m_Max);
		archive.AddNVP("value", t.m_Center);
		archive.AddNVP("length", t.m_AxisLength);
		return archive;
	}

	// HumanBone
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, HumanBone& t)
	{
		archive.AddNVP("boneName", t.boneName);
		archive.AddNVP("humanName", t.humanName);
		archive.AddNVP("limit", t.limit);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const HumanBone& t)
	{
		archive.AddNVP("boneName", t.boneName);
		archive.AddNVP("humanName", t.humanName);
		archive.AddNVP("limit", t.limit);
		return archive;
	}

	// SkeletonBone
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, SkeletonBone& t)
	{
		archive.AddNVP("name", t.name);
		archive.AddNVP("position", t.position);
		archive.AddNVP("rotation", t.rotation);
		archive.AddNVP("scale", t.scale);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const SkeletonBone& t)
	{
		archive.AddNVP("name", t.name);
		archive.AddNVP("position", t.position);
		archive.AddNVP("rotation", t.rotation);
		archive.AddNVP("scale", t.scale);
		return archive;
	}

	// HumanDescription
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, HumanDescription& t)
	{
		archive.AddNVP("human", t.human);
		archive.AddNVP("skeleton", t.skeleton);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const HumanDescription& t)
	{
		archive.AddNVP("human", t.human);
		archive.AddNVP("skeleton", t.skeleton);
		return archive;
	}

	// MaterialTextureProperty
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, MaterialTextureProperty& t)
	{
		archive.AddNVP("m_Texture", t.m_Texture);
		archive.AddNVP("m_Scale", t.m_Scale);
		archive.AddNVP("m_Offset", t.m_Offset);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const MaterialTextureProperty& t)
	{
		archive.AddNVP("m_Texture", t.m_Texture);
		archive.AddNVP("m_Scale", t.m_Scale);
		archive.AddNVP("m_Offset", t.m_Offset);
		return archive;
	}

	// MaterialProperties
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, MaterialProperties& t)
	{
		archive.AddNVP("m_TexEnvs", t.m_TexEnvs);
		archive.AddNVP("m_Floats", t.m_Floats);
		archive.AddNVP("m_Colors", t.m_Colors);
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const MaterialProperties& t)
	{
		archive.AddNVP("m_TexEnvs", t.m_TexEnvs);
		archive.AddNVP("m_Floats", t.m_Floats);
		archive.AddNVP("m_Colors", t.m_Colors);
		return archive;
	}
}

//namespace FishEngine
//{
	// Object
	void Object::Deserialize(InputArchive& archive)
	{
		archive.AddNVP("m_ObjectHideFlags", this->m_ObjectHideFlags);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		archive.AddNVP("m_Name", this->m_Name);
	}

	void Object::Serialize(OutputArchive& archive) const
	{
		archive.AddNVP("m_ObjectHideFlags", this->m_ObjectHideFlags);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		archive.AddNVP("m_Name", this->m_Name);
	}


	// GameObject
	void GameObject::Deserialize(InputArchive& archive)
	{
//		Object::Deserialize(archive);
		archive.AddNVP("m_ObjectHideFlags", this->m_ObjectHideFlags);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		decltype(this->m_Component) components;
		archive.AddNVP("m_Component", components);
		for (auto comp : components)
		{
			this->AddComponent(comp);
		}
		archive.AddNVP("m_Layer", this->m_Layer);
		archive.AddNVP("m_Name", this->m_Name);
		archive.AddNVP("m_TagString", this->m_TagString);
		archive.AddNVP("m_IsActive", this->m_IsActive);
	}

	void GameObject::Serialize(OutputArchive& archive) const
	{
//		Object::Serialize(archive);
		archive.AddNVP("m_ObjectHideFlags", this->m_ObjectHideFlags);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		archive.AddNVP("m_Component", this->m_Component);
		archive.AddNVP("m_Layer", this->m_Layer);
		archive.AddNVP("m_Name", this->m_Name);
		archive.AddNVP("m_TagString", this->m_TagString);
		archive.AddNVP("m_IsActive", this->m_IsActive);
	}


	// Prefab
	void Prefab::Deserialize(InputArchive& archive)
	{
		Object::Deserialize(archive);
		archive.AddNVP("m_Modification", this->m_Modification);
		archive.AddNVP("m_ParentPrefab", this->m_ParentPrefab);
		archive.AddNVP("m_IsPrefabParent", this->m_IsPrefabParent);
		if (this->m_IsPrefabParent) {
			archive.AddNVP("m_RootGameObject", this->m_RootGameObject);
		}
	}

	void Prefab::Serialize(OutputArchive& archive) const
	{
		Object::Serialize(archive);
		archive.AddNVP("m_Modification", this->m_Modification);
		archive.AddNVP("m_ParentPrefab", this->m_ParentPrefab);
		archive.AddNVP("m_IsPrefabParent", this->m_IsPrefabParent);
		if (this->m_IsPrefabParent) {
			archive.AddNVP("m_RootGameObject", this->m_RootGameObject);
		}
	}


	// Component
	void Component::Deserialize(InputArchive& archive)
	{
//		Object::Deserialize(archive);
		archive.AddNVP("m_ObjectHideFlags", this->m_ObjectHideFlags);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		archive.AddNVP("m_GameObject", this->m_GameObject);
		this->m_GameObject->AddComponent(this);
	}

	void Component::Serialize(OutputArchive& archive) const
	{
//		Object::Serialize(archive);
		archive.AddNVP("m_ObjectHideFlags", this->m_ObjectHideFlags);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		archive.AddNVP("m_GameObject", this->m_GameObject);
	}


	// Transform
	void Transform::Deserialize(InputArchive& archive)
	{
		Component::Deserialize(archive);
		archive.AddNVP("m_LocalRotation", this->m_LocalRotation);
		archive.AddNVP("m_LocalPosition", this->m_LocalPosition);
		archive.AddNVP("m_LocalScale", this->m_LocalScale);
		archive.AddNVP("m_Children", this->m_Children);
		archive.AddNVP("m_Father", this->m_Father);
		archive.AddNVP("m_RootOrder", this->m_RootOrder);
	}

	void Transform::Serialize(OutputArchive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_LocalRotation", this->m_LocalRotation);
		archive.AddNVP("m_LocalPosition", this->m_LocalPosition);
		archive.AddNVP("m_LocalScale", this->m_LocalScale);
		archive.AddNVP("m_Children", this->m_Children);
		archive.AddNVP("m_Father", this->m_Father);
		archive.AddNVP("m_RootOrder", this->m_RootOrder);
	}


	// Camera
	void Camera::Deserialize(InputArchive& archive)
	{
		Component::Deserialize(archive);
		archive.AddNVP("m_NearClipPlane", this->m_NearClipPlane);
		archive.AddNVP("m_FarClipPlane", this->m_FarClipPlane);
		archive.AddNVP("m_FieldOfView", this->m_FieldOfView);
		archive.AddNVP("m_Orthographic", this->m_Orthographic);
		archive.AddNVP("m_OrthographicSize", this->m_OrthographicSize);
	}

	void Camera::Serialize(OutputArchive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_NearClipPlane", this->m_NearClipPlane);
		archive.AddNVP("m_FarClipPlane", this->m_FarClipPlane);
		archive.AddNVP("m_FieldOfView", this->m_FieldOfView);
		archive.AddNVP("m_Orthographic", this->m_Orthographic);
		archive.AddNVP("m_OrthographicSize", this->m_OrthographicSize);
	}


	// Material
	void Material::Deserialize(InputArchive& archive)
	{
		Object::Deserialize(archive);
		archive.AddNVP("m_Shader", this->m_Shader);
		archive.AddNVP("m_ShaderKeywords", this->m_ShaderKeywords);
		//archive.AddNVP("m_LightmapFlags", this->m_LightmapFlags);
		//archive.AddNVP("m_CustomRenderQueue", this->m_CustomRenderQueue);
		//archive.AddNVP("stringTagMap", this->stringTagMap);
		archive.AddNVP("m_SavedProperties", this->m_SavedProperties);
	}

	void Material::Serialize(OutputArchive& archive) const
	{
		Object::Serialize(archive);
		archive.AddNVP("m_Shader", this->m_Shader);
		archive.AddNVP("m_ShaderKeywords", this->m_ShaderKeywords);
		//archive.AddNVP("m_LightmapFlags", this->m_LightmapFlags);
		//archive.AddNVP("m_CustomRenderQueue", this->m_CustomRenderQueue);
		//archive.AddNVP("stringTagMap", this->stringTagMap);
		archive.AddNVP("m_SavedProperties", this->m_SavedProperties);
	}


	// Behaviour
	void Behaviour::Deserialize(InputArchive& archive)
	{
		Component::Deserialize(archive);
		archive.AddNVP("m_Enabled", this->m_Enabled);
	}

	void Behaviour::Serialize(OutputArchive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_Enabled", this->m_Enabled);
	}


	// Light
	void Light::Deserialize(InputArchive& archive)
	{
		Behaviour::Deserialize(archive);
	}

	void Light::Serialize(OutputArchive& archive) const
	{
		Behaviour::Serialize(archive);
	}


	// RectTransform
	void RectTransform::Deserialize(InputArchive& archive)
	{
		Component::Deserialize(archive);
		archive.AddNVP("m_AnchorMin", this->m_AnchorMin);
		archive.AddNVP("m_AnchorMax", this->m_AnchorMax);
		archive.AddNVP("m_AnchoredPosition", this->m_AnchoredPosition);
		archive.AddNVP("m_SizeDelta", this->m_SizeDelta);
		archive.AddNVP("m_Pivot", this->m_Pivot);
	}

	void RectTransform::Serialize(OutputArchive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_AnchorMin", this->m_AnchorMin);
		archive.AddNVP("m_AnchorMax", this->m_AnchorMax);
		archive.AddNVP("m_AnchoredPosition", this->m_AnchoredPosition);
		archive.AddNVP("m_SizeDelta", this->m_SizeDelta);
		archive.AddNVP("m_Pivot", this->m_Pivot);
	}


	// MeshFilter
	void MeshFilter::Deserialize(InputArchive& archive)
	{
		Component::Deserialize(archive);
		archive.AddNVP("m_Mesh", this->m_Mesh);
	}

	void MeshFilter::Serialize(OutputArchive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_Mesh", this->m_Mesh);
	}


	// Renderer
	void Renderer::Deserialize(InputArchive& archive)
	{
		Component::Deserialize(archive);
		archive.AddNVP("m_Enabled", this->m_Enabled);
		archive.AddNVP("m_CastShadows", this->m_CastShadows);
		archive.AddNVP("m_ReceiveShadows", this->m_ReceiveShadows);
		archive.AddNVP("m_Materials", this->m_Materials);
	}

	void Renderer::Serialize(OutputArchive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_Enabled", this->m_Enabled);
		archive.AddNVP("m_CastShadows", this->m_CastShadows);
		archive.AddNVP("m_ReceiveShadows", this->m_ReceiveShadows);
		archive.AddNVP("m_Materials", this->m_Materials);
	}


	// MeshRenderer
	void MeshRenderer::Deserialize(InputArchive& archive)
	{
		Renderer::Deserialize(archive);
	}

	void MeshRenderer::Serialize(OutputArchive& archive) const
	{
		Renderer::Serialize(archive);
	}


	// SkinnedMeshRenderer
	void SkinnedMeshRenderer::Deserialize(InputArchive& archive)
	{
		Renderer::Deserialize(archive);
		archive.AddNVP("m_Mesh", this->m_Mesh);
		archive.AddNVP("m_Avatar", this->m_Avatar);
		archive.AddNVP("m_RootBone", this->m_RootBone);
		archive.AddNVP("m_Bones", this->m_Bones);
	}

	void SkinnedMeshRenderer::Serialize(OutputArchive& archive) const
	{
		Renderer::Serialize(archive);
		archive.AddNVP("m_Mesh", this->m_Mesh);
		archive.AddNVP("m_Avatar", this->m_Avatar);
		archive.AddNVP("m_RootBone", this->m_RootBone);
		archive.AddNVP("m_Bones", this->m_Bones);
	}


	// Collider
	void Collider::Deserialize(InputArchive& archive)
	{
		Component::Deserialize(archive);
		archive.AddNVP("m_IsTrigger", this->m_IsTrigger);
		archive.AddNVP("m_Enabled", this->m_Enabled);
	}

	void Collider::Serialize(OutputArchive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_IsTrigger", this->m_IsTrigger);
		archive.AddNVP("m_Enabled", this->m_Enabled);
	}


	// BoxCollider
	void BoxCollider::Deserialize(InputArchive& archive)
	{
		Collider::Deserialize(archive);
		archive.AddNVP("m_Size", this->m_Size);
		archive.AddNVP("m_Center", this->m_Center);
	}

	void BoxCollider::Serialize(OutputArchive& archive) const
	{
		Collider::Serialize(archive);
		archive.AddNVP("m_Size", this->m_Size);
		archive.AddNVP("m_Center", this->m_Center);
	}


	// SphereCollider
	void SphereCollider::Deserialize(InputArchive& archive)
	{
		Collider::Deserialize(archive);
		archive.AddNVP("m_Radius", this->m_Radius);
		archive.AddNVP("m_Center", this->m_Center);
	}

	void SphereCollider::Serialize(OutputArchive& archive) const
	{
		Collider::Serialize(archive);
		archive.AddNVP("m_Radius", this->m_Radius);
		archive.AddNVP("m_Center", this->m_Center);
	}


	// Rigidbody
	void Rigidbody::Deserialize(InputArchive& archive)
	{
		Component::Deserialize(archive);
		archive.AddNVP("m_Mass", this->m_Mass);
		archive.AddNVP("m_Drag", this->m_Drag);
		archive.AddNVP("m_AngularDrag", this->m_AngularDrag);
		archive.AddNVP("m_UseGravity", this->m_UseGravity);
		archive.AddNVP("m_IsKinematic", this->m_IsKinematic);
	}

	void Rigidbody::Serialize(OutputArchive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_Mass", this->m_Mass);
		archive.AddNVP("m_Drag", this->m_Drag);
		archive.AddNVP("m_AngularDrag", this->m_AngularDrag);
		archive.AddNVP("m_UseGravity", this->m_UseGravity);
		archive.AddNVP("m_IsKinematic", this->m_IsKinematic);
	}


	// Avatar
	void Avatar::Deserialize(InputArchive& archive)
	{
		Object::Deserialize(archive);
	}

	void Avatar::Serialize(OutputArchive& archive) const
	{
		Object::Serialize(archive);
	}


	// Motion
	void Motion::Deserialize(InputArchive& archive)
	{
		Object::Deserialize(archive);
	}

	void Motion::Serialize(OutputArchive& archive) const
	{
		Object::Serialize(archive);
	}


	// AnimationClip
	void AnimationClip::Deserialize(InputArchive& archive)
	{
		Motion::Deserialize(archive);
		archive.AddNVP("m_avatar", this->m_avatar);
	}

	void AnimationClip::Serialize(OutputArchive& archive) const
	{
		Motion::Serialize(archive);
		archive.AddNVP("m_avatar", this->m_avatar);
	}


	// Animation
	void Animation::Deserialize(InputArchive& archive)
	{
		Behaviour::Deserialize(archive);
		archive.AddNVP("m_clip", this->m_clip);
		archive.AddNVP("m_wrapMode", this->m_wrapMode);
	}

	void Animation::Serialize(OutputArchive& archive) const
	{
		Behaviour::Serialize(archive);
		archive.AddNVP("m_clip", this->m_clip);
		archive.AddNVP("m_wrapMode", this->m_wrapMode);
	}

	// Animator
	void Animator::Deserialize(InputArchive& archive)
	{
		Behaviour::Deserialize(archive);
		archive.AddNVP("m_Avatar", this->m_Avatar);
		archive.AddNVP("m_Controller", this->m_Controller);
		archive.AddNVP("m_ApplyRootMotion", this->m_ApplyRootMotion);
		archive.AddNVP("m_LinearVelocityBlending", this->m_LinearVelocityBlending);
		archive.AddNVP("m_WarningMessage", this->m_WarningMessage);
		archive.AddNVP("m_HasTransformHierarchy", this->m_HasTransformHierarchy);
		archive.AddNVP("m_AllowConstantClipSamplingOptimization", this->m_AllowConstantClipSamplingOptimization);
	}

	void Animator::Serialize(OutputArchive& archive) const
	{
		Behaviour::Serialize(archive);
		archive.AddNVP("m_Avatar", this->m_Avatar);
		archive.AddNVP("m_Controller", this->m_Controller);
		archive.AddNVP("m_ApplyRootMotion", this->m_ApplyRootMotion);
		archive.AddNVP("m_LinearVelocityBlending", this->m_LinearVelocityBlending);
		archive.AddNVP("m_WarningMessage", this->m_WarningMessage);
		archive.AddNVP("m_HasTransformHierarchy", this->m_HasTransformHierarchy);
		archive.AddNVP("m_AllowConstantClipSamplingOptimization", this->m_AllowConstantClipSamplingOptimization);
	}
	
	
	// RuntimeAnimatorController
	void RuntimeAnimatorController::Deserialize(InputArchive& archive)
	{
		Object::Deserialize(archive);
	}
	
	void RuntimeAnimatorController::Serialize(OutputArchive& archive) const
	{
		Object::Serialize(archive);
	}
	
	
	// AnimatorController
	void AnimatorController::Deserialize(FishEngine::InputArchive& archive)
	{
		RuntimeAnimatorController::Deserialize(archive);
		archive.AddNVP("m_AnimatorLayers", this->m_AnimatorLayers);
	}
	
	void AnimatorController::Serialize(OutputArchive& archive) const
	{
		RuntimeAnimatorController::Serialize(archive);
		archive.AddNVP("m_AnimatorLayers", this->m_AnimatorLayers);
	}
	
	
	// AnimatorState
	void AnimatorState::Deserialize(InputArchive& archive)
	{
		Object::Deserialize(archive);
		archive.AddNVP("m_Motion", this->m_Motion);
	}
	
	void AnimatorState::Serialize(OutputArchive& archive) const
	{
		Object::Serialize(archive);
		archive.AddNVP("m_Motion", this->m_Motion);
	}
	
	
	// AnimatorStateMachine
	void AnimatorStateMachine::Deserialize(InputArchive& archive)
	{
		Object::Deserialize(archive);
		archive.AddNVP("m_ChildStates", this->m_ChildStates);
		archive.AddNVP("m_DefaultState", this->m_DefaultState);
	}
	
	void AnimatorStateMachine::Serialize(OutputArchive& archive) const
	{
		Object::Serialize(archive);
		archive.AddNVP("m_ChildStates", this->m_ChildStates);
		archive.AddNVP("m_DefaultState", this->m_DefaultState);
	}


	// RenderSettings
	void RenderSettings::Deserialize(InputArchive& archive)
	{
		Object::Deserialize(archive);
		archive.AddNVP("m_ObjectHideFlags", this->m_ObjectHideFlags);
		archive.AddNVP("m_Fog", this->m_Fog);
		archive.AddNVP("m_FogColor", this->m_FogColor);
		archive.AddNVP("m_FogMode", this->m_FogMode);
		archive.AddNVP("m_FogDensity", this->m_FogDensity);
		archive.AddNVP("m_LinearFogStart", this->m_LinearFogStart);
		archive.AddNVP("m_LinearFogEnd", this->m_LinearFogEnd);
		archive.AddNVP("m_AmbientSkyColor", this->m_AmbientSkyColor);
		archive.AddNVP("m_AmbientEquatorColor", this->m_AmbientEquatorColor);
		archive.AddNVP("m_AmbientGroundColor", this->m_AmbientGroundColor);
		archive.AddNVP("m_AmbientIntensity", this->m_AmbientIntensity);
		archive.AddNVP("m_AmbientMode", this->m_AmbientMode);
		archive.AddNVP("m_SubtractiveShadowColor", this->m_SubtractiveShadowColor);
		archive.AddNVP("m_SkyboxMaterial", this->m_SkyboxMaterial);
		archive.AddNVP("m_HaloStrength", this->m_HaloStrength);
		archive.AddNVP("m_FlareStrength", this->m_FlareStrength);
		archive.AddNVP("m_FlareFadeSpeed", this->m_FlareFadeSpeed);
		archive.AddNVP("m_Sun", this->m_Sun);
		archive.AddNVP("m_IndirectSpecularColor", this->m_IndirectSpecularColor);
	}

	void RenderSettings::Serialize(OutputArchive& archive) const
	{
		Object::Serialize(archive);
		archive.AddNVP("m_ObjectHideFlags", this->m_ObjectHideFlags);
		archive.AddNVP("m_Fog", this->m_Fog);
		archive.AddNVP("m_FogColor", this->m_FogColor);
		archive.AddNVP("m_FogMode", this->m_FogMode);
		archive.AddNVP("m_FogDensity", this->m_FogDensity);
		archive.AddNVP("m_LinearFogStart", this->m_LinearFogStart);
		archive.AddNVP("m_LinearFogEnd", this->m_LinearFogEnd);
		archive.AddNVP("m_AmbientSkyColor", this->m_AmbientSkyColor);
		archive.AddNVP("m_AmbientEquatorColor", this->m_AmbientEquatorColor);
		archive.AddNVP("m_AmbientGroundColor", this->m_AmbientGroundColor);
		archive.AddNVP("m_AmbientIntensity", this->m_AmbientIntensity);
		archive.AddNVP("m_AmbientMode", this->m_AmbientMode);
		archive.AddNVP("m_SubtractiveShadowColor", this->m_SubtractiveShadowColor);
		archive.AddNVP("m_SkyboxMaterial", this->m_SkyboxMaterial);
		archive.AddNVP("m_HaloStrength", this->m_HaloStrength);
		archive.AddNVP("m_FlareStrength", this->m_FlareStrength);
		archive.AddNVP("m_FlareFadeSpeed", this->m_FlareFadeSpeed);
		archive.AddNVP("m_Sun", this->m_Sun);
		archive.AddNVP("m_IndirectSpecularColor", this->m_IndirectSpecularColor);
	}


//}
