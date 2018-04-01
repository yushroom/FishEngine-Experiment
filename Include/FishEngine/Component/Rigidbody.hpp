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
		DeclareObject(Rigidbody, 54);
		
		Rigidbody() : Component(ClassID, ClassName)
		{

		}

		~Rigidbody()
		{

		}

		void Start();
		void Update();
		void OnDestroy();

		bool IsInitialized() const;
		void Initialize(physx::PxShape* shape);

		
		float GetMass() const { return m_Mass; }
		void SetMass(float value) { m_Mass = value; }
		
		float GetDrag() const { return m_Drag; }
		void SetDrag(float value) { m_Drag = value; }
		
		float GetAngularDrag() const { return m_AngularDrag; }
		void SetAngularDrag(float value) { m_AngularDrag = value; }
		
		bool GetUseGravity() const { return m_UseGravity; }
		void SetUseGravity(bool value) { m_UseGravity = value; }
		
		bool GetIsKinematic() const { return m_IsKinematic; }
		void SetIsKinematic(bool value) { m_IsKinematic = value; }

		
	private:
		float m_Mass = 2;
		float m_Drag = 0;
		float m_AngularDrag = 0.05f;
		bool m_UseGravity = true;
		bool m_IsKinematic = false;

		Vector3 m_velocity{ 0, 0, 0 };

		physx::PxRigidDynamic* m_physxRigidDynamic = nullptr;
	};
}
