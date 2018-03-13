#include <FishEngine/Component/Rigidbody.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Component/BoxCollider.hpp>

#define _DEBUG 1
#include <PxPhysicsAPI.h>

using namespace physx;

extern physx::PxPhysics*				gPhysics;
extern physx::PxScene*				gScene;
extern physx::PxMaterial*				gMaterial;

inline physx::PxVec3 PxVec3(const FishEngine::Vector3& v)
{
	return physx::PxVec3(v.x, v.y, v.z);
}

namespace FishEngine
{
	void Rigidbody::Initialize(PxShape* shape)
	{
		auto t = GetTransform();
		const Vector3& p = t->GetPosition();
		const auto& q = t->GetRotation();
		m_physxRigidDynamic = gPhysics->createRigidDynamic(PxTransform(p.x, p.y, p.z, PxQuat(q.x, q.y, q.z, q.w)));
		if (shape)
		{
			m_physxRigidDynamic->attachShape(*shape);
			shape->release();
		}
	}

	void Rigidbody::Start()
	{
		auto collider = GetGameObject()->GetComponent<BoxCollider>();
		if (collider != nullptr)
			Initialize(collider->GetPhysicsShape());
		else
			Initialize(nullptr);

		auto t = GetTransform();
		const Vector3 p = t->GetPosition();
		const auto& q = t->GetRotation();
		m_physxRigidDynamic->setGlobalPose(PxTransform(p.x, p.y, p.z, PxQuat(q.x, q.y, q.z, q.w)));
		PxRigidBodyExt::updateMassAndInertia(*m_physxRigidDynamic, 10.0f);
		m_physxRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !m_UseGravity);
		gScene->addActor(*m_physxRigidDynamic);
	}

	void Rigidbody::Update()
	{
		//m_physxRigidDynamic->user
		const auto& t = m_physxRigidDynamic->getGlobalPose();
		//const auto& pt = t.actor2World;
		GetTransform()->_setPosition(t.p.x, t.p.y, t.p.z);
		GetTransform()->SetLocalRotation(Quaternion(t.q.x, t.q.y, t.q.z, t.q.w));
	}

	void Rigidbody::OnDestroy()
	{
		m_physxRigidDynamic = nullptr;
	}

	bool Rigidbody::IsInitialized() const
	{
		return m_physxRigidDynamic != nullptr;
	}

}
