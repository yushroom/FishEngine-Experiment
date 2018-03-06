#include <FishEngine/GameApp.hpp>

#include <FishEngine/FishEngine.hpp>

#if FISHENGINE_PLATFORM_WINDOWS
#	include <GL/glew.h>
#endif

#if FISHENGINE_PLATFORM_APPLE
#	define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>

#include <FishEngine/FishEngine.hpp>
#include <FishEngine/Screen.hpp>

#include <FishEngine/System/UISystem.hpp>
#include <FishEngine/System/ScriptSystem.hpp>
#include <FishEngine/System/RenderSystem.hpp>

#include <thread>
#include <chrono>

namespace FishEngine
{
	GameApp* GameApp::s_current = nullptr;
	
	GameApp::GameApp()
	{
		Assert(s_current == nullptr);
		s_current = this;
	}
	
    int GameApp::Run()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "FishEngine", nullptr, nullptr);
        glfwMakeContextCurrent(m_window);
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
		
		glfwSetWindowSizeCallback(m_window, GameApp::WindowSizeCallback);
		glfwSetKeyCallback(m_window, GameApp::KeyCallBack);
		glfwSetFramebufferSizeCallback(m_window, GameApp::FramebufferSizeCallback);
		
		glfwGetFramebufferSize(m_window, &Screen::s_width, &Screen::s_height);
		Screen::s_pixelsPerPoint = static_cast<float>(Screen::s_width) / m_windowWidth;
		
		FishEngine::Start();
		Init();
		
		
		glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		
		auto lastTimeStamp = glfwGetTime();
		
		while (!glfwWindowShouldClose(m_window))
		{

//			int iconified = glfwGetWindowAttrib(m_window, GLFW_ICONIFIED);
//			if (iconified)
//				std::this_thread::sleep_for(200ms);

			
			FishEngine::Update();
			
			// Rendering
			RenderSystem::GetInstance().Update();
			
			
//			glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			UISystem::GetInstance().BeginDraw();
			UISystem::GetInstance().Update();
			UISystem::GetInstance().AfterDraw();

			
			glfwSwapBuffers(m_window);
			glfwPollEvents();
			
			constexpr int targetFPS = 60;
			constexpr float targetTimePreFrame = 1000.0 / targetFPS;
			auto now = glfwGetTime();
			auto interval = now - lastTimeStamp;
			lastTimeStamp = now;
//			printf("%f\n", interval);
			using namespace std::chrono_literals;
			if (interval < targetTimePreFrame)
			{
				auto sleeptime = targetTimePreFrame - interval;
				std::this_thread::sleep_for(sleeptime*1ms);
			}
		}
		
		FishEngine::Clean();
		glfwTerminate();
		return 0;
    }
	
	void GameApp::Reload()
	{
		ScriptSystem::GetInstance().Reload();
//		Init();
	}
	
	void GameApp::Resize(int width, int height)
	{
		glfwSetWindowSize(m_window, width, height);
		// call WindowSizeCallback automatically
//		m_windowWidth = width;
//		m_windowHeight = height;
//		glfwGetFramebufferSize(m_window, &Screen::s_width, &Screen::s_height);
	}
	
	void GameApp::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_R)
				ScriptSystem::GetInstance().Reload();
			else if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, 1);
		}
	}
	
	void GameApp::WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		//Debug::Log("window size changed");
		auto app = GameApp::GetCurrent();
		app->m_windowWidth = width;
		app->m_windowHeight = height;
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		Screen::s_pixelsPerPoint = static_cast<float>(w) / width;
		if (w != 0 && h != 0)
		{
			Screen::s_width = w;
			Screen::s_height = h;
		}
	}
	
	void GameApp::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
	{
		auto app = GameApp::GetCurrent();
		glfwGetWindowSize(window, &app->m_windowWidth, &app->m_windowHeight);
		Screen::s_pixelsPerPoint = static_cast<float>(width) / width;
		Screen::s_width = width;
		Screen::s_height = height;
	}
}
