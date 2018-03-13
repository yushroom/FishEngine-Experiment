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
		
		Vector3 GetCenter() const { return m_center; }
		void SetCenter(const Vector3& center) { m_center = center; }
		
		Vector3 GetSize() const { return m_size; }
		void SetSize(const Vector3& size) { m_size = size; }

	private:
		Vector3 m_center{ 0, 0, 0 };
		Vector3 m_size{ 1, 1, 1 };

		virtual void CreatePhysicsShape() override;
	};
}
