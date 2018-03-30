#pragma once

#include <string>

namespace FishEditor
{
	class EditorApplication;
}

namespace FishEngine
{
	class Application final
	{
	public:

		static Application& GetInstance()
		{
			static Application instance;
			return instance;
		}

		// The value depends on which platform you are running on:
		// Unity Editor: <path to project folder>/Assets
		// Mac player: <path to player app bundle>/Contents
		// Win/Linux player: <path to executablename_Data folder> (note that most Linux installations will be case-sensitive!)
		const std::string& GetDataPath() const
		{
			return m_DataPath;
		}

	private:
		Application() = default;
		Application(const Application&) = delete;
		const Application& operator=(const Application&) = delete;

		friend class FishEditor::EditorApplication;
		std::string m_DataPath;
	};
}