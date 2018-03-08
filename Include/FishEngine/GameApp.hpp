#pragma once

struct GLFWwindow;

namespace FishEngine
{
    class GameApp
    {
    public:
		
		GameApp();
		virtual ~GameApp() = default;
		
        virtual int Run();
        virtual void Init() = 0;
        virtual void Update() = 0;
		
		void SetWindowSize(int w, int h)
		{
			m_windowWidth = w;
			m_windowHeight = h;
		}
		
		static GameApp* GetCurrent()
		{
			return s_current;
		}
		
		virtual void Resize(int width, int height);

    protected:
		// GLFW callback
		static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void WindowSizeCallback(GLFWwindow* window, int width, int height);
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
		
		void Reload();
		
    private:
		GLFWwindow* m_window = nullptr;
        int m_windowWidth = 1;
        int m_windowHeight = 1;
		
		static GameApp* s_current;
    };
}
