#pragma once

#include <FishEngine/Component.hpp>
#include <FishEngine/Math/Vector3.hpp>

namespace physx
{
	class PxShape;
	class PxRigidDynamic;
}

namespace FishEngine
{
	class Rigidbody : public Component
	{
	public:
		enum {ClassID = 54};
		Rigidbody() : Component(ClassID)
		{

		}

		void Start();
		void Update();
		void OnDestroy();

		void setUseGravity(bool value)
		{
			m_useGravity = value;
		}

		bool useGravity() const
		{
			return m_useGravity;
		}

		bool IsInitialized() const;
		void Initialize(physx::PxShape* shape);

		virtual Rigidbody* Clone() const override
		{
			abort();
		}

	private:
		friend class Rigidbody;
		float m_mass = 2;
		float m_drag = 0;
		float m_angularDrag = 0.05f;
		bool m_useGravity = true;
		bool m_isKinematic = false;

		Vector3 m_velocity{ 0, 0, 0 };

		physx::PxRigidDynamic* m_physxRigidDynamic = nullptr;
	};
}