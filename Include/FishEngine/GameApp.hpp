#pragma once

struct GLFWwindow;

namespace FishEngine
{
	class AbstractGameApp
	{
	public:
		AbstractGameApp();
		virtual ~AbstractGameApp() = default;

		virtual void Init() = 0;
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;

		static AbstractGameApp* GetCurrent()
		{
			return s_current;
		}

	protected:
		static AbstractGameApp* s_current;
	};

    class GameApp : public AbstractGameApp
    {
    public:
		
		GameApp();
		virtual ~GameApp() = default;
		
        virtual int Run();
        //virtual void Init() = 0;
        //virtual void Update() = 0;
		
		void SetWindowSize(int w, int h)
		{
			m_windowWidth = w;
			m_windowHeight = h;
		}
		
		virtual void Resize(int width, int height);

		static GameApp* GetCurrent()
		{
			return (GameApp*) AbstractGameApp::s_current;
		}

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
    };
}
