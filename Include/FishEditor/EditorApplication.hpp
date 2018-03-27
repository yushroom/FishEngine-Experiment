#pragma once

#include <string>
#include "FileNode.hpp"

#include <boost/signals2/signal.hpp>


class EditorInternalApp;

namespace FishEngine
{
	class Scene;
}

namespace FishEditor
{
	class EditorApplication final
	{
	public:

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
		void Pause();
		void Resume();

		boost::signals2::signal<void()> OnProjectOpened;

	private:

		EditorApplication() = default;
		EditorApplication(const EditorApplication&) = delete;
		EditorApplication& operator=(const EditorApplication&) = delete;

		EditorInternalApp * m_app;
		bool m_IsPlaying = false;

		FishEngine::Scene * m_currentScene = nullptr;	// scene in editor
		
		std::string m_ApplicationPath;
//		FileNode* m_AssetRootDir = nullptr;
	};
}
