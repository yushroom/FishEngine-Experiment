#pragma once

#include <FishEngine/GameApp.hpp>
#include <FishEngine/Screen.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/System/RenderSystem.hpp>
#include <FishEngine/System/UISystem.hpp>



class SceneViewApp : public FishEngine::GameApp
{
public:
	SceneViewApp()
	{

	}

	virtual int Run() override
	{
		return 0;
	}

	virtual void Init() override
	{
		FishEngine::Start();
	}

	virtual void Update() override
	{
		FishEngine::Update();
		FishEngine::RenderSystem::GetInstance().Update();
		FishEngine::UISystem::GetInstance().BeginDraw();
		FishEngine::UISystem::GetInstance().Update();
		FishEngine::UISystem::GetInstance().AfterDraw();
	}

	virtual void Resize(int width, int height) override
	{
		FishEngine::Screen::SetResolution(width, height, false);
	}
};
