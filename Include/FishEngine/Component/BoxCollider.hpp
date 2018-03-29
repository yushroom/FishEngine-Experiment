#pragma once

#include <FishEngine/Component/Collider.hpp>
#include <FishEngine/Math/Vector3.hpp>

namespace FishEngine
{
	class BoxCollider : public Collider
	{
	public:
		DeclareObject(BoxCollider, 65);
		
		BoxCollider() : Collider(ClassID, ClassName) { }
		
		Vector3 GetCenter() const { return m_Center; }
		void SetCenter(const Vector3& center) { m_Center = center; }
		
		Vector3 GetSize() const { return m_Size; }
		void SetSize(const Vector3& size) { m_Size = size; }

	private:
		Vector3 m_Center{ 0, 0, 0 };
		Vector3 m_Size{ 1, 1, 1 };

		virtual void CreatePhysicsShape() override;
	};
}
