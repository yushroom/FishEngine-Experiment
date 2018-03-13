#include <FishEngine/Component/BoxCollider.hpp>
#include <FishEngine/Transform.hpp>

#define _DEBUG 1
#include <PxPhysicsAPI.h>

using namespace physx;

extern physx::PxFoundation*			gFoundation;
extern physx::PxPhysics*				gPhysics;
extern physx::PxDefaultCpuDispatcher*	gDispatcher;
extern physx::PxScene*				gScene;
extern physx::PxMaterial*				gMaterial;

namespace FishEngine
{
	BoxCollider::BoxCollider(const Vector3& center, const Vector3& size)
		: Collider(ClassID), m_center(center), m_size(size)
	{
		//m_physxShape = gPhysics->createShape(PxBoxGeometry(size.x*0.5f, size.y*0.5f, size.z*0.5f), *gMaterial);
	}

	void FishEngine::BoxCollider::CreatePhysicsShape()
	{
		//if (m_physxShape == nullptr) {
		auto s = GetTransform()->GetLossyScale();
		m_physxShape = gPhysics->createShape(PxBoxGeometry(s.x*m_size.x*0.5f, s.y*m_size.y*0.5f, s.z*m_size.z*0.5f), *gMaterial);
		m_physxShape->setLocalPose(PxTransform(m_center.x, m_center.y, m_center.z));
		//}
	}
}
