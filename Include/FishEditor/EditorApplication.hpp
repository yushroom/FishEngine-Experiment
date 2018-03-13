#pragma once

#include <string>

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

		static void OpenProject(const std::string& projectPath);

		void Init();

		void Update();

		void Play();
		void Stop();
		void Pause();
		void Resume();

	private:

		EditorApplication() = default;
		EditorApplication(const EditorApplication&) = delete;
		EditorApplication& operator=(const EditorApplication&) = delete;

		EditorInternalApp * m_app;
		bool m_IsPlaying = false;

		FishEngine::Scene * m_currentScene = nullptr;	// scene in editor
	};
}
