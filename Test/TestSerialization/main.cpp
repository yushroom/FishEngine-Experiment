#include <FishEditor/Serialization/YAMLArchive.hpp>
#include <FishEditor/Serialization/NativeFormatImporter.hpp>
#include <FishEngine/Scene.hpp>

#include <FishEditor/EditorApplication.hpp>
#include <FishEditor/AssetDatabase.hpp>

using namespace FishEngine;
using namespace FishEditor;

int main()
{
#if _WIN32
#else
	const std::string projectPath = "/Users/yushroom/program/FishEngine-Experiment/Projects/Unity/";
#endif
	const char* prefabPath = "Cube.prefab";
	const char* scenePath = "Scene01.unity";
	EditorApplication::GetInstance().OpenProject(projectPath);
	
	auto scene = SceneManager::CreateScene("Test");
	SceneManager::SetActiveScene(scene);
	
	Prefab* prefab = (Prefab*)AssetDatabase::LoadMainAssetAtPath(prefabPath);

	YAMLInputArchive input;
	auto objects = input.LoadAll(projectPath+"/Assets/"+scenePath);
    return 0;
}

