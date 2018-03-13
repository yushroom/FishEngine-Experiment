#pragma once

#include <string>
#include <FishEngine/Scene.hpp>

namespace FishEditor
{
	// Used when creating a new scene in the Editor.
	enum class NewSceneMode
	{
		Single,		// All current open scenes are closed and the newly created scene are opened.
		Additive,	// The newly created scene is added to the current open scenes.
	};

	// Used when creating a new scene in the Editor.
	enum class NewSceneSetup
	{
		EmptyScene,		// No game objects are added to the new scene.
		DefaultGameObjects	// Adds default game objects to the new scene (a light and camera).
	};

	// Used when opening a scene in the Editor to specify how a scene should be opened.
	enum class OpenSceneMode
	{
		Single,		// Closes all current open scenes and loads a scene.
		Additive,	// Adds a scene to the current open scenes and loads it.
		AdditiveWithoutLoading		// Adds a scene to the current open scenes without loading it. It will show up as 'unloaded' in the Hierarchy Window.
	};

	// The setup information for a scene in the SceneManager.
	class SceneSetup
	{
	public:
		std::string m_Path;
		bool m_IsLoaded = false;
		bool m_IsActive = false;
	};

	class EditorSceneManager
	{
	public:

		static FishEngine::Scene* NewScene(const NewSceneSetup& setup, NewSceneMode mode = NewSceneMode::Single)
		{

		}
	};
}