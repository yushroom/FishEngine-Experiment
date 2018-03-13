#include <FishEngine/System/PhysicsSystem.hpp>
#include <FishEngine/Debug.hpp>

#include <PxPhysicsAPI.h>
//#include <pvd/PxPvd.h>

#include <FishEngine/Component/Rigidbody.hpp>

using namespace FishEngine;
using namespace physx;

class FishEnginePhysxErrorCallback : public PxErrorCallback
{
public:
	virtual void reportError(PxErrorCode::Enum code,
		const char* message,
		const char* file,
		int line) override
	{
		//if (code & PxErrorCode::eDEBUG_INFO != 0)
		LogError(Format("PyhsX Error[file {} at line {}]: {}", file, line, message));
	}
};

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics = NULL;
PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene = NULL;
PxMaterial*				gMaterial = NULL;
PxPvd * gPvd = NULL;

namespace FishEngine
{
	void PhysicsSystem::Init()
	{
		static PxDefaultAllocator		gAllocator;
		static FishEnginePhysxErrorCallback	gErrorCallback;
		gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
		if (gFoundation == nullptr) {
			LogError("[PhysX] create foundation failed");
			abort();
		}

		bool recordMemoryAllocations = false;

		//gPvd = PxCreatePvd(*gFoundation);
		//PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		//mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		PxTolerancesScale scale;
 		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, scale, recordMemoryAllocations);

		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		gDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = gDispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
		gScene = gPhysics->createScene(sceneDesc);
		gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	}

	void PhysicsSystem::Start()
	{
		auto & rbs = Object::FindObjectsOfType<Rigidbody>();
		for (auto o : rbs)
		{
			auto rb = (Rigidbody*)o;
			rb->Start();
		}
	}

	void PhysicsSystem::FixedUpdate()
	{
		gScene->simulate(1.0f / 30.f);
		gScene->fetchResults(true);
	}

	void PhysicsSystem::Clean()
	{
		gScene->release();
		gDispatcher->release();
		gPhysics->release();
		if (gPvd != nullptr)
			gPvd->release();
		gFoundation->release();

		LogInfo("Clean up PhysX.");
	}
}