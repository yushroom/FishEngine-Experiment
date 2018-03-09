#include <FishEditor/EditorApplication.hpp>

#include <FishEngine/GameApp.hpp>
#include <FishEngine/Screen.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/System/RenderSystem.hpp>
#include <FishEngine/System/UISystem.hpp>

class EditorInternalApp : public FishEngine::AbstractGameApp
{
public:
	EditorInternalApp()
	{

	}

	int Run()
	{
		return 0;
	}

	virtual void Init() override
	{
		FishEngine::Init();
	}

	virtual void Start() override
	{
		FishEngine::Start();
	}

	virtual void Update() override
	{
		FishEngine::Update();
		DrawScene();
	}

	virtual void Shutdown() override
	{

	}

	void DrawScene()
	{
		FishEngine::RenderSystem::GetInstance().Update();
		FishEngine::UISystem::GetInstance().BeginDraw();
		FishEngine::UISystem::GetInstance().Update();
		FishEngine::UISystem::GetInstance().AfterDraw();
	}

	//virtual void Resize(int width, int height) override
	//{
	//	FishEngine::Screen::SetResolution(width, height, false);
	//}
};


namespace FishEditor
{
	void EditorApplication::Init()
	{
		m_app = new EditorInternalApp();
		m_app->Init();
	}

	void EditorApplication::Update()
	{
		if (m_IsPlaying)
		{
			FishEngine::Update();
			FishEngine::RenderSystem::GetInstance().Update();
			FishEngine::UISystem::GetInstance().BeginDraw();
			FishEngine::UISystem::GetInstance().Update();
			FishEngine::UISystem::GetInstance().AfterDraw();
		}
		else
		{
			FishEngine::RenderSystem::GetInstance().Update();
			FishEngine::UISystem::GetInstance().BeginDraw();
			FishEngine::UISystem::GetInstance().Update();
			FishEngine::UISystem::GetInstance().AfterDraw();
		}
	}

	void EditorApplication::Play()
	{
		m_app->Start();
		m_IsPlaying = true;
	}

	void EditorApplication::Stop()
	{
		m_IsPlaying = false;
	}

	void EditorApplication::Pause()
	{
	}

	void EditorApplication::NextFrame()
	{
	}
}