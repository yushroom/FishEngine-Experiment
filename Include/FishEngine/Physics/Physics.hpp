#pragma once

#include "../Math/Vector3.hpp"
#include "../Math/Vector2.hpp"

namespace FishEngine
{
	class Collider;

	struct RaycastHit
	{
		Vector3 m_Point;
		float m_Distance;
		Vector3 m_Normal;
		Collider* m_Collider;
		Vector2 m_UV;
	};

	class Physics
	{
	public:
		Physics() = delete;

		static bool Raycast(const Vector3& origin, const Vector3& direction, float maxDistance, int layerMask);
		static bool Raycast(const Vector3& origin, const Vector3& direction, RaycastHit& hitInfo, float maxDistance, int layerMask);
	};
}