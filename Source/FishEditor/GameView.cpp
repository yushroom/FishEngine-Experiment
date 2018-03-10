#include <FishEditor/GameView.hpp>

#include <FishGUI/FishGUI.hpp>
#include <FishEngine/Debug.hpp>
#include <FishEngine/Screen.hpp>

namespace FishEditor
{
	GameView* GameView::s_Current = nullptr;
	
	void GameView::Init()
	{
		int w = GetWidth();
		int h = GetHeight();
		m_FrameBuffer.Init(w, h);
		this->Resize(w, h);
	}
	
	void GameView::DrawImpl()
	{
		auto tex = m_FrameBuffer.GetColorTexture();
		FishGUI::Image(tex, m_rect, true);
		
		auto size = m_FrameBuffer.GetSize();
		if (m_rect.width != size.width || m_rect.height != size.height)
		{
			this->Resize(m_rect.width, m_rect.height);
		}
	}
	
	void GameView::Resize(int width, int height)
	{
		if (width <= 0 || height <= 0)
			return;
		LogInfo(FishEngine::Format("GLWidget::Resize w={} h={}", width, height));
		m_FrameBuffer.Resize(width, height);
		FishEngine::Screen::SetResolution(width, height, false);
	}

}
