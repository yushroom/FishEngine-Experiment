#include <FishEngine/System/AnimationSystem.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/Animation/Animation.hpp>

//#include <FishEngine/Gizmos.hpp>
//using namespace FishEngine;
//void DrawSkeleton(const std::map<std::string, Transform*> & skeleton)
//{
//	for (auto&& p : skeleton)
//	{
//		auto t = p.second;
//		auto parent = t->GetParent();
//		if (parent != nullptr)
//		{
//			Gizmos::DrawLine(parent->GetPosition(), t->GetPosition());
//		}
//	}
//}

void FishEngine::AnimationSystem::Start()
{
	auto scene = SceneManager::GetActiveScene();
	auto animations = scene->FindComponents<Animation>();
	for (auto animation : animations)
	{
		animation->Start();
	}
}



void FishEngine::AnimationSystem::Update()
{
	auto scene = SceneManager::GetActiveScene();
	auto animations = scene->FindComponents<Animation>();
	for (auto animation : animations)
	{
		animation->Update(0.03333f);
		//DrawSkeleton(animation->m_skeleton);
	}
}
