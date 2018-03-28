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

namespace FishEngine
{

	class Archive
	{
	public:
		template<class T>
		void Add(const char* name, T t)
		{

		}
	};


	void Object::Serialize(Archive& archive) const
	{
		archive.Add("m_ObjectHideFlags", this->m_ObjectHideFlags);
	}

	void GameObject::Serialize(Archive& archive) const
	{
		Object::Serialize(archive);
		archive.Add("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.Add("m_PrefabInternal", this->m_PrefabInternal);
		archive.Add("m_Component", this->m_Component);
		archive.Add("m_Name", this->m_Name);
		archive.Add("m_IsActive", this->m_IsActive);
	}

	void Prefab::Serialize(Archive& archive) const
	{
		Object::Serialize(archive);
		archive.Add("m_ParentPrefab", this->m_ParentPrefab);
		archive.Add("m_IsPrefabParent", this->m_IsPrefabParent);
	}

	void Component::Serialize(Archive& archive) const
	{
		Object::Serialize(archive);
		archive.Add("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.Add("m_PrefabInternal", this->m_PrefabInternal);
		archive.Add("m_GameObject", this->m_GameObject);
	}

	void Transform::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.Add("m_LocalRotation", this->m_LocalRotation);
		archive.Add("m_LocalPosition", this->m_LocalPosition);
		archive.Add("m_LocalScale", this->m_LocalScale);
		archive.Add("m_Children", this->m_Children);
		archive.Add("m_Father", this->m_Father);
		archive.Add("m_RootOrder", this->m_RootOrder);
	}

	void MeshFilter::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.Add("m_Mesh", this->m_Mesh);
	}

	void MeshRenderer::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.Add("m_Material", this->m_Material);
	}

	void Collider::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.Add("m_IsTrigger", this->m_IsTrigger);
		archive.Add("m_Enabled", this->m_Enabled);
	}

	void BoxCollider::Serialize(Archive& archive) const
	{
		Collider::Serialize(archive);
		archive.Add("m_Size", this->m_Size);
		archive.Add("m_Center", this->m_Center);
	}

	void SphereCollider::Serialize(Archive& archive) const
	{
		Collider::Serialize(archive);
		archive.Add("m_Radius", this->m_Radius);
		archive.Add("m_Center", this->m_Center);
	}

	void Rigidbody::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.Add("m_Mass", this->m_Mass);
		archive.Add("m_Drag", this->m_Drag);
		archive.Add("m_AngularDrag", this->m_AngularDrag);
		archive.Add("m_UseGravity", this->m_UseGravity);
		archive.Add("m_IsKinematic", this->m_IsKinematic);
	}

	void Camera::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.Add("m_FarClipPlane", this->m_FarClipPlane);
		archive.Add("m_NearClipPlane", this->m_NearClipPlane);
		archive.Add("m_FieldOfView", this->m_FieldOfView);
		archive.Add("m_Orthographic", this->m_Orthographic);
		archive.Add("m_OrthographicSize", this->m_OrthographicSize);
	}

	void Behaviour::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.Add("m_Enabled", this->m_Enabled);
	}

	void Light::Serialize(Archive& archive) const
	{
		Behaviour::Serialize(archive);
	}

	void RectTransform::Serialize(Archive& archive) const
	{
		Component::Serialize(archive);
		archive.Add("m_AnchorMin", this->m_AnchorMin);
		archive.Add("m_AnchorMax", this->m_AnchorMax);
		archive.Add("m_AnchoredPosition", this->m_AnchoredPosition);
		archive.Add("m_SizeDelta", this->m_SizeDelta);
		archive.Add("m_Pivot", this->m_Pivot);
	}

}