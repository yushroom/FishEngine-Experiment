#include <FishEngine/Render/Shader.hpp>
#include <FishEditor/Path.hpp>

#if FISHENGINE_PLATFORM_APPLE
#	define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>

using namespace FishEngine;

int main()
try
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

	std::string SkyBox_Procedural = FishEditor::ReadFileAsString("/Users/yushroom/program/FishEngine-Experiment/Assets/Shaders/SkyBox-Procedural.shader");

	std::string vs = "#version 410 core\n#define VERTEX\n" + SkyBox_Procedural;
	std::string fs = "#version 410 core\n#define FRAGMENT\n" + SkyBox_Procedural;

	auto s = Shader::FromString(vs, fs);

	puts("Done.");
	return 0;
}
catch(std::exception& e) {
	puts(">>> Exception:");
	puts(e.what());
	return 1;
}