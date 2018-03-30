#include <FishEditor/Serialization/YAMLArchive.hpp>
#include <FishEditor/Serialization/NativeFormatImporter.hpp>
#include <FishEditor/Serialization/DefaultImporter.hpp>
#include <FishEngine/Scene.hpp>

#include <FishEditor/EditorApplication.hpp>
#include <FishEditor/AssetDatabase.hpp>

using namespace FishEngine;
using namespace FishEditor;

int main()
{
#if _WIN32
	const std::string projectPath = R"(D:\program\FishEngine-Experiment\Project\Unity)";
#else
	const std::string projectPath = "/Users/yushroom/program/FishEngine-Experiment/Projects/Unity/";
#endif
	const char* prefabPath = "TestPrefab/Cube.prefab";
	const char* scenePath = "TestPrefab/scene.unity";
	EditorApplication::GetInstance().OpenProject(projectPath);
	
	auto scene = SceneManager::CreateScene("Test");
	SceneManager::SetActiveScene(scene);
	
	Prefab* prefab = (Prefab*)AssetDatabase::LoadMainAssetAtPath(prefabPath);
	{
		std::ofstream fout("FishEngine_test.prefab");
		YAMLOutputArchive output(fout);
		output.Dump(prefab);
		fout.close();
	}

	{
		auto importer = AssetImporter::GetAtPath(scenePath);
		importer->Import();
	}

	{
		YAMLInputArchive input;
		auto objects = input.LoadAll(projectPath + "/Assets/" + scenePath);
	}

	return 0;
}