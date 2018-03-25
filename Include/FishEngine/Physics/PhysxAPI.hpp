#pragma once

#if defined(_DEBUG) || defined(NDEBUG)
#else
#	define _DEBUG 1
#endif
#include <PxPhysicsAPI.h>

namespace FishEngine
{
	class PhysxWrap
	{
	public:
		static PhysxWrap& GetInstance()
		{
			static PhysxWrap instance;
			return instance;
		}

		physx::PxFoundation*			foundation = nullptr;
		physx::PxPhysics*				physics = nullptr;
		physx::PxDefaultCpuDispatcher*	dispatcher = nullptr;
		physx::PxScene*					scene = nullptr;
		physx::PxMaterial*				material = nullptr;
		physx::PxPvd*					pvd = nullptr;

	private:
		PhysxWrap() = default;
		PhysxWrap(PhysxWrap&) = delete;
		PhysxWrap& operator=(PhysxWrap&) = delete;
	};
}