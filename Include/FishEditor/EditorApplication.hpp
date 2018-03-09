
class EditorInternalApp;

namespace FishEditor
{
	class EditorApplication final
	{
	public:

		static EditorApplication & GetInstance()
		{
			static EditorApplication instance;
			return instance;
		}

		void Init();

		void Update();

		void Play();
		void Stop();
		void Pause();
		void NextFrame();

	private:

		EditorApplication() = default;
		EditorApplication(const EditorApplication&) = delete;
		EditorApplication& operator=(const EditorApplication&) = delete;

		EditorInternalApp * m_app;
		bool m_IsPlaying = false;
	};
}