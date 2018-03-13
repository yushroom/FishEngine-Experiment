#pragma once

#include <FishEngine/Component/Collider.hpp>
#include <FishEngine/Math/Vector3.hpp>

namespace FishEngine
{
	class BoxCollider : public Collider
	{
	public:
		enum { ClassID = 65 };
		BoxCollider() : Collider(ClassID) { }

		BoxCollider(const Vector3& center, const Vector3& size);

	private:
		Vector3 m_center{ 0, 0, 0 };
		Vector3 m_size{ 1, 1, 1 };

		virtual void CreatePhysicsShape() override;
	};
}