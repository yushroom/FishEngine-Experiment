#include <FishEditor/Serialization/YAMLArchive.hpp>
#include <FishEditor/Serialization/NativeFormatImporter.hpp>
#include <FishEngine/Scene.hpp>

using namespace FishEngine;
using namespace FishEditor;

int main()
{
	auto scene = SceneManager::CreateScene("Test");
	SceneManager::SetActiveScene(scene);

	const char* prefabPath = R"(D:\program\FishEngine-Experiment\Project\Unity\Assets\TestPrefab\Cube.prefab)";
	NativeFormatImporter importer;
	importer.SetAssetPath(prefabPath);
	Prefab* prefab = (Prefab*)importer.Import();

#if _WIN32
	const char* path = R"(D:\workspace\unity\FishEngine\Assets\TestPhysics.unity)";
#else
	const char* path = "/Users/yushroom/program/Unity/FishEngine/Assets/TestPhysics.unity";
#endif


	YAMLInputArchive input;
	auto objects = input.LoadAll(path);
    return 0;
}

