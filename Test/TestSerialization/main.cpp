#include <FishEditor/Serialization/YAMLArchive.hpp>
#include <FishEditor/Serialization/NativeFormatImporter.hpp>
#include <FishEditor/Serialization/DefaultImporter.hpp>
#include <FishEngine/Scene.hpp>

#include <FishEditor/EditorApplication.hpp>
#include <FishEditor/AssetDatabase.hpp>

#if FISHENGINE_PLATFORM_APPLE
#	define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>

using namespace FishEngine;
using namespace FishEditor;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	auto window = glfwCreateWindow(1, 1, "FishEngine", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

#if FISHENGINE_PLATFORM_WINDOWS
	glewExperimental = GL_TRUE;
		auto err = glewInit();
		if (err != GLEW_OK)
		{
			printf("GLEW not initialized");
			abort();
		}
#endif

#if _WIN32
	const std::string projectPath = R"(D:\program\FishEngine-Experiment\Project\Unity)";
#else
	const std::string projectPath = "/Users/yushroom/program/Unity/FishEngine";
#endif
//	const char* prefabPath = "Assets/TestPrefab/Cube.prefab";
	const char* scenePath = "Assets/TestPhysics.unity";
	EditorApplication::GetInstance().OpenProject(projectPath);
	
	auto scene = SceneManager::CreateScene("Test");
	SceneManager::SetActiveScene(scene);
	
//	Prefab* prefab = (Prefab*)AssetDatabase::LoadMainAssetAtPath(prefabPath);
//	{
//		std::ofstream fout("FishEngine_test.prefab");
//		YAMLOutputArchive output(fout);
//		output.Dump(prefab);
//		fout.close();
//	}

	FishEngine::Init();
	AssetDatabase::StaticInit();

	{
		auto importer = AssetImporter::GetAtPath(scenePath);
	}

	{
		auto str = ReadFileAsString(projectPath + '/' + scenePath);
		YAMLInputArchive input;
		auto objects = input.LoadAllFromString(str);
	}

	{

		auto importer = AssetImporter::GetAtPath(scenePath);
		auto guid = importer->GetGUID();
	}

	return 0;
}