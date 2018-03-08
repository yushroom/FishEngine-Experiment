#pragma once

#include <FishEngine/GameApp.hpp>
#include <FishEngine/Screen.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/System/RenderSystem.hpp>
#include <FishEngine/System/UISystem.hpp>

#include <FishGUI/Render/FrameBuffer.hpp>

class SceneViewApp : public FishEngine::GameApp
{
public:
	SceneViewApp(FishGUI::Widget* widget) : m_widget(widget)
	{

	}

	virtual int Run() override
	{
		return 0;
	}

	virtual void Init() override
	{
		int w = m_widget->GetWidth();
		int h = m_widget->GetHeight();
		m_framebuffer.Init(w, h);
		FishEngine::Screen::SetResolution(w, h, false);
		FishEngine::Start();
	}

	virtual void Update() override
	{
		m_framebuffer.Bind();
		FishEngine::Update();
		FishEngine::RenderSystem::GetInstance().Update();
		FishEngine::UISystem::GetInstance().BeginDraw();
		FishEngine::UISystem::GetInstance().Update();
		FishEngine::UISystem::GetInstance().AfterDraw();
		m_framebuffer.Unbind();
	}

	virtual void Resize(int width, int height) override
	{
		FishEngine::Screen::SetResolution(width, height, false);
		m_framebuffer.Resize(width, height);
	}

private:
	FishGUI::Widget* m_widget = nullptr;
public:
	FishGUI::FrameBuffer m_framebuffer;
};
