#include <FishEngine/Component/Rigidbody.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Component/BoxCollider.hpp>
#include <FishEngine/Component/SphereCollider.hpp>
#include <FishEngine/Physics/PhysxAPI.hpp>

using namespace physx;

inline physx::PxVec3 PxVec3(const FishEngine::Vector3& v)
{
	return physx::PxVec3(v.x, v.y, v.z);
}

namespace FishEngine
{
	void Rigidbody::Initialize(PxShape* shape)
	{
		auto& px = PhysxWrap::GetInstance();
		auto t = GetTransform();
		const Vector3& p = t->GetPosition();
		const auto& q = t->GetRotation();
		m_physxRigidDynamic = px.physics->createRigidDynamic(PxTransform(p.x, p.y, p.z, PxQuat(q.x, q.y, q.z, q.w)));
		m_physxRigidDynamic->userData = (void*)this;
		m_physxRigidDynamic->setMass(m_Mass);
		if (shape)
		{
			m_physxRigidDynamic->attachShape(*shape);
			shape->release();
		}
	}

	void Rigidbody::Start()
	{
		auto& px = PhysxWrap::GetInstance();
		Collider* collider = GetGameObject()->GetComponent<Collider>();
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
		px.scene->addActor(*m_physxRigidDynamic);
	}

	void Rigidbody::Update()
	{
		const auto& t = m_physxRigidDynamic->getGlobalPose();
		GetTransform()->SetPosition(t.p.x, t.p.y, t.p.z);
		GetTransform()->SetRotation(Quaternion(t.q.x, t.q.y, t.q.z, t.q.w));
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
