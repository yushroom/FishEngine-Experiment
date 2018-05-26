#pragma once

#include <FishEngine/Math/Vector2.hpp>
#include <FishEngine/Math/Vector3.hpp>
#include <FishEngine/Math/Vector4.hpp>
#include <FishEngine/Math/Quaternion.hpp>
#include <FishEngine/Color.hpp>
#include <FishEngine/Prefab.hpp>

#include <FishEngine/Serialization/Archive.hpp>

namespace FishEngine
{
	// Vector2
	inline InputArchive& operator>>(InputArchive& archive, Vector2& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		return archive;
	}
	inline OutputArchive& operator<<(OutputArchive& archive, const Vector2& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		return archive;
	}

	// Vector3
	inline InputArchive& operator>>(InputArchive& archive, Vector3& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		return archive;
	}
	inline OutputArchive& operator<<(OutputArchive& archive, const Vector3& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		return archive;
	}

	// Vector4
	inline InputArchive& operator>>(InputArchive& archive, Vector4& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}
	inline OutputArchive& operator<<(OutputArchive& archive, const Vector4& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}

	// Quaternion
	inline InputArchive& operator>>(InputArchive& archive, Quaternion& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}
	inline OutputArchive& operator<<(OutputArchive& archive, const Quaternion& t)
	{
		archive.AddNVP("x", t.x);
		archive.AddNVP("y", t.y);
		archive.AddNVP("z", t.z);
		archive.AddNVP("w", t.w);
		return archive;
	}

	// Color
	inline InputArchive& operator>>(InputArchive& archive, Color& t)
	{
		archive.AddNVP("r", t.r);
		archive.AddNVP("g", t.g);
		archive.AddNVP("b", t.b);
		archive.AddNVP("a", t.a);
		return archive;
	}
	inline OutputArchive& operator<<(OutputArchive& archive, const Color& t)
	{
		archive.AddNVP("r", t.r);
		archive.AddNVP("g", t.g);
		archive.AddNVP("b", t.b);
		archive.AddNVP("a", t.a);
		return archive;
	}

	// Modification
	inline InputArchive& operator>>(InputArchive& archive, Modification& t)
	{
		archive.AddNVP("target", t.target);
		archive.AddNVP("propertyPath", t.propertyPath);
		archive.AddNVP("value", t.value);
		archive.AddNVP("objectReference", t.objectReference);
		return archive;
	}
	inline OutputArchive& operator<<(OutputArchive& archive, const Modification& t)
	{
		archive.AddNVP("target", t.target);
		archive.AddNVP("propertyPath", t.propertyPath);
		archive.AddNVP("value", t.value);
		archive.AddNVP("objectReference", t.objectReference);
		return archive;
	}

	// PrefabModification
	inline InputArchive& operator>>(InputArchive& archive, PrefabModification& t)
	{
		archive.AddNVP("m_TransformParent", t.m_TransformParent);
		archive.AddNVP("m_Modifications", t.m_Modifications);
		archive.AddNVP("m_RemovedComponents", t.m_RemovedComponents);
		return archive;
	}
	inline OutputArchive& operator<<(OutputArchive& archive, const PrefabModification& t)
	{
		archive.AddNVP("m_TransformParent", t.m_TransformParent);
		archive.AddNVP("m_Modifications", t.m_Modifications);
		archive.AddNVP("m_RemovedComponents", t.m_RemovedComponents);
		return archive;
	}

}


