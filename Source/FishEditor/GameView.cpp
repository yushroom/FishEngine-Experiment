#include <FishEditor/GameView.hpp>

#include <FishGUI/FishGUI.hpp>
#include <FishEngine/Debug.hpp>
#include <FishEngine/Screen.hpp>
//#include <FishEngine/Render/GLEnvironment.hpp>

namespace FishEditor
{
	GameView* GameView::s_Current = nullptr;
	
	void GameView::Init()
	{
		int w = GetWidth();
		int h = GetHeight();
//		m_Framebuffer.EnableMSAA();
		m_Framebuffer.Init(w, h);
		this->Resize(w, h);
	}
	
	void GameView::DrawImpl()
	{
		auto tex = m_Framebuffer.GetColorTexture();
		FishGUI::Image(tex, m_rect, true);
		//GLint defaultFBO;
		//glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Framebuffer.GetFramebuffer());
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFBO);
		//glBlitFramebuffer(0, 0, m_rect.width, m_rect.height, m_rect.x, m_rect.y, m_rect.x+m_rect.width, m_rect.y+m_rect.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		//glCheckError();
		//glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
		
		float x = FishGUI::Context::GetInstance().GetPixelRatioOfCurrentWindow();
		float w = m_rect.width * x;
		float h = m_rect.height * x;
		auto size = m_Framebuffer.GetSize();
		if (w != size.width || h != size.height)
		{
			this->Resize(w, h);
		}
	}
	
	void GameView::Resize(int width, int height)
	{
		if (width <= 0 || height <= 0)
			return;
		LogInfo(FishEngine::Format("GLWidget::Resize w={} h={}", width, height));
		m_Framebuffer.Resize(width, height);
		FishEngine::Screen::SetResolution(width, height, false);
	}

}
