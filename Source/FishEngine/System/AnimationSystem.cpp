#include <FishEngine/System/AnimationSystem.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/Animation/Animation.hpp>
#include <FishEngine/Animation/Animator.hpp>


void FishEngine::AnimationSystem::Start()
{
	auto scene = SceneManager::GetActiveScene();
	auto animations = scene->FindComponents<Animation>();
	for (auto animation : animations)
	{
		animation->Start();
	}
	auto animators = scene->FindComponents<Animator>();
	for (auto animator : animators)
	{
		animator->Start();
	}
}


void FishEngine::AnimationSystem::Update()
{
	auto scene = SceneManager::GetActiveScene();
	auto animations = scene->FindComponents<Animation>();
	for (auto animation : animations)
	{
		animation->Update(0.03333f);
	}
	auto animators = scene->FindComponents<Animator>();
	for (auto animator : animators)
	{
		animator->Update(0.03333f);
	}
}
