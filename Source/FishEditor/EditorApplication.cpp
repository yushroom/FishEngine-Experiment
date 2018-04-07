#include <FishEditor/EditorApplication.hpp>


#include <FishEngine/GameApp.hpp>
#include <FishEngine/Screen.hpp>

#include <FishEngine/System/RenderSystem.hpp>
#include <FishEngine/System/UISystem.hpp>
#include <FishEngine/System/PhysicsSystem.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEditor/Selection.hpp>
#include <FishEditor/AssetDatabase.hpp>
#include <FishEditor/GameView.hpp>


#include <pybind11/embed.h>
#include <FishEngine/Application.hpp>

#include <FishEditor/UI/HierarchyView.hpp>

namespace py = pybind11;

using namespace FishEngine;

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
		FishEngine::Application::GetInstance().m_DataPath = projectPath+"/Assets";
		AssetDatabase::s_AssetRootDir = new FileNode(projectPath+"/Assets");
//		m_ApplicationPath = projectPath;
        OnProjectOpened();
	}

	void EditorApplication::Init()
	{
		m_app = new EditorInternalApp();
//		m_app->Init();
		FishEngine::Init();
		AssetDatabase::StaticInit();
		FishEngine::Start();
	}

	void EditorApplication::Update()
	{
		auto gameView = GameView::GetCurrent();
		gameView->m_Framebuffer.Bind();
		
		if (m_IsPlaying && !m_IsPaused)
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


	Object* GetOrDefault(const std::map<Object*, Object*>& dict, Object* key)
	{
		auto pos = dict.find(key);
		if (pos == dict.end())
			return nullptr;
		return pos->second;
	}


	void EditorApplication::Play()
	{
//		m_app->Start();
		m_IsPlaying = true;
		auto scene = FishEngine::SceneManager::GetActiveScene();
		m_currentScene = scene;

//		auto app = py::module::import("app");
//		app.attr("Save")();
//		std::map<Object*, Object*> memo;
		m_SceneObjectMemo.clear();
		scene = scene->CloneWithMemo(m_SceneObjectMemo);
		FishEngine::SceneManager::SetActiveScene(scene);

		auto selectTransform = Selection::GetActiveTransform();
		if (selectTransform != nullptr)
		{
			auto cloned = m_SceneObjectMemo[selectTransform]->As<Transform>();
			Selection::SetActiveTransform(cloned);
		}

		std::set<Transform*> unfolded;
		for (auto& o : m_HierarchyView->m_unfolded)
		{
			unfolded.insert(m_SceneObjectMemo[o]->As<Transform>());
		}
		m_HierarchyView->m_unfolded = unfolded;


		FishEngine::PhysicsSystem::GetInstance().Init();
		FishEngine::PhysicsSystem::GetInstance().Start();
	}


	void EditorApplication::Stop()
	{
		if (!m_IsPlaying)
			return;
		FishEngine::PhysicsSystem::GetInstance().Clean();

		auto t = Selection::GetActiveTransform();

		std::map<Object*, Object*> inverseMap;
		for (auto&& p : m_SceneObjectMemo)
		{
			inverseMap[p.second] = p.first;
		}

		if (t != nullptr)
		{
			auto originalT = GetOrDefault(inverseMap, t);
			Selection::SetActiveObject(originalT);
		}

		std::set<Transform*> unfolded;
		for (auto& o : m_HierarchyView->m_unfolded)
		{
			auto oo = GetOrDefault(inverseMap, o);
			if (oo != nullptr)
				unfolded.insert(oo->As<Transform>());
		}
		m_HierarchyView->m_unfolded = unfolded;



		m_IsPlaying = false;
		auto scene = FishEngine::SceneManager::GetActiveScene();
//		scene->Clean();
		delete scene;
		FishEngine::SceneManager::SetActiveScene(m_currentScene);
//		scene = FishEngine::SceneManager::GetActiveScene();
		//auto app = py::module::import("app");
		//app.attr("Restore")();
	}
}
