#pragma once

#include <FishEngine/Component/Collider.hpp>
#include <FishEngine/Math/Vector3.hpp>

namespace FishEngine
{
	class SphereCollider : public Collider
	{
	public:
		enum { ClassID = 135 };
		SphereCollider() : Collider(ClassID) { }

		Vector3 GetCenter() const { return m_Center; }
		void SetCenter(const Vector3& center) { m_Center = center; }

		float GetRadius() const { return m_Radius; }
		void SetRadius(float radius) { m_Radius = radius; }

	private:
		Vector3 m_Center{ 0, 0, 0 };
		float m_Radius = 1;

		virtual void CreatePhysicsShape() override;
	};
}
