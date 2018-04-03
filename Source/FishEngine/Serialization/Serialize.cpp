#include <FishEngine/Serialization/Serialize.hpp>

#include <FishEngine/FishEngine2.hpp>
#include <FishEngine/Serialization/Archive.hpp>

namespace FishEngine
{


	void Object::Deserialize(InputArchive& archive)
	{
		archive.AddNVP("m_ObjectHideFlags", this->m_ObjectHideFlags);
	}

	void Object::Serialize(OutputArchive& archive) const
	{
		archive.AddNVP("m_ObjectHideFlags", this->m_ObjectHideFlags);
	}


	void GameObject::Deserialize(InputArchive& archive)
	{
		Object::Deserialize(archive);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		auto components = this->m_Component;
		archive.AddNVP("m_Component", components);
		for (auto comp : components)
		{
			this->AddComponent(comp);
		}
		archive.AddNVP("m_Name", this->m_Name);
		archive.AddNVP("m_IsActive", this->m_IsActive);
	}

	void GameObject::Serialize(OutputArchive& archive) const
	{
		Object::Serialize(archive);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		archive.AddNVP("m_Component", this->m_Component);
		archive.AddNVP("m_Name", this->m_Name);
		archive.AddNVP("m_IsActive", this->m_IsActive);
	}


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


	void Component::Deserialize(InputArchive& archive)
	{
		Object::Deserialize(archive);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		archive.AddNVP("m_GameObject", this->m_GameObject);
		this->m_GameObject->AddComponent(this);
	}

	void Component::Serialize(OutputArchive& archive) const
	{
		Object::Serialize(archive);
		archive.AddNVP("m_PrefabParentObject", this->m_PrefabParentObject);
		archive.AddNVP("m_PrefabInternal", this->m_PrefabInternal);
		archive.AddNVP("m_GameObject", this->m_GameObject);
	}


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


	void Light::Deserialize(InputArchive& archive)
	{
		Behaviour::Deserialize(archive);
	}

	void Light::Serialize(OutputArchive& archive) const
	{
		Behaviour::Serialize(archive);
	}


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


	void MeshRenderer::Deserialize(InputArchive& archive)
	{
		Component::Deserialize(archive);
		archive.AddNVP("m_Materials", this->m_Materials);
	}

	void MeshRenderer::Serialize(OutputArchive& archive) const
	{
		Component::Serialize(archive);
		archive.AddNVP("m_Materials", this->m_Materials);
	}


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
}
