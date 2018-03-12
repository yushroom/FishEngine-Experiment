#include <FishEditor/EditorApplication.hpp>

#include <FishEditor/GameView.hpp>

#include <FishEngine/GameApp.hpp>
#include <FishEngine/Screen.hpp>
#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/System/RenderSystem.hpp>
#include <FishEngine/System/UISystem.hpp>
#include <FishEngine/Scene.hpp>

#include <FishEditor/Selection.hpp>

#include <pybind11/embed.h>
namespace py = pybind11;

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
//		m_app->Init();
		FishEngine::Init();
		FishEngine::Start();
	}

	void EditorApplication::Update()
	{
		auto gameView = GameView::GetCurrent();
		gameView->m_FrameBuffer.Bind();
		
		if (m_IsPlaying)
		{
			FishEngine::Update();
		}
		else
		{
		}
		FishEngine::RenderSystem::GetInstance().Update();
		FishEngine::UISystem::GetInstance().BeginDraw();
		FishEngine::UISystem::GetInstance().Update();
		FishEngine::UISystem::GetInstance().AfterDraw();
		
		gameView->m_FrameBuffer.Bind();
	}

	void EditorApplication::Play()
	{
//		m_app->Start();
		m_IsPlaying = true;
		auto app = py::module::import("demo1");
		app.attr("Save")();
//		auto scene = FishEngine::SceneManager::GetActiveScene();
//		m_currentScene = scene;
//		scene = scene->Clone();
//		FishEngine::SceneManager::SetActiveScene(scene);
		
		Selection::SetActiveTransform(nullptr);
	}

	void EditorApplication::Stop()
	{
		m_IsPlaying = false;
		//FishEngine::Clean();
//		auto scene = FishEngine::SceneManager::GetActiveScene();
//		scene->Clean();
//		delete scene;
//		FishEngine::SceneManager::SetActiveScene(m_currentScene);
		auto app = py::module::import("demo1");
		app.attr("Restore")();
		
		Selection::SetActiveTransform(nullptr);
	}

	void EditorApplication::Pause()
	{
		m_IsPlaying = false;
	}
	
	void EditorApplication::Resume()
	{
		m_IsPlaying = true;
	}
}
