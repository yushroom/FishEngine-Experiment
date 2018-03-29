#pragma once

#include <FishEngine/Component/Collider.hpp>
#include <FishEngine/Math/Vector3.hpp>

namespace FishEngine
{
	class SphereCollider : public Collider
	{
	public:
		DeclareObject(SphereCollider, 135);
		
		SphereCollider() : Collider(ClassID, ClassName) { }

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
