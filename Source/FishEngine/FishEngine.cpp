#include <FishEngine/FishEngine.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Script.hpp>
#include <FishEngine/System/UISystem.hpp>
#include <FishEngine/System/ScriptSystem.hpp>
#include <FishEngine/System/InputSystem.hpp>
#include <FishEngine/System/PhysicsSystem.hpp>
#include <FishEngine/System/AnimationSystem.hpp>
#include <FishEngine/Render/Material.hpp>
#include <FishEngine/Scene.hpp>

#include <exception>

#include <FishEngine/Render/Shader.hpp>
#include <FishEngine/Render/Mesh.hpp>
#include <FishEngine/Render/Material.hpp>
#include <FishEngine/Component/Camera.hpp>
#include <FishEngine/Component/Light.hpp>
#include <FishEngine/Component/MeshFilter.hpp>
#include <FishEngine/Component/MeshRenderer.hpp>
#include <FishEngine/ClassID.hpp>
#include <FishEngine/Render/TextureSampler.hpp>
#include <FishEngine/Render/Pipeline.hpp>

namespace FishEngine
{
	void Init()
	{
		puts("======== Init ========");
		//Debug::Init();
		//Debug::SetColorMode(true);
		TextureSampler::StaticInit();
		Mesh::StaticInit();
		Pipeline::StaticInit();
		Material::StaticInit();
		InputSystem::GetInstance().Init();
		ScriptSystem::GetInstance().Init();
		UISystem::GetInstance().Init();
	}

	void Start()
	{
		puts("======== Start ========");
		ScriptSystem::GetInstance().Start();

		//AnimationSystem::GetInstance().Start();

		for (auto o : Object::FindObjectsOfType<GameObject>())
		{
			auto go = (GameObject*)o;
			for (auto comp : go->GetAllComponents())
			{
				 if (comp->GetClassID() == Script::ClassID)
				 {
					 auto s = dynamic_cast<Script*>(comp);
					 s->Start();
				 }
			}
		}
	}
	
	void UpdateRecursively(GameObject* go)
	{
		for (auto comp : go->GetAllComponents())
		{
			if (comp->GetClassID() == Script::ClassID)
			{
				auto s = dynamic_cast<Script*>(comp);
				s->Update();
			}
		}
		for (auto child : go->GetTransform()->GetChildren())
		{
			UpdateRecursively(child->GetGameObject());
		}
	}

	void Update()
	{
//		puts("======== Update ========");
		ScriptSystem::GetInstance().Update();

		AnimationSystem::GetInstance().Update();
		
		auto scene = SceneManager::GetActiveScene();
		for (auto t : scene->GetRootTransforms())
		{
			UpdateRecursively(t->GetGameObject());
		}
		
		PhysicsSystem::GetInstance().FixedUpdate();
	}
	
	void Clean()
	{
		Material::StaticClean();
		SceneManager::StaticClean();
		ScriptSystem::GetInstance().Clean();	// put this after Scene::Clean
		AssetManager::GetInstance().ClearAll();
		
		// check memory leak
		int a = Object::GetInstanceCounter();
		int b = Object::GetDeleteCounter();
		if (a != b)
		{
			LogError(Format("Memory Leak in FishEngine::Clean(): [{}] objects created but only [{}] objects deleted", a, b));
			
			for (auto& p : Object::GetAllObjects())
			{
				if (p.second.size() != 0)
				{
//					int instanceID = (*p.second.begin())->GetInstanceID();
					LogError(Format("Class[ID:{}, name:{}] has {} obj", p.first, GetNameByClassID(p.first), p.second.size()));
					if (p.first == GameObject::ClassID)
					{
						for (auto go : p.second)
						{
							printf("GameObject %s, %d\n", go->GetName().c_str(), go->GetInstanceID());
						}
					}
				}
			}
			throw std::runtime_error("Memory Leak!");
		}
	}
}
