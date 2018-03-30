#pragma once

#include "FishEngine2.hpp"

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
		if (classID == Prefab::ClassID)
			return CreateEmptyObject<Prefab>();
		else if (classID == GameObject::ClassID)
			return CreateEmptyObject<GameObject>();
		else if (classID == Transform::ClassID)
			return CreateEmptyObject<Transform>();
		else if (classID == Camera::ClassID)
			return CreateEmptyObject<Camera>();
		else if (classID == Light::ClassID)
			return CreateEmptyObject<Light>();
		else if (classID == RectTransform::ClassID)
			return CreateEmptyObject<RectTransform>();
		else if (classID == MeshFilter::ClassID)
			return CreateEmptyObject<MeshFilter>();
		else if (classID == MeshRenderer::ClassID)
			return CreateEmptyObject<MeshRenderer>();
		else if (classID == BoxCollider::ClassID)
			return CreateEmptyObject<BoxCollider>();
		else if (classID == SphereCollider::ClassID)
			return CreateEmptyObject<SphereCollider>();
		else if (classID == Rigidbody::ClassID)
			return CreateEmptyObject<Rigidbody>();
		return nullptr;
	}
}