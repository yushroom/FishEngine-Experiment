#pragma once

#include <FishEngine/Math/Vector2.hpp>
#include <FishEngine/Math/Vector3.hpp>
#include <FishEngine/Math/Vector4.hpp>
#include <FishEngine/Math/Quaternion.hpp>
#include <FishEngine/Prefab.hpp>

#include <FishEngine/Serialization/Archive.hpp>

namespace FishEngine
{
	// Vector2
	static InputArchive& operator>>(InputArchive& archive, Vector2& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		return archive;
	}
	static OutputArchive& operator<<(OutputArchive& archive, const Vector2& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		return archive;
	}

	// Vector3
	static InputArchive& operator>>(InputArchive& archive, Vector3& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		return archive;
	}
	static OutputArchive& operator<<(OutputArchive& archive, const Vector3& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		return archive;
	}

	// Vector4
	static InputArchive& operator>>(InputArchive& archive, Vector4& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}
	static OutputArchive& operator<<(OutputArchive& archive, const Vector4& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}

	// Quaternion
	static InputArchive& operator>>(InputArchive& archive, Quaternion& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}
	static OutputArchive& operator<<(OutputArchive& archive, const Quaternion& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}

	// Modification
	static InputArchive& operator>>(InputArchive& archive, Modification& t)
	{
		archive.AddNVP("target", t.target);
		archive.AddNVP("propertyPath", t.propertyPath);
		archive.AddNVP("value", t.value);
		archive.AddNVP("objectReference", t.objectReference);
		return archive;
	}
	static OutputArchive& operator<<(OutputArchive& archive, const Modification& t)
	{
		archive.AddNVP("target", t.target);
		archive.AddNVP("propertyPath", t.propertyPath);
		archive.AddNVP("value", t.value);
		archive.AddNVP("objectReference", t.objectReference);
		return archive;
	}

	// PrefabModification
	static InputArchive& operator>>(InputArchive& archive, PrefabModification& t)
	{
		archive.AddNVP("m_TransformParent", t.m_TransformParent);
		archive.AddNVP("m_Modifications", t.m_Modifications);
		archive.AddNVP("m_RemovedComponents", t.m_RemovedComponents);
		return archive;
	}
	static OutputArchive& operator<<(OutputArchive& archive, const PrefabModification& t)
	{
		archive.AddNVP("m_TransformParent", t.m_TransformParent);
		archive.AddNVP("m_Modifications", t.m_Modifications);
		archive.AddNVP("m_RemovedComponents", t.m_RemovedComponents);
		return archive;
	}

}


