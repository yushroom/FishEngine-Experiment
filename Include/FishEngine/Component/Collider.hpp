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
		DeclareObject(Collider, 56);

		Collider(int classID, const char* className) : Component(classID, className) { }
		
		void Start();

		bool Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance);

		physx::PxShape* GetPhysicsShape() { return m_physxShape; }
		virtual void CreatePhysicsShape() = 0;

	protected:
		// Enabled Colliders will collide with other colliders, disabled Colliders won't.
		bool    m_Enabled = true;

		// Is the collider a trigger?
		bool    m_IsTrigger = false;

		physx::PxShape* m_physxShape = nullptr;
	};
}
