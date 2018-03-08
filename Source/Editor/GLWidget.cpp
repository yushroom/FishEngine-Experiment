#include "GLWidget.hpp"

#include <FishEngine/Debug.hpp>
#include <FishGUI/FishGUI.hpp>
#include "SceneViewApp.hpp"

GLWidget::GLWidget(const char* name, SceneViewApp* app)
	: FishGUI::IMWidget(name)
{
	m_app = app;
}

void GLWidget::Init()
{
	int w = GetWidth();
	int h = GetHeight();
	m_framebuffer.Init(w, h);
	this->Resize(w, h);
}

void GLWidget::DrawImpl()
{
	auto tex = m_framebuffer.GetColorTexture();
	FishGUI::Image(tex, m_rect, true);

	auto size = m_framebuffer.GetSize();
	if (m_rect.width != size.width || m_rect.height != size.height)
	{
		this->Resize(m_rect.width, m_rect.height);
	}
}

void GLWidget::DrawScene()
{
	m_framebuffer.Bind();
	m_app->Update();
	m_framebuffer.Unbind();
}

void GLWidget::Resize(int width, int height)
{
	if (width <= 0 || height <= 0)
		return;
	LogInfo(FishEngine::Format("GLWidget::Resize w={} h={}", width, height));
	m_framebuffer.Resize(width, height);
	m_app->Resize(width, height);
}