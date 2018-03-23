#include <FishEngine/Component/SphereCollider.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/Physics/PhysxAPI.hpp>

using namespace physx;

namespace FishEngine
{
	void SphereCollider::CreatePhysicsShape()
	{
		auto& p = PhysxWrap::GetInstance();
		m_physxShape = p.physics->createShape(PxSphereGeometry(m_Radius), *p.material);
		m_physxShape->setLocalPose(PxTransform(m_Center.x, m_Center.y, m_Center.z));
	}
}