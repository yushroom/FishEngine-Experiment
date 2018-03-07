#include <FishEngine/FishEngine.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Script.hpp>
#include <FishEngine/System/UISystem.hpp>
#include <FishEngine/System/ScriptSystem.hpp>
#include <FishEngine/System/InputSystem.hpp>

#include <FishEngine/Render/Material.hpp>

namespace FishEngine
{
    void Start()
    {
        puts("======== Start ========");
		
		Material::StaticInit();
		InputSystem::GetInstance().Init();
		ScriptSystem::GetInstance().Init();
		UISystem::GetInstance().Init();
		for (auto o : Object::FindObjectsOfType<GameObject>())
		{
			auto go = (GameObject*)o;
			for (auto comp : go->m_components)
			{
				 if (comp->GetClassID() == Script::ClassID)
				 {
					 auto s = static_cast<Script*>(comp);
					 s->Start();
				 }
			}
        }
    }

    void Update()
    {
//		puts("======== Update ========");
		ScriptSystem::GetInstance().Update();
        for (auto o : Object::FindObjectsOfType<GameObject>())
        {
			auto go = (GameObject*)o;
            for (auto comp : go->m_components)
            {
//                comp->Update();
				if (comp->GetClassID() == Script::ClassID)
				{
					auto s = static_cast<Script*>(comp);
					s->Update();
				}
            }
        }
    }

//    void Run()
//    {
//        LOGF;
//        Start();
//        for (int i = 0; i < 10; ++i)
//        {
//            printf("======== Frame %d: ========\n", i);
//            Update();
//        }
//    }
	
	void Clean()
	{
		ScriptSystem::GetInstance().Clean();
		
		Material::StaticClean();
		
		// check memory leak
		int a = Object::GetInstanceCounter();
		int b = Object::GetDeleteCounter();
		if (a != b)
		{
			printf("Memory Leak in FishEngine::Clean(): [%d] objects created but only [%d] objects deleted\n", a, b);
			
			for (auto& p : Object::GetAllObjects())
			{
				if (p.second.size() != 0)
				{
					printf("Class[ID:%d] has %lu obj\n", p.first, p.second.size());
				}
			}
		}
	}
}
