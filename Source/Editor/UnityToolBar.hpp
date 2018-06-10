#pragma once

#include <FishGUI/Widget.hpp>
#include <functional>
#include <boost/signals2/signal.hpp>

class UnityToolBar : public FishGUI::ToolBar
{
public:
	UnityToolBar() : ToolBar() {}
	
	UnityToolBar(UnityToolBar&) = delete;
	UnityToolBar& operator=(UnityToolBar&) = delete;
	
	virtual void Draw() override;
	
	typedef std::function<void(void)> Callback;

	void SetTransformType();

	boost::signals2::signal<void(void)> OnRun;
	boost::signals2::signal<void(void)> OnStop;
	boost::signals2::signal<void(void)> OnPause;
	boost::signals2::signal<void(void)> OnResume;

protected:
	
	void Run()
	{
		OnRun();
		m_IsPlaying = true;
	}
	
	void Stop()
	{
		OnStop();
		m_PauseAfterNextFrame = false;
		m_IsPlaying = false;
		m_Paused = false;
	}
	
	void Pause()
	{
		if (m_IsPlaying)
		{
			OnPause();
//			m_Paused = true;
		}
		else
			m_PauseAfterNextFrame = true;
		m_Paused = true;
	}
	
	void Resume()
	{
		if (m_IsPlaying)
			OnResume();
		else
			m_PauseAfterNextFrame = false;
		m_Paused = false;
	}
//	void NextFrame() { OnNextFrame(); }
	
private:
	bool m_IsPlaying = false;
	bool m_Paused = false;
	bool m_PauseAfterNextFrame = false;
};
