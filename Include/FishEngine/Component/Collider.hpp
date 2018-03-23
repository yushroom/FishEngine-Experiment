#pragma once

#include <FishEngine/Component.hpp>
#include <FishEngine/Math/Ray.hpp>
#include "../Physics/RaycastHit.hpp"

namespace physx {
	class PxShape;
	class PxRigidDynamic;
}

namespace FishEngine
{
	class Collider : public Component
	{
	public:
		enum {ClassID = 56};

		Collider(int classID) : Component(classID) { }

		virtual Collider* Clone() const override
		{
			abort();
		}
		
		void Start();

		bool Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance);

		physx::PxShape* GetPhysicsShape() { return m_physxShape; }
		virtual void CreatePhysicsShape() = 0;

	protected:
		// Enabled Colliders will collide with other colliders, disabled Colliders won't.
		bool    m_enabled = true;

		// Is the collider a trigger?
		bool    m_isTrigger = false;

		physx::PxShape* m_physxShape = nullptr;
	};
}
