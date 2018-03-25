
#include <FishEditor/EditorApplication.hpp>


#include <FishEngine/GameApp.hpp>
#include <FishEngine/Screen.hpp>
//#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/System/RenderSystem.hpp>
#include <FishEngine/System/UISystem.hpp>
#include <FishEngine/System/PhysicsSystem.hpp>
#include <FishEngine/Scene.hpp>

#include <FishEditor/Selection.hpp>
#include <FishEditor/AssetDatabase.hpp>
#include <FishEditor/GameView.hpp>


#include <pybind11/embed.h>
namespace py = pybind11;

class EditorInternalApp : public FishEngine::AbstractGameApp
{
public:
	EditorInternalApp() = default;

	int Run()
	{
		return 0;
	}

    void Init() override
	{
		FishEngine::Init();
	}

    void Start() override
	{
		FishEngine::Start();
	}

    void Update() override
	{
		FishEngine::Update();
		DrawScene();
	}

    void Shutdown() override
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
	void EditorApplication::OpenProject(const std::string& projectPath)
	{
		AssetDatabase::s_AssetRootDir = new FileNode(projectPath);
//		m_ApplicationPath = projectPath;
        OnProjectOpened();
	}

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
		gameView->m_Framebuffer.Bind();
		
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
		
		gameView->m_Framebuffer.Unbind();
	}

	void EditorApplication::Play()
	{
//		m_app->Start();
		m_IsPlaying = true;
		auto scene = FishEngine::SceneManager::GetActiveScene();
		m_currentScene = scene;

		auto app = py::module::import("app");
		app.attr("Save")();
//		scene = scene->Clone();
//		FishEngine::SceneManager::SetActiveScene(scene);
		
		Selection::SetActiveTransform(nullptr);
		FishEngine::PhysicsSystem::GetInstance().Init();
		FishEngine::PhysicsSystem::GetInstance().Start();
	}

	void EditorApplication::Stop()
	{
		FishEngine::PhysicsSystem::GetInstance().Clean();

		m_IsPlaying = false;
		auto scene = FishEngine::SceneManager::GetActiveScene();
		scene->Clean();
		delete scene;
		FishEngine::SceneManager::SetActiveScene(m_currentScene);
//		scene = FishEngine::SceneManager::GetActiveScene();
		//auto app = py::module::import("app");
		//app.attr("Restore")();
		
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
