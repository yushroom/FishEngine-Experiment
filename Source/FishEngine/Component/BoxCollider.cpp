#include <FishEngine/Component/BoxCollider.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/Physics/PhysxAPI.hpp>

using namespace physx;

namespace FishEngine
{
	void BoxCollider::CreatePhysicsShape()
	{
		auto& p = PhysxWrap::GetInstance();
		auto s = GetTransform()->GetLossyScale() * m_Size * 0.5f;
		m_physxShape = p.physics->createShape(PxBoxGeometry(s.x, s.y, s.z), *p.material);
		//m_physxShape = p.physics->createShape(PxPlaneGeometry(), *p.material);
		m_physxShape->setLocalPose(PxTransform(m_Center.x, m_Center.y, m_Center.z));
		//auto offset = m_physxShape->getContactOffset();
		//m_physxShape->setContactOffset(0);
	}
}
