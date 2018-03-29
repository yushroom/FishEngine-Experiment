#include <FishEditor/Serialization/YAMLArchive.hpp>
#include <FishEngine/Scene.hpp>

using namespace FishEngine;
using namespace FishEditor;

int main()
{
	const char* path = "/Users/yushroom/program/Unity/FishEngine/Assets/TestPhysics.unity";
	auto scene = SceneManager::CreateScene("TestPhysics");
	SceneManager::SetActiveScene(scene);
	YAMLInputArchive input;
	auto objects = input.LoadAll(path);
    return 0;
}

