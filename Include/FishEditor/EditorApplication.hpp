#pragma once

#include <string>
#include "FileNode.hpp"

#include <boost/signals2/signal.hpp>

class EditorInternalApp;
class HierarchyView;

namespace FishEngine
{
	class Scene;
	class Object;
}

namespace FishEditor
{


	class EditorApplication final
	{
	public:

		// noncopyable
		EditorApplication(const EditorApplication&) = delete;
		EditorApplication& operator=(const EditorApplication&) = delete;

		static EditorApplication & GetInstance()
		{
			static EditorApplication instance;
			return instance;
		}

		void OpenProject(const std::string& projectPath);

		void Init();

		void Update();

		void Play();
		void Stop();


		bool GetIsPlaying() const { return m_IsPlaying; }
		void SetIsPlaying(bool value) { m_IsPlaying = value; }

		bool GetIsPaused() const { return m_IsPaused; }
		void SetIsPaused(bool value) { m_IsPaused = value; }

		boost::signals2::signal<void()> OnProjectOpened;

		void SetHierarchyView(HierarchyView* hierarchyView)
		{
			m_HierarchyView = hierarchyView;
		}

	private:

		EditorApplication() = default;


		EditorInternalApp * m_app = nullptr;
		bool m_IsPlaying = false;
		bool m_IsPaused = false;

		FishEngine::Scene * m_currentScene = nullptr;	// scene in editor
		
		std::string m_ApplicationPath;
//		FileNode* m_AssetRootDir = nullptr;

		HierarchyView* m_HierarchyView = nullptr;
		std::map<FishEngine::Object*, FishEngine::Object*> m_SceneObjectMemo;
	};
}
