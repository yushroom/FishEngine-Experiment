#include <FishEngine/System/PhysicsSystem.hpp>
#include <FishEngine/Debug.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/Component/BoxCollider.hpp>
#include <FishEngine/Component/SphereCollider.hpp>

#include <FishEngine/Physics/PhysxAPI.hpp>

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


namespace FishEngine
{
	void PhysicsSystem::Init()
	{
		static PxDefaultAllocator		gAllocator;
		static FishEnginePhysxErrorCallback	gErrorCallback;
		auto& p = PhysxWrap::GetInstance();
		p.foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
		if (p.foundation == nullptr) {
			LogError("[PhysX] create foundation failed");
			abort();
		}

		const char*     pvd_host_ip = "127.0.0.1";  // IP of the PC which is running PVD
		int             port = 5425;         // TCP port to connect to, where PVD is listening
		unsigned int    timeout = 100;          // timeout in milliseconds to wait for PVD to respond,
												// consoles and remote PCs need a higher timeout.

		p.pvd = PxCreatePvd(*p.foundation);
		auto transport = PxDefaultPvdSocketTransportCreate(pvd_host_ip, port, timeout);
		p.pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
		// TODO: release PVD

		bool recordMemoryAllocations = false;
		PxTolerancesScale scale;
		p.physics = PxCreatePhysics(PX_PHYSICS_VERSION, *p.foundation, scale, recordMemoryAllocations, p.pvd);

		PxSceneDesc sceneDesc(p.physics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		p.dispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = p.dispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
		p.scene = p.physics->createScene(sceneDesc);
		p.material = p.physics->createMaterial(0.5f, 0.5f, 0.6f);
	}

	void PhysicsSystem::Start()
	{
		auto scene = SceneManager::GetActiveScene();
		
		auto colliders = scene->FindComponents<BoxCollider>();
		for (auto c : colliders)
		{
			c->Start();
		}

		{
			auto colliders = scene->FindComponents<SphereCollider>();
			for (auto c : colliders)
			{
				c->Start();
			}
		}
		
		auto rbs = scene->FindComponents<Rigidbody>();
		for (auto o : rbs)
		{
			o->Start();
		}
	}

	void PhysicsSystem::FixedUpdate()
	{
		auto& p = PhysxWrap::GetInstance();
		p.scene->simulate(1.0f / 30.f);
		p.scene->fetchResults(true);

		auto scene = SceneManager::GetActiveScene();
		auto rbs = scene->FindComponents<Rigidbody>();
		for (auto o : rbs)
		{
			o->Update();
		}

	}

	void PhysicsSystem::Clean()
	{
		auto& p = PhysxWrap::GetInstance();
		p.scene->release();
		p.dispatcher->release();
		p.physics->release();
		auto t = p.pvd->getTransport();
		p.pvd->release();
		t->release();
		p.foundation->release();

		LogInfo("Clean up PhysX.");
	}
}
