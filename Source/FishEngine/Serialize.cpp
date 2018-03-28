#include <FishEngine/Object.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Component.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/Prefab.hpp>
#include <FishEngine/RectTransform.hpp>
#include <FishEngine/Component/MeshFilter.hpp>
#include <FishEngine/Component/MeshRenderer.hpp>
#include <FishEngine/Component/Collider.hpp>
#include <FishEngine/Component/BoxCollider.hpp>
#include <FishEngine/Component/SphereCollider.hpp>
#include <FishEngine/Component/Rigidbody.hpp>
#include <FishEngine/Component/Camera.hpp>
#include <FishEngine/Component/Behaviour.hpp>
#include <FishEngine/Component/Light.hpp>

#include <FishEngine/Archive.hpp>

namespace FishEngine
{

	void Object::Serialize(Archive& archive) const
	{
		archive.AddNVP("m_ObjectHideFlags", this->m_ObjectHideFlags);
	}

	void GameObject::Serialize(Archive& archive) const
	{
		Object::Serialize(archive);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		archive.AddNVP("m_Component", this->m_Component);
		archive.AddNVP("m_Name", this->m_Name);
		archive.AddNVP("m_IsActive", this->m_IsActive);
	}

	void Prefab::Serialize(Archive& archive) const
	{
		Object::Serialize(archive);
		archive.AddNVP("m_ParentPrefab", this->m_ParentPrefab);
		archive.AddNVP("m_IsPrefabParent", this->m_IsPrefabParent);
	}

	void Component::Serialize(Archive& archive) const
	{
		Object::Serialize(archive);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		archive.AddNVP("m_GameObject", this->m_GameObject);
	}

	void Transform::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_LocalRotation", this->m_LocalRotation);
		archive.AddNVP("m_LocalPosition", this->m_LocalPosition);
		archive.AddNVP("m_LocalScale", this->m_LocalScale);
		archive.AddNVP("m_Children", this->m_Children);
		archive.AddNVP("m_Father", this->m_Father);
		archive.AddNVP("m_RootOrder", this->m_RootOrder);
	}

	void Camera::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_FarClipPlane", this->m_FarClipPlane);
		archive.AddNVP("m_NearClipPlane", this->m_NearClipPlane);
		archive.AddNVP("m_FieldOfView", this->m_FieldOfView);
		archive.AddNVP("m_Orthographic", this->m_Orthographic);
		archive.AddNVP("m_OrthographicSize", this->m_OrthographicSize);
	}

	void Behaviour::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_Enabled", this->m_Enabled);
	}

	void Light::Serialize(Archive& archive) const
	{
		Behaviour::Serialize(archive);
	}

	void RectTransform::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_AnchorMin", this->m_AnchorMin);
		archive.AddNVP("m_AnchorMax", this->m_AnchorMax);
		archive.AddNVP("m_AnchoredPosition", this->m_AnchoredPosition);
		archive.AddNVP("m_SizeDelta", this->m_SizeDelta);
		archive.AddNVP("m_Pivot", this->m_Pivot);
	}

	void MeshFilter::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_Mesh", this->m_Mesh);
	}

	void MeshRenderer::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_Material", this->m_Material);
	}

	void Collider::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_IsTrigger", this->m_IsTrigger);
		archive.AddNVP("m_Enabled", this->m_Enabled);
	}

	void BoxCollider::Serialize(Archive& archive) const
	{
		Collider::Serialize(archive);
		archive.AddNVP("m_Size", this->m_Size);
		archive.AddNVP("m_Center", this->m_Center);
	}

	void SphereCollider::Serialize(Archive& archive) const
	{
		Collider::Serialize(archive);
		archive.AddNVP("m_Radius", this->m_Radius);
		archive.AddNVP("m_Center", this->m_Center);
	}

	void Rigidbody::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_Mass", this->m_Mass);
		archive.AddNVP("m_Drag", this->m_Drag);
		archive.AddNVP("m_AngularDrag", this->m_AngularDrag);
		archive.AddNVP("m_UseGravity", this->m_UseGravity);
		archive.AddNVP("m_IsKinematic", this->m_IsKinematic);
	}

}