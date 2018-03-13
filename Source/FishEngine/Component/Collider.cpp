#include <FishEngine/Component/Collider.hpp>
#include <FishEngine/Component/Rigidbody.hpp>
#include <FishEngine/GameObject.hpp>

#define _DEBUG 1
#include <PxPhysicsAPI.h>

using namespace physx;
extern physx::PxPhysics*    gPhysics;
extern physx::PxScene*      gScene;
extern physx::PxMaterial*   gMaterial;


namespace FishEngine
{
	void Collider::Start()
	{
		//m_physxShape = gPhysics->createShape(PxSphereGeometry(m_radius), *gMaterial);
		CreatePhysicsShape();
		auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();
		if (rigidbody == nullptr)
		{
			//PxRigidStatic* rigidStatic = PxCreatePlane(*gPhysics, PxPlane(0,1,0,0), *gMaterial);
			auto t = GetTransform();
			auto p = t->GetPosition();
			auto q = t->GetRotation();
			auto rigidStatic = PxCreateStatic(*gPhysics, PxTransform(p.x, p.y, p.z, PxQuat(q.x, q.y, q.z, q.w)), *m_physxShape);
			gScene->addActor(*rigidStatic);
		}
//        else {
//            rigidbody->Initialize(m_physxShape);
//        }
	}
}
