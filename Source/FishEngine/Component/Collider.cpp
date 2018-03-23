#include <FishEngine/Component/Collider.hpp>
#include <FishEngine/Component/Rigidbody.hpp>
#include <FishEngine/GameObject.hpp>

#include <FishEngine/Physics/PhysxAPI.hpp>

using namespace physx;

namespace FishEngine
{
	void Collider::Start()
	{
		auto& px = PhysxWrap::GetInstance();
		CreatePhysicsShape();
		m_physxShape->userData = (void*)this;
		//m_physxShape->setRestOffset(1);
		auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();
		if (rigidbody == nullptr)
		{
			auto t = GetTransform();
			auto p = t->GetPosition();
			auto q = t->GetRotation();
			auto rigidStatic = PxCreateStatic(*px.physics, PxTransform(p.x, p.y, p.z, PxQuat(q.x, q.y, q.z, q.w)), *m_physxShape);
			px.scene->addActor(*rigidStatic);
		}
	}


	bool Collider::Raycast(const Ray & ray, RaycastHit & hitInfo, float maxDistance)
	{
		abort();
		return false;
	}
}
